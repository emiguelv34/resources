AUX_SOURCE_DIRECTORY(${HW_DIR} DIR_SOURCES)

# this will ensure that test files are acessible regardless of how we ran cmake
FILE(COPY "../test-small" DESTINATION ".")

add_source_problem(
	NAME searcheng
	USER_SOURCE ${DIR_SOURCES} 
	TEST_SOURCE searcheng_tests.cpp
)
