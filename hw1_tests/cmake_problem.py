# This file is a bridge between the old, completely python-based testing system,
# and the new CMake and CTest based system.

# note: the name of the passed problem has to be a substring of the directory that the tests for it are stored in

import os
import glob
import xml.etree.ElementTree as ET
import cs_grading
from cs_grading import executable_tools
from cs_grading import logging_tools
import re

# regexes for detecting valgrind output
subprocess_valgrind_failure_re = re.compile("WARNING: user program created Valgrind errors")
valgrind_results_re = re.compile(
    r"==[0-9]+== LEAK SUMMARY:\n==[0-9]+== \s+ definitely lost: ([0-9,]+) bytes in [0-9,]+ blocks\n==[0-9]+== \s+ indirectly lost: ([0-9,]+) bytes in [0-9,]+ blocks\n==[0-9]+== \s+ possibly lost: ([0-9,]+) bytes in [0-9,]+ blocks\n==[0-9]+== \s+ still reachable: ([0-9,]+) bytes in [0-9,]+ blocks\n==[0-9]+== \s+ suppressed: ([0-9,]+) bytes in [0-9,]+ blocks")

valgrind_errors_re = re.compile(r"==[0-9]+== ERROR SUMMARY: ([0-9,]+) errors from [0-9,]+ contexts")

# Generates grade reports for the given problem by parsing CTest's XML file
def cmake_problem(problem):
    # set path to output files
    problem.compile_file = os.path.join('compile-logs', problem.name + '.complog')
    stdout_file_path = os.path.join('test-output', problem.name + '-test-stdout.txt')

    stdout_file = open(stdout_file_path, 'w')

    # Find tests' output XML file, buried a couple of directories deep
    xml_path = glob.glob("Testing/*-*/Test.xml")
    if len(xml_path) == 0:
        logging_tools.log_error("Cannot find test XML output file!", problem.logging_level)
        return

    elif len(xml_path) > 1:
        logging_tools.log_error("Multiple candidates for test XML file: " + " ".join(xml_path), problem.logging_level)
        return

    logging_tools.log_info("Found XML output file: " + xml_path[0], problem.logging_level)

    # parse XML file
    tree = ET.parse(xml_path[0])
    root = tree.getroot()

    test_list = []
    didnt_run_tests = set()
    crashed_tests = set()
    timed_out_tests = set()

    failed_tests = {}
    passed_tests = {}

    # Valgrind exit codes, indexed by test
    valgrind_exit_codes = []

    # now, go through all tests
    test_elements = root.findall('.//Test')
    for test_results_element in test_elements:
        path_elem = test_results_element.find('Path')
        if path_elem is None or problem.name not in path_elem.text:
            continue
        
        name_elem = test_results_element.find('Name')
        test_name = name_elem.text if name_elem is not None else "Unknown"
        #print("\n>> Processing test: " + test_name)

        # write test results to output file
        stdout_file.write("""
------------------------------------------------------------------------------
OUTPUT OF TEST %s:
------------------------------------------------------------------------------
""" % test_name)
        
        # Get measurement value
        measurement_elem = test_results_element.find('.//Measurement/Value')
        if measurement_elem is not None:
            stdout_file.write(measurement_elem.text if measurement_elem.text else "")

        # detect Valgrind failues
        # note: we want to assign seperate deductions for Valgrind failures and actual test case failures.  So,
        # we can't use Valgrind's --error-exitcode option, since that would make CTest think all the tests had failed.
        valgrind_error = False;
        measurement_text = measurement_elem.text if measurement_elem is not None and measurement_elem.text else ""
        match_list = valgrind_results_re.findall(measurement_text)

        if match_list is None or len(match_list) < 1:
            # program may have died before it got to even producing the valgrind output, or it's a test that doesn't use Valgrind
            pass

        else:
            # make sure to grab the last match in case a student tries to defeat this by printing a fake valgrind summary
            definitely_lost = int(match_list[-1][0].replace(",", ""))
            indirectly_lost = int(match_list[-1][1].replace(",", ""))
            possibly_lost = int(match_list[-1][2].replace(",", ""))
            still_reachable = int(match_list[-1][3].replace(",", ""))
            suppressed = int(match_list[-1][4].replace(",", ""))

            if definitely_lost > 0 or indirectly_lost > 0 or possibly_lost > 0 or still_reachable > 0:
                valgrind_error = True

        # now look for "X errors in X contexts"
        error_match_list = valgrind_errors_re.findall(measurement_text)

        if error_match_list is None or len(error_match_list) < 1:
            # program may have died before it got to even producing the valgrind output, or it's a test that doesn't use Valgrind
            pass

        else:
            # make sure to grab the last match in case a student tries to defeat this by printing a fake valgrind summary
            num_errors = int(error_match_list[-1].replace(",", ""))

            if num_errors > 0:
                valgrind_error = True

        if subprocess_valgrind_failure_re.search(measurement_text) is not None:
            valgrind_error = True

        # now, parse out the test status
        status_attr = test_results_element.get('Status', 'failed')
        passed = status_attr == 'passed'

        # true if the test was not run (as in, it failed to build)
        didnt_run = False

        # true if the test dies with a segfault/sigfpe/etc
        crashed = False

        # true if test timed out
        timed_out = False

        test_time = -1.0

        named_measurements = test_results_element.findall('.//NamedMeasurement')
        
        if len(named_measurements) == 0:
            # if the test did not run because the excutable was not found, there might not be the usual set of NamedMeasurements
            didnt_run = True
        elif status_attr == "notrun":
            didnt_run = True
        else:
            # iterate through namedmeasurements
            for measurement_element in named_measurements:
                name_attr = measurement_element.get('name', '')
                value_elem = measurement_element.find('Value')
                value_text = value_elem.text if value_elem is not None and value_elem.text else ''
                
                if name_attr == 'Execution Time':
                    test_time = float(value_text)

                if not passed and name_attr == 'Exit Code':
                    if value_text == 'Timeout':
                        timed_out = True
                    elif value_text != 'Failed':
                        # if exit code is something other than Failed it means that the test crashed
                        crashed = True

                if not passed and name_attr == 'Exit Value':
                    if int(value_text) == 127:
                        # Valgrind returns this to indicate that the test was not run
                        didnt_run = True

        # write test data to collections
        test_list.append(test_name)
        if crashed:
            crashed_tests.add(test_name)
        elif timed_out:
            timed_out_tests.add(test_name)
        elif didnt_run:
            didnt_run_tests.add(test_name)
        elif passed:
            passed_tests[test_name] = test_time
        else:
            failed_tests[test_name] = test_time

        # figure out what the Valgrind exit code should have been for this test
        if valgrind_error:
            valgrind_exit_codes.append(executable_tools.VALGRIND_ERROR)
        elif crashed:
            valgrind_exit_codes.append(executable_tools.EXE_ERROR)
        else:
            valgrind_exit_codes.append(executable_tools.VALGRIND_SUCCESS)

    stdout_file.close()
    # write test result files
    cs_grading.write_test_result(
        problem.result_file,
        test_list, didnt_run_tests, timed_out_tests, crashed_tests, failed_tests, passed_tests,
        logging_level=problem.logging_level)
    cs_grading.write_formatted_result(
        problem.formatted_file,
        test_list, valgrind_exit_codes, didnt_run_tests, timed_out_tests, crashed_tests, failed_tests, passed_tests,
        logging_level=problem.logging_level)
