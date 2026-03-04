#include <gtest/gtest.h>
#include <user_code_runner.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <kwsys/SystemTools.hxx>
#include <sstream>
#include <string>
#include <vector>

#include "solver.h"
#include "heur.h"
#include "board.h"

using namespace std;

//=============================================================================
//   Test helper functions
//=============================================================================

Board buildBoardFromString(const string& boardContents)
{
    stringstream ss(boardContents);
    return Board(ss);
}

struct MovePairSortComp {
    bool operator()(const Board::MovePair& p1, const Board::MovePair& p2)
    {
        return (p1.first < p2.first) || 
               ((p1.first == p2.first) && (p1.second < p2.second));
    }
};
testing::AssertionResult comparePotentialMoves(const string& expectedRes, Board::MovePairList& potentialMoves, size_t expectedSize)
{
    stringstream errMsg1;

    std::sort(potentialMoves.begin(), potentialMoves.end(), MovePairSortComp());

    errMsg1 << "Expected to have a moves list of size " << expectedSize << "actual list size: " << potentialMoves.size() << endl;
    if(expectedSize != potentialMoves.size()){
        return testing::AssertionFailure() << errMsg1.str(); 
    }
    std::ostringstream oss;
    Board::MovePairList::iterator itr;
    for(auto& pair : potentialMoves){
        oss << '(' << pair.first << ',' << pair.second << ')';
    }

    if(oss.str() != expectedRes){
        errMsg1 << "Potential moves incorrect." << endl;
        return testing::AssertionFailure() << errMsg1.str(); 
    }
    return testing::AssertionSuccess();
}

testing::AssertionResult runAndVerifySolution(
   const string& boardLayout, Heuristic* h, bool expHasSol, size_t optimalSolutionSize)
{
    Board b = buildBoardFromString(boardLayout);
    Solver s(b, h);
    bool hasSol = s.run();
    stringstream errMsg1;
    errMsg1 << "\nFor board:\n" << b;

    if(expHasSol != hasSol) {
        return testing::AssertionFailure() << errMsg1.str() << "\n expected hasSol: " << expHasSol << " actual hasSol: " << hasSol;
    }

    if(expHasSol){
        Board::MovePairList sol = s.solution();

        errMsg1 << "\n expected to have a solution of size " << optimalSolutionSize << "actual solution size: " << sol.size() << endl;

        if( optimalSolutionSize != sol.size()){
            return testing::AssertionFailure() << errMsg1.str(); 
        } 

        errMsg1 << "Now attempting to verify your solution: " << endl;
        for( auto movePair : sol )
        {
            errMsg1 << "Attempting to move " << movePair.first 
                << " by " << movePair.second << "...";
            if( ! b.isLegalMove(movePair.first, movePair.second))
            {
                errMsg1 << "NOT LEGAL!" << endl;
                return testing::AssertionFailure() << errMsg1.str();
            }
            b.move(movePair.first, movePair.second);
            errMsg1 << endl;
        }
        if (!b.solved()){
            return testing::AssertionFailure() << errMsg1.str();
        } 
    }
    return testing::AssertionSuccess();
}


//=============================================================================
//   Tests
//=============================================================================
TEST(Heuristic, DirectHeuristic0)
{
    {
        string boardLayout =    
        "...b..\n"
        "...bcc\n"
        "aa....\n"
        "..d..f\n"
        "..d..f\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(0U, h.compute(b));
    }

    {
        string boardLayout =    
        "...b..\n"
        "...bcc\n"
        "gaa...\n"
        "g.d..f\n"
        "..d..f\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(0U, h.compute(b));
    }
}

TEST(Heuristic, DirectHeuristic1)
{
    {
        string boardLayout =    
        ".....c\n"
        "...b.c\n"
        "aa.b..\n"
        "...b.d\n"
        ".....d\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(1U, h.compute(b));
    }

    {
        string boardLayout =    
        "......\n"
        ".....b\n"
        "faa..b\n"
        "f....d\n"
        ".....d\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(1U, h.compute(b));
    }
}

