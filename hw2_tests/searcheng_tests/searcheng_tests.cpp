#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <txt_parser.h>

#include "misc_utils.h"

using namespace std;


// make public for access to internal data
//#define private public
//#define protected public
// make private = public
//#define CS104TEST
#include "webpage.h"
#include "searcheng.h"
#include "combiners.h"
#include "md_parser.h"
//#undef private
//#undef protected


StringSet pageSetToFilenames(const WebPageSet &s)
{
    StringSet ss;
    for (auto wp : s)
        ss.insert(wp->filename());
    return ss;
}


std::unique_ptr<SearchEng> searchEngFactory()
{
    auto seng = std::unique_ptr<SearchEng>(new SearchEng);
    seng->register_parser("md", new MDParser);
    seng->register_parser("txt", new TXTParser);
    return seng;
}


void writeSearchEngFile(const std::string &filename, const std::string &content)
{
    ofstream ofile(filename.c_str());
    ofile << content;
    ofile.close();
}


void writeSearchEngFile(const std::string& filename, size_t numWords)
{
    ostringstream ss;
    for (size_t i = 0; i < numWords; ++i)
        ss << "word" << i << endl;
    writeSearchEngFile(filename, ss.str());
}


unique_ptr<SearchEng> searchEngTestSetup()
{
    auto seng = searchEngFactory();

    std::vector<std::string> filenames = {"file1.md", "file2.md", "file3.md", "file4.md"};
    std::vector<size_t> numWords = {1, 2, 3, 4};

    for(size_t i = 0U; i < filenames.size(); ++i) {
        writeSearchEngFile(filenames[i], numWords[i]);
        seng->read_page(filenames[i]);
    }

    return seng;
}


TEST(SearchEng, ParserRegistration)
{
    SearchEng seng;

    std::remove("test_parser.md");  // remove file if it exists (i.e. from previous test invocation)

    // parser not registered yet: throw logic error
    EXPECT_THROW(seng.read_page("test_parser.md"), std::logic_error);

    // file extension is register but file does not exist
    seng.register_parser("md", new MDParser);
    EXPECT_THROW(seng.read_page("test_parser.md"), std::invalid_argument);

    writeSearchEngFile("test_parser.md", "test");
    EXPECT_NO_THROW(seng.read_page("test_parser.md"));

    // make sure we can use other parsers beyond just .txt and .md
    writeSearchEngFile("test_parser.html", "<HTML>\n<HEAD>\n</HEAD>\n</HTML>");
    EXPECT_THROW(seng.read_page("test_parser.html"), std::logic_error);

    class FakeHtmlParser : public PageParser {
        void parse(std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks) override {}
        std::string display_text(std::istream& filename) override { return {}; }
    };

    auto fakeHtmlParser = new FakeHtmlParser;
    seng.register_parser("html", fakeHtmlParser);

    EXPECT_NO_THROW(seng.read_page("test_parser.html"));
}


TEST(SearchEng, ReadPageMultipleTimes)
{
    auto seng = searchEngFactory();
    AndWebPageSetCombiner andSearch;

    const auto pageName = "test_multiple_reads.md";
    writeSearchEngFile(pageName, 3);
    seng->read_page(pageName);

    const auto page = seng->retrieve_page(pageName);
    const auto terms = page->all_terms();

    EXPECT_EQ(setToString(terms), "[word0, word1, word2]");

    WebPageSet ws = seng->search({"word1", "word2"}, &andSearch);

    seng->read_page(pageName);
    const auto page2 = seng->retrieve_page(pageName);
    const auto terms2 = page2->all_terms();
    EXPECT_TRUE(areSetsEqual(terms, terms2));

    WebPageSet ws2 = seng->search({"word1", "word2"}, &andSearch);
    EXPECT_TRUE(areSetsEqual(ws, ws2));
}


TEST(SearchEng, ANDCombiner)
{
    auto seng = searchEngTestSetup();

    AndWebPageSetCombiner andSearch;

    {
        std::vector<std::string> terms = {"word0", "word1"};
        WebPageSet ws = seng->search(terms, &andSearch);
        EXPECT_EQ(3U, ws.size());
        set<string> expectedFilenames = {"file2.md", "file3.md", "file4.md"}, actualFilenames = pageSetToFilenames(ws);
        EXPECT_TRUE(areSetsEqual(expectedFilenames, actualFilenames));

        EXPECT_EQ(actualFilenames.count("file1.md"), 0);
        EXPECT_EQ(actualFilenames.count("file2.md"), 1);
    }

    {
        std::vector<std::string> terms = {"word1", "word0"};
        WebPageSet ws = seng->search(terms, &andSearch);
        EXPECT_EQ(3U, ws.size());
        set<string> expectedFilenames = {"file2.md", "file3.md", "file4.md"}, actualFilenames = pageSetToFilenames(ws);
        EXPECT_TRUE(areSetsEqual(expectedFilenames, actualFilenames));

        EXPECT_EQ(actualFilenames.count("file1.md"), 0);
        EXPECT_EQ(actualFilenames.count("file2.md"), 1);
    }
}

