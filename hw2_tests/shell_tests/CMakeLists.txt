AUX_SOURCE_DIRECTORY(${HW_DIR} DIR_SOURCES)

# this will ensure that test files are acessible regardless of how we ran cmake
FILE(COPY "../test-small" DESTINATION ".")

add_makefile_problem(
  NAME shell
  EXECUTABLE_NAME search-shell
#	USER_SOURCE ${HW_DIR}/search-shell.cpp ${HW_DIR}/searchui.cpp ${HW_DIR}/searcheng.cpp ${HW_DIR}/cmdhandler.cpp ${HW_DIR}/handler.cpp ${HW_DIR}/combiners.cpp ${HW_DIR}/txt_parser.cpp ${HW_DIR}/md_parser.cpp ${HW_DIR}/webpage.cpp ${HW_DIR}/util.cpp
	TEST_SOURCE shell_tests.cpp)
