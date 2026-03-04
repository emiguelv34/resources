#include <gtest/gtest.h>
#include <runtime_evaluator.h>
#include <string>
#include <iostream>
#include <kwsys/SystemTools.hxx>
#include <string>
#include <user_code_runner.h>
#include <random_generator.h>
#include <fstream>
using namespace std;

TEST(LaundryRuntime, AddRemove)
{
	RuntimeEvaluator evaluator("laundry", 15, 25, 30, [&](uint64_t numElements, RandomSeed seed)
	{
		string executablePath = "./hw4_laundry_runtime";
		string testFolder = TEST_BINARY_DIR "/laundry_tests/testFiles/runtime";
		kwsys::SystemTools::MakeDirectory(testFolder);
		string inputFilePath = testFolder + "/input.txt";
		string outputFilePath = testFolder + "/output.txt";
		string stdoutFilePath = testFolder + "/stdout.txt";

		stringstream s;
		for(uint64_t i = 0; i < numElements; i++)
		{
			int val = rand() % 2 - 1;
			s << val << ' ';
		}
		ofstream inputWriter;
		inputWriter.open(inputFilePath);
		inputWriter << s.rdbuf();
		UserCodeRunner runner(testFolder, executablePath, {inputFilePath, outputFilePath}, stdoutFilePath, false);
		BenchmarkTimer timer;
		runner.execute();
		timer.stop();
		return timer.getTime();
	});
	
	evaluator.setCorrelationThreshold(1.2);
	evaluator.evaluate();

	EXPECT_TRUE(evaluator.meetsComplexity(RuntimeEvaluator::TimeComplexity::LINEAR));
}


