#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


// make public for access to internal data
//#define private public
//#define protected public
// make private = public
//#define CS104TEST
#include "searcheng.h"
#include "combiners.h"
#include "webpage.h"
//#undef private
//#undef protected

// Free up webpages
void cleanup(const std::vector<WebPage*>& pages)
{
    for(auto p : pages) delete p;
}

TEST(Combiners, ANDCombiner)
{
    std::vector<WebPage*> wps;
    for(size_t i=0; i < 10U; i++){
        std::string filename = "file" + to_string(i);
        wps.push_back(new WebPage(filename));
    }

    AndWebPageSetCombiner andCombiner;

    WebPageSet s1 = {wps[0], wps[1]};
    WebPageSet s2 = {wps[2], wps[3]};
    WebPageSet actual1 = andCombiner.combine(s1,s2);
    EXPECT_EQ(0U, actual1.size());

    WebPageSet s3 = {wps[0], wps[1]};
    WebPageSet s4 = {wps[0], wps[2]};
    WebPageSet actual2 = andCombiner.combine(s3, s4);
    EXPECT_EQ(1U, actual2.size());
    EXPECT_TRUE(actual2.find(wps[0]) != actual2.end());

    WebPageSet s5(wps.begin(), wps.end());
    WebPageSet s6(wps.begin(), wps.end());
    WebPageSet actual3 = andCombiner.combine(s5, s6);
    EXPECT_EQ(wps.size(), actual3.size());

    WebPageSet s7;
    WebPageSet s8 = {wps[0]};
    WebPageSet actual4 = andCombiner.combine(s7, s8);
    EXPECT_EQ(0U, actual4.size());

    WebPageSet s9 = {wps[1]};
    WebPageSet s10;
    WebPageSet actual5 = andCombiner.combine(s9, s10);
    EXPECT_EQ(0U, actual5.size());

    cleanup(wps);
}

TEST(Combiners, ORCombiner)
{
    std::vector<WebPage*> wps;
    for(size_t i=0; i < 10U; i++){
        std::string filename = "file" + to_string(i);
        wps.push_back(new WebPage(filename));
    }

    OrWebPageSetCombiner orCombiner;

    WebPageSet s1 = {wps[0], wps[1]};
    WebPageSet s2 = {wps[2], wps[3]};
    WebPageSet actual1 = orCombiner.combine(s1,s2);
    EXPECT_EQ(4U, actual1.size());

    WebPageSet s3 = {wps[0], wps[1]};
    WebPageSet s4 = {wps[0], wps[2]};
    WebPageSet actual2 = orCombiner.combine(s3, s4);
    EXPECT_EQ(3U, actual2.size());
    EXPECT_TRUE(actual2.find(wps[0]) != actual2.end());

    WebPageSet s5(wps.begin(), wps.end());
    WebPageSet s6(wps.begin(), wps.end());
    WebPageSet actual3 = orCombiner.combine(s5, s6);
    EXPECT_EQ(wps.size(), actual3.size());

    WebPageSet s7;
    WebPageSet s8 = {wps[0]};
    WebPageSet actual4 = orCombiner.combine(s7, s8);
    EXPECT_EQ(1U, actual4.size());
    EXPECT_TRUE(actual2.find(wps[0]) != actual2.end());

    WebPageSet s9 = {wps[1]};
    WebPageSet s10;
    WebPageSet actual5 = orCombiner.combine(s9, s10);
    EXPECT_EQ(1U, actual5.size());
    EXPECT_TRUE(actual2.find(wps[1]) != actual2.end());

    cleanup(wps);
}

TEST(Combiners, DiffCombiner)
{
    std::vector<WebPage*> wps;
    for(size_t i=0; i < 10U; i++){
        std::string filename = "file" + to_string(i);
        wps.push_back(new WebPage(filename));
    }

    DiffWebPageSetCombiner diffCombiner;

    WebPageSet s1 = {wps[0], wps[1]};
    WebPageSet s2 = {wps[2], wps[3]};
    WebPageSet actual1 = diffCombiner.combine(s1,s2);
    EXPECT_EQ(2U, actual1.size());
    EXPECT_TRUE(actual1.find(wps[0]) != actual1.end());
    EXPECT_TRUE(actual1.find(wps[1]) != actual1.end());

    WebPageSet s3 = {wps[0], wps[1]};
    WebPageSet s4 = {wps[0], wps[2]};
    WebPageSet actual2 = diffCombiner.combine(s3, s4);
    EXPECT_EQ(1, actual2.size());
    EXPECT_TRUE(actual2.find(wps[1]) != actual2.end());

    WebPageSet s5(wps.begin(), wps.end());
    WebPageSet s6(wps.begin(), wps.end());
    WebPageSet actual3 = diffCombiner.combine(s5, s6);
    EXPECT_EQ(0U, actual3.size());

    WebPageSet s7;
    WebPageSet s8 = {wps[0]};
    WebPageSet actual4 = diffCombiner.combine(s7, s8);
    EXPECT_EQ(0U, actual4.size());

    WebPageSet s9 = {wps[1]};
    WebPageSet s10;
    WebPageSet actual5 = diffCombiner.combine(s9, s10);
    EXPECT_EQ(1U, actual5.size());
    EXPECT_TRUE(actual2.find(wps[1]) != actual2.end());

    cleanup(wps);
}