TEST(Heuristic, DirectHeuristic234)
{

    {
        string boardLayout =    
        "......\n"
        "...b..\n"
        "aa.b.d\n"
        "...b.d\n"
        "......\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(2U, h.compute(b));
        
    }

    {
        string boardLayout =    
        "......\n"
        "...bc.\n"
        "aa.bcd\n"
        "..fb.d\n"
        "..f...\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));
        
    }

    {
        string boardLayout =    
        "......\n"
        "...cd.\n"
        "aabcde\n"
        "..bffe\n"
        "......\n"
        "......\n";
        Board b = buildBoardFromString(boardLayout);
        DirectHeuristic h;
        EXPECT_EQ(4U,h.compute(b));
    }

}

TEST(Heuristic, IndirectHeuristic0)
{
    {
        string boardLayout =    
        "...b..\n"
        "...bcc\n"
        "aa....\n"
        "..d..f\n"
        "..d..f\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(0U, h.compute(b));
    }

    {
        string boardLayout =    
        "...b..\n"
        "...bcc\n"
        "gaa...\n"
        "g.d..f\n"
        "..d..f\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(0U, h.compute(b));
    }
}

TEST(Heuristic, IndirectHeuristic1)
{
    {
        string boardLayout =    
        ".....c\n"
        "...b.c\n"
        "aa.b..\n"
        "...b.d\n"
        ".....d\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(1U, h.compute(b));
    }

    {
        string boardLayout =    
        "......\n"
        ".....b\n"
        "faa..b\n"
        "f...d.\n"
        "....d.\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(1U, h.compute(b));
    }
}

TEST(Heuristic, IndirectHeuristic234)
{

    {
        string boardLayout =    
        "......\n"
        "...b..\n"
        "aa.b.d\n"
        "...b.d\n"
        "......\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(2U, h.compute(b));
        
    }

    {
        string boardLayout =    
        "......\n"
        "...bc.\n"
        "aa.bcd\n"
        "..fb.d\n"
        "..f...\n"
        "eee...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));
        
    }

    {
        string boardLayout =    
        "......\n"
        "...cd.\n"
        "aabcde\n"
        "..bffe\n"
        "......\n"
        "......\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(4U,h.compute(b));
    }

}

TEST(Heuristic, IndirectHeuristicFalsePositives)
{
    {
        string boardLayout =    
        "......\n"
        "......\n"
        "aabcf.\n"
        "..bcf.\n"
        "..dee.\n"
        "..d...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));

    }
    {
        string boardLayout =    
        "......\n"
        "..eee.\n"
        "aabcd.\n"
        "..bcd.\n"
        "....d.\n"
        ".fff..\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));

    }

}

TEST(Heuristic, IndirectHeuristicTruePositives)
{
    {
        string boardLayout =    
        "......\n"
        "..bc..\n"
        "aabc..\n"
        "..bc..\n"
        "...eee\n"
        ".dd...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(4U, h.compute(b));

    }
    {
        string boardLayout =    
        "......\n"
        "..bc.f\n"
        "aabc.f\n"
        "..bc.f\n"
        "...eee\n"
        ".dd...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(5U, h.compute(b));

    }
    {
        string boardLayout =    
        "..eegg\n"
        "..b...\n"
        "aabcd.\n"
        "..bcd.\n"
        "....d.\n"
        "..ffhh\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(5U, h.compute(b));

    }
    {
        string boardLayout =    
        "......\n"
        ".ddc..\n"
        "..bc..\n"
        "aabc..\n"
        "..bee.\n"
        "......\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));
    }
    {
        string boardLayout =    
        ".ee...\n"
        "...c..\n"
        "aabc..\n"
        "..bc..\n"
        "..b...\n"
        ".dd...\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));
    }

    {
        string boardLayout =    
        "........\n"
        "..eee...\n"
        "..bc....\n"
        "..bc....\n"
        "aabc....\n"
        "..b.....\n"
        ".fff....\n"
        "........\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));
    }

}

TEST(Heuristic, IndirectHeuristicAboveBelow)
{
    {
        string boardLayout =    
        ".ddee.\n"
        "...c..\n"
        "aabc..\n"
        "..b...\n"
        "..ff..\n"
        "....gg\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(2U,h.compute(b));
    }

    {
        string boardLayout =    
        "......\n"
        "..eee.\n"
        "aabcd.\n"
        "..bcd.\n"
        "..ffgg\n"
        "......\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(3U, h.compute(b));

    }

    {
        string boardLayout =    
        "........\n"
        "..ee....\n"
        "...c....\n"
        "..bc....\n"
        "aabc....\n"
        "..b.....\n"
        "........\n"
        ".fff....\n";
        Board b = buildBoardFromString(boardLayout);
        IndirectHeuristic h;
        EXPECT_EQ(2U, h.compute(b));
    }
}

