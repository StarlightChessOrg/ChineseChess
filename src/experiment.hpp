#pragma once
#include <iostream>
#include <ctime>
#include <random>
#include <cstdlib>
#include "genMove.hpp"
using namespace std;

struct test{
    static void testGenMove(){
        position p;
        p.side = black;
        vector<step> moveList;
        genMove::genMoveList(p,moveList,all);
        step::printMoveList(moveList);
    }
};

