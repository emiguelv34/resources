#!/usr/bin/python3

import cs_grading
import cmake_problem
import setting
import os

source_dir = os.path.dirname(os.path.realpath(__file__))

RESULT_DIR = 'results/' # where test results are stored
GRADER_CONFIG = '../grader.txt'
RUBRIC_GENERAL = os.path.join(source_dir, 'rubric', 'general.config')
RUBRIC_MDPARSER = os.path.join(source_dir, 'rubric', 'mdparser.config')
RUBRIC_COMBINER = os.path.join(source_dir, 'rubric', 'combiner.config')
RUBRIC_CMDHANDLER = os.path.join(source_dir, 'rubric', 'cmdhandler.config')
RUBRIC_SEARCHENG = os.path.join(source_dir, 'rubric', 'searcheng.config')
RUBRIC_SHELL = os.path.join(source_dir, 'rubric', 'shell.config')

GRADE_REPORT_DIR = './'

HOMEWORK = cs_grading.Homework(
    2,
    RESULT_DIR,
    False,
    detailed_results=setting.DETAILED_RESULT,
    compile_flags=setting.COMPILE_FLAGS,
    logging_level=setting.LOGGING_LEVEL,
)

#P1 = cs_grading.Problem(HOMEWORK, 1, 'login', 15)
#P2 = cs_grading.Problem(HOMEWORK, 2, 'heap', 25)
#P3 = cs_grading.Problem(HOMEWORK, 3, 'recs', 40)

P1 = cs_grading.Problem(HOMEWORK, 6.1, 'mdparser', 20)
P2 = cs_grading.Problem(HOMEWORK, 6.2, 'combiner', 15)
P3 = cs_grading.Problem(HOMEWORK, 6.3, 'cmdhandler', 10)
P4 = cs_grading.Problem(HOMEWORK, 6.4, 'searcheng', 45)
P5 = cs_grading.Problem(HOMEWORK, 6.5, 'shell', 10)

for problem, rubric in [(P1, RUBRIC_MDPARSER), (P2, RUBRIC_COMBINER), (P3, RUBRIC_CMDHANDLER), (P4, RUBRIC_SEARCHENG), (P5, RUBRIC_SHELL)]:
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
