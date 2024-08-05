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
    killerCache(){

    }
protected:
    void clearCache(){
        for(vector<step>& moveList : killerMoveList){
            vector<step>().swap(moveList);
        }
    }
private:
    vector<step> killerMoveList[MAX_KILLER_MOVE_NUM];
};
