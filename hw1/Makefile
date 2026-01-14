CXX := g++
CXXFLAGS := -g -Wall --std=c++11

VALGRIND := valgrind --tool=memcheck --leak-check=yes

all: ulliststr_test rem_dup 

ulliststr_test: ulliststr_test.o ulliststr.o
	$(CXX) $(CXXFLAGS) $^ -o $@
ulliststr.o: ulliststr.cpp ulliststr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
ulliststr_test.o: ulliststr_test.cpp ulliststr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


rem_dup: rem_dup.cpp rem_dup_lib.o
	$(CXX) $(CXXFLAGS) $^ -o $@
rem_dup_lib.o: rem_dup_lib.cpp rem_dup_lib.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f *.o ulliststr_test rem_dup 

.PHONY: clean 