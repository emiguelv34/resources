
add_source_problem(
    NAME unrolledlist
    USER_SOURCE ${HW_DIR}/ulliststr.cpp
    TEST_SOURCE list_utils.cpp insert_tests.cpp misc_tests.cpp set_tests.cpp remove_tests.cpp)
     # RUNTIME_TEST_SOURCE runtime_tests.cpp)