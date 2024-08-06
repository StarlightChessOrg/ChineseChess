#pragma once
#include "genMove.hpp"

class historyCache{
public:
    historyCache(){
        clearCache();
    }
protected:
    void clearCache(){
        memset(history,0,sizeof(int) * 256 * 256);
    }
    void recoardCache(step& move,int depth){
        history[move.fromPos][move.toPos] += (1 << depth);
    }
    int getCache(step& move){
        return history[move.fromPos][move.toPos];
    }
private:
    int history[256][256]{};
    friend class searchGroup;
    friend class moveSort;
};

static const int MAX_KILLER_MOVE_NUM = 64;

class killerCache{
public:
    killerCache()= default;
    void getCache(evaluate& e,vector<step>& moveList){
        for(step& move : killerMoveList[e.getNowDistance()]){
            if(genMove::legalMove(e,move)){
                moveList.push_back(move);
            }
        }
    }
    void recoardCache(evaluate& e,step& move){
        const int nowPosDistance = e.getNowDistance();
        killerMoveList[nowPosDistance][1] = killerMoveList[nowPosDistance][0];
        killerMoveList[nowPosDistance][0] = move;
    }
protected:
    void clearCache(){
        for(auto& killer : killerMoveList){
            killer[0] = step(0,0,0,0);
            killer[1] = step(0,0,0,0);
        }
    }
private:
    step killerMoveList[MAX_KILLER_MOVE_NUM][2];
    friend class searchGroup;
};
