#!/usr/bin/python3

import cs_grading
import cmake_problem
import setting
import os

source_dir = os.path.dirname(os.path.realpath(__file__))

RESULT_DIR = 'results/' # where test results are stored
GRADER_CONFIG = '../grader.txt'
RUBRIC_GENERAL = os.path.join(source_dir, 'rubric', 'general.config')
#RUBRIC_LLREC = os.path.join(source_dir, 'rubric', 'llrec.config')
RUBRIC_STACK = os.path.join(source_dir, 'rubric', 'stack.config')
RUBRIC_LAUNDRY = os.path.join(source_dir, 'rubric', 'laundry.config')
RUBRIC_HEAP = os.path.join(source_dir, 'rubric', 'heap.config')
RUBRIC_RH = os.path.join(source_dir, 'rubric', 'rh.config')
#RUBRIC_RECS = os.path.join(source_dir, 'rubric', 'recs.config')

GRADE_REPORT_DIR = './'

HOMEWORK = cs_grading.Homework(
    3,
    RESULT_DIR,
    False,
    detailed_results=setting.DETAILED_RESULT,
    compile_flags=setting.COMPILE_FLAGS,
    logging_level=setting.LOGGING_LEVEL,
)

#P1 = cs_grading.Problem(HOMEWORK, 1, 'login', 15)
#P2 = cs_grading.Problem(HOMEWORK, 2, 'heap', 25)
#P3 = cs_grading.Problem(HOMEWORK, 3, 'recs', 40)
#P4 = cs_grading.Problem(HOMEWORK, 3, 'llrec', 13)
P2 = cs_grading.Problem(HOMEWORK, 2, 'stack', 15)
P3 = cs_grading.Problem(HOMEWORK, 3, 'laundry', 10)
P4 = cs_grading.Problem(HOMEWORK, 4, 'heap', 35)
P5 = cs_grading.Problem(HOMEWORK, 5, 'rh', 40)
#for problem, rubric in [(P4, RUBRIC_LLREC), (P2, RUBRIC_STACK), (P3, RUBRIC_LAUNDRY), (P4, RUBRIC_HEAP), (P5, RUBRIC_RH)]:
for problem, rubric in [(P2, RUBRIC_STACK), (P3, RUBRIC_LAUNDRY), (P4, RUBRIC_HEAP), (P5, RUBRIC_RH)]:
    problem.generate_results(
        cmake_problem.cmake_problem,
        True,
        timeout=0,)
    if setting.GENERATE_GRADE_REPORT:
        problem.grade_problem(RUBRIC_GENERAL, rubric)
    if setting.OPEN_RESULT:
        problem.open_result(text_editor=setting.TEXT_EDITOR)


GRADER = cs_grading.Grader(GRADER_CONFIG, setting.LOGGING_LEVEL)
cs_grading.generate_grade_report(HOMEWORK, GRADER, GRADE_REPORT_DIR, overwrite=True, logging_level=setting.LOGGING_LEVEL)
