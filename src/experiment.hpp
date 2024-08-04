#pragma once
#include "search.hpp"

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
        cout<<endl;
    }

    static void testGenPawnMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genPawnMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenAdvisorMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genAdvisorMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenKingMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genKingMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenBishopMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genBishopMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenKnightMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genKnightMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenRookMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genRookMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenCannonMove(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genCannonMove(p, moveList,genType);
        step::printMoveList(moveList);
    }

    static void testGenMoveList(int side,int genType = all){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genMoveList(p,moveList,genType);
        step::printMoveList(moveList);
    }

    static void testLegalMove(int side){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genMoveList(p,moveList);
        for(step& s : moveList){
            s.printMove();
            assert(genMove::legalMove(p,s));
        }
    }

    static void testGetRelation(){
        position p = position(initGameBoard);
        cout<<genMove::getRelation(p,52,beThreatened)<<endl;
        cout<<genMove::getRelation(p,52,beProtected)<<endl;
        cout<<genMove::getRelation(p,54,beThreatened)<<endl;
        cout<<genMove::getRelation(p,54,beProtected)<<endl;
        cout<<genMove::getRelation(p,51,beThreatened)<<endl;
        cout<<genMove::getRelation(p,51,beProtected)<<endl;
    }

    static void testEvaluateMakeMove(){
        evaluate e = evaluate(initGameBoard);
        e.resetEvaBoard();
        cout<<e.vlRed<<" "<<e.vlBlack<<endl;
        e.makeMove(84,196);
        cout<<e.vlRed<<" "<<e.vlBlack<<endl;
        e.unMakeMove();
        cout<<e.vlRed<<" "<<e.vlBlack<<endl;
        e.board.printBasicBoard();
    }

    static void testEvaluate(){
        evaluate e = evaluate(initGameBoard);
        e.board.printBasicBoard();
        e.resetEvaBoard();
        int vl = e.rookMobility(red);
        cout<<vl<<endl;
        vl = e.knightTrap(red);
        cout<<vl<<endl;
        cout<<"-----------------------"<<endl;
        vl = e.stringHold(red);
        cout<<vl<<endl;
        vl = e.stringHold(black);
        cout<<vl<<endl;
        cout<<"-----------------------"<<endl;
        vl = e.advisorShape(red);
        cout<<vl<<endl;
        vl = e.advisorShape(black);
        cout<<vl<<endl;
    }

    static void testCheckedBy(){
        evaluate e = evaluate(initGameBoard);
        e.board.printBasicBoard();
        e.resetEvaBoard();
        cout<<genMove::CheckedBy(e,red);
    }

    static void testChasedBy(){
        evaluate e = evaluate(initGameBoard);
        e.board.printBasicBoard();
        e.resetEvaBoard();
        step move = step(51,51 + 32 + 1,-6,0);
        cout<<genMove::ChasedBy(e,move);
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
};