TEST(SearchEng, ORCombiner)
{
    auto seng = searchEngTestSetup();

    writeSearchEngFile("extra_page.md", "uniqueword1\nuniqueword2");
    seng->read_page("extra_page.md");

    OrWebPageSetCombiner orSearch;

    {
        std::vector<std::string> terms = {"word0", "word1"};
        WebPageSet ws = seng->search(terms, &orSearch);

        EXPECT_EQ(4U, ws.size());
        set<string> expectedFilenames = {"file1.md", "file2.md", "file3.md", "file4.md"},
                    actualFilenames = pageSetToFilenames(ws);
        EXPECT_TRUE(areSetsEqual(expectedFilenames, actualFilenames));
    }

    {
        std::vector<std::string> terms = {"word2", "word3", "uniqueword1"};
        WebPageSet ws = seng->search(terms, &orSearch);

        EXPECT_EQ(3U, ws.size());
        set<string> expectedFilenames = {"file3.md", "file4.md", "extra_page.md"},
                    actualFilenames = pageSetToFilenames(ws);
        EXPECT_TRUE(areSetsEqual(expectedFilenames, actualFilenames));
    }
}

TEST(SearchEng, DiffCombiner)
{
    auto seng = searchEngTestSetup();

    writeSearchEngFile("extra_page.md", "uniqueword1\nuniqueword2");
    seng->read_page("extra_page.md");

    DiffWebPageSetCombiner diffSearch;

    {
        std::vector<std::string> terms = {"word0", "word2", "word1"};
        WebPageSet ws = seng->search(terms, &diffSearch);

        EXPECT_EQ(1U, ws.size());
        set<string> expectedFilenames = {"file1.md"}, actualFilenames = pageSetToFilenames(ws);
        EXPECT_TRUE(areSetsEqual(expectedFilenames, actualFilenames));
    }

    {
        std::vector<std::string> terms = {"word0", "word1", "word2"};
        WebPageSet ws = seng->search(terms, &diffSearch);

        EXPECT_EQ(1U, ws.size());
        set<string> expectedFilenames = {"file1.md"}, actualFilenames = pageSetToFilenames(ws);
        EXPECT_TRUE(areSetsEqual(expectedFilenames, actualFilenames));
    }
}

