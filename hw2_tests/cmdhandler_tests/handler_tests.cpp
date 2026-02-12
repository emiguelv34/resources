#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "cmdhandler.h"
#include "handler.h"
#include "webpage.h"
#include "searcheng.h"
#include "md_parser.h"
#include "txt_parser.h"


SearchEng* searchEngFactory(){
    SearchEng* seng = new SearchEng();
    seng->register_parser("md", new MDParser);
    seng->register_parser("txt", new TXTParser);
    return seng;
}

TEST(Cmdhandlers, AndHandler){
    SearchEng* seng = new SearchEng();
    AndHandler ah;
    std::istringstream iss("a b c");
    EXPECT_EQ(ah.handle(seng, "OR", iss, std::cout), 2);
    EXPECT_EQ(ah.handle(seng, "AND", iss, std::cout), 0);
    delete seng;
}

TEST(Cmdhandlers, OrHandler) {
    SearchEng* seng = new SearchEng();
    OrHandler oh;
    std::istringstream iss("a b c");
    EXPECT_EQ(oh.handle(seng, "OR", iss, std::cout), 0);
    EXPECT_EQ(oh.handle(seng, "AND", iss, std::cout), 2);
    delete seng;
}

TEST(Cmdhandlers, DiffHandler){
    SearchEng* seng = new SearchEng();
    DiffHandler dh;
    std::istringstream iss("a b c");
    EXPECT_EQ(dh.handle(seng, "DIFF", iss, std::cout), 0);
    EXPECT_EQ(dh.handle(seng, "abcxyz", iss, std::cout), 2);
    delete seng;
}

TEST(Cmdhandlers, IncomingHandler) {
    auto seng = searchEngFactory();
    seng->read_pages_from_index("test-small/index.in");
    IncomingHandler ih;
    std::istringstream iss("test-small/pga.md");
    std::istringstream invalid("blah");

    EXPECT_EQ(ih.handle(seng, "INCOMING", iss, std::cout), 0);
    EXPECT_EQ(ih.handle(seng, "INCOMING", invalid, std::cout), 2);
    EXPECT_EQ(ih.handle(seng, "OUTGOING", iss, std::cout), 2);
    delete seng;
}

TEST(Cmdhandlers, OutgoingHandler) {
    auto seng = searchEngFactory();
    seng->read_pages_from_index("test-small/index.in");
    OutgoingHandler oh;
    std::istringstream  iss("test-small/pgd.md");
    std::istringstream invalid("test-small/error404.txt");

    EXPECT_EQ(oh.handle(seng, "OUTGOING", iss, std::cout), 0);
    EXPECT_EQ(oh.handle(seng, "INCOMING", iss, std::cout), 2);
    EXPECT_EQ(oh.handle(seng, "OUTGOING", invalid, std::cout), 2);
    delete seng; 
}