TEST(OperatorLT, Nominal1)
{
    {
        string boardLayout =    
        "...b\n"
        "aa.b\n"
        "..cc\n"
        "....\n";
        Board b1 = buildBoardFromString(boardLayout);
        Board b2 = buildBoardFromString(boardLayout);
        // two equal boards should be equal (i.e. neither is less-than the other)
        bool b1LTb2 = (b1 < b2);
        bool b2LTb1 = (b2 < b1);
        EXPECT_TRUE( !b1LTb2 && !b2LTb1 );
    }
    {
        string boardLayout =    
        "...ee.\n"
        "...c..\n"
        "aabc..\n"
        "..b...\n"
        "..ff..\n"
        "....gg\n";
        Board b1 = buildBoardFromString(boardLayout);
        Board b2 = buildBoardFromString(boardLayout);
        // two equal boards should be equal (i.e. neither is less-than the other)
        bool b1LTb2 = (b1 < b2);
        bool b2LTb1 = (b2 < b1);
        EXPECT_TRUE( !b1LTb2 && !b2LTb1 );

        b2.move('e', -1);
        b1LTb2 = (b1 < b2);
        b2LTb1 = (b2 < b1);
        EXPECT_TRUE( b1LTb2 || b2LTb1 );
    }
}

TEST(Move, ConstructorAndScore) 
{
    {
        string boardLayout =    
        "...ee.\n"
        "...c..\n"
        "aabc..\n"
        "..b...\n"
        "......\n"
        "....gg\n";
        DirectHeuristic h;
        stringstream ss(boardLayout);
        Board* b = new Board(ss);
        Move move1 = Move(b);
        move1.score(&h);
        EXPECT_EQ(2U, move1.h);
        EXPECT_EQ(0U, move1.g);

        Board::MovePair nextMove = make_pair('b', -2);
        Board* nextboard = new Board(*(move1.b));
        nextboard->move(nextMove.first, nextMove.second);

        Move move2 = Move(nextMove, nextboard, &move1);
        move2.score(&h);
        EXPECT_EQ(1U, move2.h);
        EXPECT_EQ(1U, move2.g);
    }
}


TEST(BoardTest, PotentialMoves1)
{
     string boardLayout =    
    ".bb.\n"
    "aa.c\n"
    "...c\n"
    "dd..\n";
    Board b = buildBoardFromString(boardLayout);
    Board::MovePairList potentialMoves = b.potentialMoves();
    string expectedRes = "(a,1)(b,-1)(b,1)(c,-1)(c,1)(d,1)(d,2)";
    EXPECT_TRUE(comparePotentialMoves(expectedRes, potentialMoves, 7U));
}

TEST(BoardTest, PotentialMoves2)
{
     string boardLayout =    
    "...ee.\n"
    "...c..\n"
    "aabc..\n"
    "..b...\n"
    "..ff..\n"
    "....gg\n";
    Board b = buildBoardFromString(boardLayout);
    Board::MovePairList potentialMoves = b.potentialMoves();
    string expectedRes = "(b,-2)(b,-1)(c,1)(e,-3)(e,-2)(e,-1)(e,1)(f,-2)(f,-1)(f,1)(f,2)(g,-4)(g,-3)(g,-2)(g,-1)";
    EXPECT_TRUE(comparePotentialMoves(expectedRes, potentialMoves, 15U));
}

TEST(BoardTest, NoPotentialMoves)
{
     string boardLayout =    
    "..beee\n"
    "..bc..\n"
    "aabc..\n"
    "..bc..\n"
    "ffffff\n"
    "......\n";
    Board b = buildBoardFromString(boardLayout);
    Board::MovePairList potentialMoves = b.potentialMoves();
    string expectedRes = "";
    EXPECT_TRUE(comparePotentialMoves(expectedRes, potentialMoves, 0U));
}

