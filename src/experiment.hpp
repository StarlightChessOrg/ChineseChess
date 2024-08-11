#pragma once
#include "search.hpp"
#include "ctime"
//#include "windows.h"

class test{
public:
    static void testPosition(){
        position p = position(initGameBoard);
        p.makeMove(84,196);
        p.board.printBasicBoard();
        p.swapBoard.printSwapBoard();
        p.bitBoard.printBitBoard();
        p.unMakeMove(84,196,-10,6);
        p.board.printBasicBoard();
        p.swapBoard.printSwapBoard();
        p.bitBoard.printBitBoard();
        std::cout<<std::endl;
    }

    static void testGenPawnMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genPawnMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenAdvisorMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genAdvisorMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenKingMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genKingMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenBishopMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genBishopMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenKnightMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genKnightMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenRookMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genRookMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenCannonMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genCannonMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenMoveList(int side,int genType = all){
        evaluate e = evaluate(initGameBoard,side);
        std::vector<step> moveList;
        genMove::genMoveList(e,moveList,genType);
        //step::printMoveList(moveList);

        for(step& move : moveList){
            if(e.makeMove(move.fromPos,move.toPos)){
                move.printMove();
                e.unMakeMove();
            }
        }
    }

    static void testLegalMove(int side){
        position p = position(initGameBoard);
        p.side = side;
        std::vector<step> moveList;
        genMove::genMoveList(p,moveList);
        for(step& s : moveList){
            s.printMove();
            assert(genMove::legalMove(p,s));
        }
    }

    static void testGetRelation(){
        position p = position(initGameBoard);
        std::cout<<genMove::getRelation(p,52,beThreatened)<<std::endl;
        std::cout<<genMove::getRelation(p,52,beProtected)<<std::endl;
        std::cout<<genMove::getRelation(p,54,beThreatened)<<std::endl;
        std::cout<<genMove::getRelation(p,54,beProtected)<<std::endl;
        std::cout<<genMove::getRelation(p,51,beThreatened)<<std::endl;
        std::cout<<genMove::getRelation(p,51,beProtected)<<std::endl;
    }

    static void testEvaluateMakeMove(){
        evaluate e = evaluate(initGameBoard);
        e.resetEvaBoard();
        std::cout<<e.vlRed<<" "<<e.vlBlack<<std::endl;
        e.makeMove(84,196);
        std::cout<<e.vlRed<<" "<<e.vlBlack<<std::endl;
        e.unMakeMove();
        std::cout<<e.vlRed<<" "<<e.vlBlack<<std::endl;
        e.board.printBasicBoard();
    }

    static void testEvaluate(){
        evaluate e = evaluate(initGameBoard);
        e.board.printBasicBoard();
        e.resetEvaBoard();
        int vl = e.rookMobility(red);
        std::cout<<vl<<std::endl;
        vl = e.knightTrap(red);
        std::cout<<vl<<std::endl;
        std::cout<<"-----------------------"<<std::endl;
        vl = e.stringHold(red);
        std::cout<<vl<<std::endl;
        vl = e.stringHold(black);
        std::cout<<vl<<std::endl;
        std::cout<<"-----------------------"<<std::endl;
        vl = e.advisorShape(red);
        std::cout<<vl<<std::endl;
        vl = e.advisorShape(black);
        std::cout<<vl<<std::endl;
    }

    static void testCheckedBy(){
        evaluate e = evaluate(initGameBoard);
        e.board.printBasicBoard();
        e.resetEvaBoard();
        std::cout<<genMove::CheckedBy(e,red)<<std::endl;
        std::cout<<genMove::CheckedBy(e,black)<<std::endl;
    }

    static void testChasedBy(){
        evaluate e = evaluate(initGameBoard);
        e.board.printBasicBoard();
        e.resetEvaBoard();
        step move = step(51,51 + 32 + 1,-6,0);
        std::cout<<genMove::ChasedBy(e,move);
    }

    static void testCheckChaseMakeMove(){
        evaluate e = evaluate(initGameBoard);
        e.resetEvaBoard();
        e.board.printBasicBoard();
        e.makeMove(164,167);
        e.makeMove(84,87);
        e.makeMove(167,103);
        e.makeMove(54,71);
        e.makeMove(196,165);
        e.board.printBasicBoard();
    }

    static void testSearch(){
        evaluate e = evaluate(initGameBoard,red);
        searchGroup s = searchGroup();
        time_t start = clock();
        s.searchMain(e,10,3000);
        time_t end = clock();
        std::cout<<(double)(end - start) / CLOCKS_PER_SEC<<std::endl;
    }
};

