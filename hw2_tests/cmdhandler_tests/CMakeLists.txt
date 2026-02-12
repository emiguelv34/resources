AUX_SOURCE_DIRECTORY(${HW_DIR} SRC_DIR)

# this will ensure that test files are acessible regardless of how we ran cmake
FILE(COPY "../test-small" DESTINATION ".")

add_source_problem(
	NAME cmdhandler
	USER_SOURCE ${SRC_DIR} 
	TEST_SOURCE handler_tests.cpp)