TEST(SearchEng, RetrievePage)
{
    auto seng = searchEngFactory();
    seng->read_pages_from_index("test-small/index.in");

    // EXPECTED OUTGOING LINKS STRUCTURE IN TEST DATA
    // pga.md -> pgg.md pgc.md
    // pgb.md -> pga.md pge.md pgf.md
    // pgc.md -> pgb.md
    // pgd.md -> pgc.md
    // pge.md -> pgc.md pgd.md pgf.md
    // pgf.md ->
    // pgg.md ->

    std::map<std::string, StringSet> expectedOutgoing {
        {"pga.md", {"pgg.md", "pgc.md"}},
        { "pgb.md", {"pga.md", "pge.md", "pgf.md"}},
        { "pgc.md", {"pgb.md"}},
        { "pgd.md", {"pgc.md"}},
        { "pge.md", {"pgc.md", "pgd.md", "pgf.md"}},
        { "pgf.md", {}},
        { "pgg.md", {}},
    };

    // EXPECTED INCOMING LINKS STRUCTURE IN TEST DATA
    // pga.md <- pgb.md
    // pgb.md <- pgc.md
    // pgc.md <- pga.md pgd.md pge.md
    // pgd.md <- pge.md
    // pge.md <- pgb.md
    // pgf.md <- pgb.md pge.md
    // pgg.md <- pga.md

    std::map<std::string, StringSet> expectedIncoming {
            {"pga.md", {"pgb.md"}},
            { "pgb.md", {"pgc.md"}},
            { "pgc.md", {"pga.md", "pgd.md", "pge.md"}},
            { "pgd.md", {"pge.md"}},
            { "pge.md", {"pgb.md"}},
            { "pgf.md", {"pgb.md", "pge.md"}},
            { "pgg.md", {"pga.md"}},
    };

    // just adds "test-small/" to all filenames in the set to avoid typing it everywhere
    const auto longFilenames = [](const StringSet &s) {
        StringSet ret;
        for (const auto& f : s)
            ret.insert("test-small/" + f);
        return ret;
    };

    for (const auto &it : expectedOutgoing) {
        const std::string pageName = "test-small/" + it.first;
        std::cout << "Checking links for page " << pageName << std::endl;
        const StringSet expectedOutgoingLinks = longFilenames(it.second);
        const StringSet expectedIncomingLinks = longFilenames(expectedIncoming[it.first]);

        const WebPage *page = seng->retrieve_page(pageName);
        EXPECT_EQ(pageName, page->filename());

        const WebPageSet outgoingLinks = page->outgoing_links();
        const StringSet outgoingLinkFilenames = pageSetToFilenames(outgoingLinks);
        std::cout << "Expect outgoing outgoingLinks: " << setToString(expectedOutgoingLinks)
                  << " actual " << setToString(outgoingLinkFilenames) << std::endl;
        EXPECT_TRUE(areSetsEqual(outgoingLinkFilenames, expectedOutgoingLinks));

        const WebPageSet incomingLinks = page->incoming_links();
        const StringSet incomingLinkFilenames = pageSetToFilenames(incomingLinks);
        std::cout << "Expect incoming outgoingLinks: " << setToString(expectedIncomingLinks)
                  << " actual " << setToString(incomingLinkFilenames) << std::endl;
        EXPECT_TRUE(areSetsEqual(incomingLinkFilenames, expectedIncomingLinks));
    }

    const auto txtPageName = "test-small/pgi.txt";
    WebPage *txtPage = seng->retrieve_page(txtPageName);
    EXPECT_EQ(txtPage, nullptr);  // page not in the index

    seng->read_page(txtPageName);
    txtPage = seng->retrieve_page(txtPageName);
    EXPECT_NE(txtPage, nullptr);
    EXPECT_EQ(txtPage->filename(), txtPageName);

    // should have no links
    EXPECT_TRUE(areSetsEqual(txtPage->incoming_links(), {}));
    EXPECT_TRUE(areSetsEqual(txtPage->outgoing_links(), {}));
}

TEST(SearchEng, LinkCornerCases)
{
    auto seng = searchEngFactory();
    const std::string refSelfPage = "reference_self.md";
    writeSearchEngFile(refSelfPage, "[samepage](reference_self.md)");
    seng->read_page(refSelfPage);
    WebPage *page = seng->retrieve_page(refSelfPage);
    EXPECT_NE(page, nullptr);
    EXPECT_EQ(page->filename(), refSelfPage);

    EXPECT_TRUE(areSetsEqual(page->incoming_links(), {page}));
    EXPECT_TRUE(areSetsEqual(page->incoming_links(), {page}));
}

TEST(SearchEng, SameLink)
{
    auto seng = searchEngFactory();
    writeSearchEngFile("somepage.md", "just some content");
    const std::string sameLinkPage = "samelink.md";

    std::ostringstream ss;
    for (int i = 0; i < 10000; ++i)
        ss << "[page](somepage.md)\n";

    writeSearchEngFile(sameLinkPage, ss.str());
    seng->read_page(sameLinkPage);

    WebPage *somePagePtr = seng->retrieve_page("somepage.md");
    WebPage *sameLinkPagePtr = seng->retrieve_page(sameLinkPage);

    EXPECT_TRUE(areSetsEqual(somePagePtr->incoming_links(), {sameLinkPagePtr}));
    EXPECT_TRUE(areSetsEqual(somePagePtr->outgoing_links(), {}));

    EXPECT_TRUE(areSetsEqual(sameLinkPagePtr->incoming_links(), {}));
    EXPECT_TRUE(areSetsEqual(sameLinkPagePtr->outgoing_links(), {somePagePtr}));
}

TEST(SearchEng, DisplayPage)
{
    auto seng = searchEngFactory();
    std::string pageWithLinksName = "page_with_links.md";
    writeSearchEngFile(pageWithLinksName, "[l1](https://usc.com) [l2](some/URL)");
    seng->read_page(pageWithLinksName);

    std::ostringstream ss;
    seng->display_page(ss, pageWithLinksName);
    std::string actualOutput = ss.str();
    std::string expectedOutput = "[l1] [l2]";

    EXPECT_EQ(actualOutput, expectedOutput);
}