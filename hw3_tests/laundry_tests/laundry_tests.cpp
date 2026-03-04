#include <gtest/gtest.h>
#include <user_code_runner.h>
#include <string>
#include <iostream>
#include <fstream>
#include <kwsys/SystemTools.hxx>
#include <algorithm>
using namespace std;

string trimTrailingWhitespace(string in)
{
	size_t endpos = in.find_last_not_of(" \t\n");
	size_t startpos = in.find_first_not_of(" \t\n");
	string out;
	
	if(string::npos != endpos)
	{
		out = in.substr(0, endpos+1);
		out = out.substr(startpos);
		return out;
	}
	else
	{
		return in;
	}
}

testing::AssertionResult checkLaundry(string const & testName, string const & inputContents, string const & outputContents, bool useValgrind = true)
{
	string executablePath = LAUNDRY_EXECUTABLE;
	string testFolder = TEST_BINARY_DIR  "/laundry_tests/testFiles/" + testName;
	kwsys::SystemTools::MakeDirectory(testFolder);
	string inputFilePath = testFolder + "/input.txt";
	string outputFilePath = testFolder + "/output.txt";
	string stdoutFilePath = testFolder + "/stdout.txt";

	ofstream inputWriter(inputFilePath);
	inputWriter << inputContents << endl;
	
	UserCodeRunner runner(testFolder, executablePath, {inputFilePath, outputFilePath}, stdoutFilePath, useValgrind);
	testing::AssertionResult result = runner.execute();
	
	{
		ifstream outputFile(outputFilePath);
		if(!outputFile)
		{	
			return testing::AssertionFailure() << "Couldn't open output file " << outputFilePath; 
		}
		string outputFromFile {istreambuf_iterator<char>(outputFile), istreambuf_iterator<char>()};
		string output = trimTrailingWhitespace(outputFromFile);
		if (output != outputContents)
		{
			//Try removing all spaces
			output.erase(remove(output.begin(), output.end(), ' '), output.end());	
			string temp(outputContents);
			temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());	
			if(output != temp)
			{
				return testing::AssertionFailure() << "Contents do not match" << endl << "got " << endl << output << endl << "expected" << endl << outputContents << endl;
			}
		}
	}
	return testing::AssertionSuccess();
}

TEST(laundry_tests, NominalInput1)
{
	const string input = "0 0 0 3";
	const string output = "black black black";
	ASSERT_TRUE(checkLaundry("NominalInput1", input, output, true));
}

TEST(laundry_tests, NominalInput2)
{
	const string input = "0 -1 2";
	const string output = "gray black";
	ASSERT_TRUE(checkLaundry("NominalInput2", input, output, true));
}

TEST(laundry_tests, NominalInput3)
{
	const string input = "0 -1 -1 3";
	const string output = "gray gray black";
	ASSERT_TRUE(checkLaundry("NominalInput3", input, output, true));
}

TEST(laundry_tests, TwoRemovals)
{
	const string input = "0 -1 -1 3 0 -1 -1 3";
	const string output = "gray gray black \ngray gray black";
	ASSERT_TRUE(checkLaundry("TwoRemovals", input, output, true));
}

TEST(laundry_tests, ThreeRemovals)
{
	const string input = "0 -1 -1 1 1 1";
	const string output = "gray \ngray \nblack";
	ASSERT_TRUE(checkLaundry("ThreeRemovals", input, output, true));
}

TEST(laundry_tests, OverRemoval1)
{
	const string input = "0 -1 4";
	const string output = "gray black";
	ASSERT_TRUE(checkLaundry("overRemoval1", input, output, true));
}

TEST(laundry_tests, OverRemoval2)
{
	const string input = "0 -1 4 0 -1 4";
	const string output = "gray black \ngray black";
	ASSERT_TRUE(checkLaundry("overRemoval2", input, output, true));
}

TEST(laundry_tests, NoRemoval)
{
	const string input = "0 -1 0 -1 0 -1";
	const string output = "";
	ASSERT_TRUE(checkLaundry("NoRemoval", input, output, true));
}

