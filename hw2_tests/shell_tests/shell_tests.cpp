#include <gtest/gtest.h>
#include <fstream>
#include <cstring>

static const std::string indexFilePath = "./test-small/index.in";
static const std::string dataPath = "./test-small/";
static const std::string queryPath = "./test-small/";

testing::AssertionResult runShell(
        const std::string &idxFilePath, const std::string &queryFilePath, const std::string &outFilePath, const std::string &expFilePath
)
{
    // just print current working dir
    {
        char currDir[1 << 10];
        getcwd(currDir, 1 << 10);
        std::cout << "Current working directory: " << currDir << std::endl;
    }

    // this is from cmake
    const std::string hwDir = HW_DIR;
    std::cout << "Student hw directory: " << hwDir << std::endl;

    // remove the output file before running the command to make sure we don't read the file from the previous execution
    std::remove(outFilePath.c_str());

    const std::string cmd = hwDir + "/search-shell " + idxFilePath + " " + queryFilePath + " " + outFilePath;
    const auto returnCode = std::system(cmd.c_str());

    // EXIT_SUCCESS is usually just defined as 0 in standard library - means process exited normally, not crashed or got killed
    EXPECT_EQ(returnCode, EXIT_SUCCESS);
    if (returnCode != EXIT_SUCCESS)
        std::cout << cmd << " exit status is " << returnCode << " (expected " << EXIT_SUCCESS << ")" << std::endl;

    std::cout << "Attempting to compare " << outFilePath << " to " << queryFilePath << std::endl;
    std::ifstream outFileStream(outFilePath);
    std::ifstream expFileStream(expFilePath);
    if (outFileStream and expFileStream) {
        std::string fileText, expText;
        while (getline(outFileStream, fileText) and getline(expFileStream, expText))
            if (fileText != expText)
                return testing::AssertionFailure() << "output files do not match";

    } else {
        return testing::AssertionFailure() << "Couldn't open output file" << outFilePath;
    }

    return testing::AssertionSuccess();
}

TEST(SearchShell, Query1) {
    std::string qPath = queryPath + "shell-query1.in";
    std::string expPath = queryPath + "shell-query1.exp";
    std::string outFilePath = "./shell-query1.out";
    auto res = runShell(indexFilePath, qPath, outFilePath, expPath);
    EXPECT_TRUE(res);
}

TEST(SearchShell, Query2) {
    std::string qPath = queryPath + "shell-query2.in";
    std::string expPath = queryPath + "shell-query2.exp";
    std::string outFilePath = "./shell-query2.out";
    auto res = runShell(indexFilePath, qPath, outFilePath, expPath);
    EXPECT_TRUE(res);
}

TEST(SearchShell, Query3) {
    std::string qPath = queryPath + "shell-query3.in";
    std::string expPath = queryPath + "shell-query3.exp";
    std::string outFilePath = "./shell-query3.out";
    auto res = runShell(indexFilePath, qPath, outFilePath, expPath);
    EXPECT_TRUE(res);
}

