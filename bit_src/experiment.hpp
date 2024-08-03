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

    static void testGenPawnMove(int side){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genPawnMove(p, moveList);
        step::printMoveList(moveList);
    }

    static void testGenAdvisorMove(int side){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genAdvisorMove(p, moveList);
        step::printMoveList(moveList);
    }

    static void testGenKingMove(int side){
        position p = position(initGameBoard);
        p.side = side;
        vector<step> moveList;
        genMove::genKingMove(p, moveList);
        step::printMoveList(moveList);
    }
};

