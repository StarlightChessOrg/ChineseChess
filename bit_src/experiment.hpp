#pragma once
#include "genMove.hpp"

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
};

