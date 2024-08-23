#pragma once
#include <iostream>
#include <ctime>
#include <random>
#include <cstdlib>
#include "status.hpp"
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
};

