#pragma once
#include <iostream>
#include <ctime>
#include <random>
#include <cstdlib>
#include "search.hpp"
using namespace std;

struct test{
    static void testGenMove(){
        position p;
        p.side = black;
        vector<step> moveList;
        genMove::genMoveList(p,moveList,all);
        step::printMoveList(moveList);
    }

    static void testStatus(){
        evaluate e = evaluate(initGameBoard,red);
        cout<<e.vlSimpleAttackRed<<" "<<e.vlSimpleDefenseRed<<endl;
        cout<<e.vlSimpleAttackBlack<<" "<<e.vlSimpleDefenseBlack<<endl;
        e.makeMove(164,52);
        cout<<e.vlSimpleAttackRed<<" "<<e.vlSimpleDefenseRed<<endl;
        cout<<e.vlSimpleAttackBlack<<" "<<e.vlSimpleDefenseBlack<<endl;
        e.unMakeMove();
        cout<<e.vlSimpleAttackRed<<" "<<e.vlSimpleDefenseRed<<endl;
        cout<<e.vlSimpleAttackBlack<<" "<<e.vlSimpleDefenseBlack<<endl;
    }

    static void testNNUE(){
        nnue* pN = new nnue;
        pN->readPara(R"(E:..\linear\para)");
        evaluate e = evaluate(initGameBoard,red);
        e.makeMove(164,167);
        e.board.printBasicBoard();
        pN->initCache(e);
        int vl = pN->forward(e);
        cout<<vl<<endl;
    }

    static void testSearch(){
        searchManager* pM = new searchManager(initGameBoard,red);
        step s = step(196,165,6,0);
        pM->s.makeMove(s);
        s = step(58,89,-7,0);
        pM->s.makeMove(s);
        s = step(170,138,11,0);
        pM->s.makeMove(s);
        pM->searchMain(16,100000000);
    }
};