TEST(BoardTest, Undo1)
{
    string boardLayout =    
    "...ee.\n"
    "...c..\n"
    "aabc..\n"
    "..b...\n"
    "..ff..\n"
    "....gg\n";
    Board b = buildBoardFromString(boardLayout);

    boardLayout =
    "...ee.\n"
    "...c..\n"
    "aabc..\n"
    "..b...\n"
    "ff....\n"
    "....gg\n";
    Board b_after_move1 = buildBoardFromString(boardLayout);

    boardLayout =
    "...ee.\n"
    "...c..\n"
    "aabc..\n"
    "..b...\n"
    "ff....\n"
    ".gg...\n";
    Board b_after_move2 = buildBoardFromString(boardLayout);

    Board startBoard = Board(b);

    Board::MovePair nextMove = make_pair('f', -2);
    startBoard.move(nextMove.first, nextMove.second);

    EXPECT_TRUE(!(startBoard < b_after_move1) && !(b_after_move1 < startBoard));

    nextMove = make_pair('g', -3);
    startBoard.move(nextMove.first, nextMove.second);

    EXPECT_TRUE(!(startBoard < b_after_move2) && !(b_after_move2 < startBoard));

    startBoard.undoLastMove();

    EXPECT_TRUE(!(startBoard < b_after_move1) && !(b_after_move1 < startBoard));

    startBoard.undoLastMove();

    EXPECT_TRUE(!(startBoard < b) && !(b < startBoard));

    EXPECT_THROW(startBoard.undoLastMove(); ,std::underflow_error);
}


TEST(Solver, Basic)
{
    {
        string boardLayout =    
        "...b..\n"
        "...b..\n"
        "aa.b.d\n"
        "..cccd\n"
        "......\n"
        "......\n";
        IndirectHeuristic h;
        size_t optimalSolutionSize = 3U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
    }
    {
        string boardLayout =    
        "...b..\n"
        "...b..\n"
        "aa.c.d\n"
        "...c.d\n"
        "......\n"
        "......\n";
        IndirectHeuristic h;
        size_t optimalSolutionSize = 2U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
    }
    {
        string boardLayout =    
        "...b..\n"
        "...b..\n"
        "aa....\n"
        ".....d\n"
        "......\n"
        "......\n";
        IndirectHeuristic h;
        size_t optimalSolutionSize = 0U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
    }

    {
        string boardLayout =    
        "......\n"
        ".bb...\n"
        "aac...\n"
        "..c..e\n"
        "..dd.e\n"
        "......\n";
        IndirectHeuristic h;
        size_t optimalSolutionSize = 2U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
    }

}

TEST(Solver, Difficult)
{
    {
        string boardLayout = 
            "......\n"
            "......\n"
            ".aab..\n"
            ".ccb.d\n"
            ".e.b.d\n"
            ".eff.d\n";
        IndirectHeuristic h;
        size_t optimalSolutionSize = 13U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
    }
}


TEST(Solver, InBoard10x10)
{
    string boardLayout =    
    "..........\n"
    "ffbbmm....\n"
    "..cg.e....\n"
    "..cg.e....\n"
    ".jjjj.....\n"
    ".hhh.nnnnn\n"
    "...k......\n"
    "aa.k......\n"
    "...k......\n"
    "...k......\n";
    IndirectHeuristic h;
    size_t optimalSolutionSize = 6U;
    EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
}

TEST(Solver, Direct)
{
    {
        string boardLayout =    
        "..bb..\n"
        "..c...\n"
        "aac...\n"
        ".dd..e\n"
        ".....e\n"
        "......\n";
        DirectHeuristic h;
        size_t optimalSolutionSize = 2U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));

    }
    {
        
        string boardLayout =    
        "bbc..d\n"
        "efc.gd\n"
        "efaagd\n"
        "hiijkk\n"
        "hm.j..\n"
        ".m.nnn\n";

        DirectHeuristic h;
        size_t optimalSolutionSize = 18U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, true, optimalSolutionSize));
    }

}

TEST(Solver, UnSolvable1)
{
    {
        string boardLayout =    
        "...b..\n"
        "...b..\n"
        "aa.c..\n"
        "...c.d\n"
        "...c..\n"
        "eeeee.\n";
        BFSHeuristic h;
        size_t optimalSolutionSize = 0U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, false, optimalSolutionSize));
    }
    {
        string boardLayout =    
        "...b..\n"
        "...b..\n"
        "aa.b..\n"
        "...b.d\n"
        "......\n"
        "eee...\n";
        BFSHeuristic h;
        size_t optimalSolutionSize = 0U;
        EXPECT_TRUE(runAndVerifySolution(boardLayout, &h, false, optimalSolutionSize));

    }
}
