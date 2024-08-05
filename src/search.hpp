#pragma once
#include "evaluate.hpp"

static const int MAX_MOVE_NUM = 1024;
static const int MIN_VALUE = -32768;
static const int MAX_VALUE = 32768;
static const int MIN_BAN_VALUE = MIN_VALUE + MAX_MOVE_NUM;
static const int MAX_BAN_VALUE = MAX_VALUE - MAX_MOVE_NUM;

static const int SORT_MAX_VALUE = 65536;

class moveSort{
public:
    static void sortRootMoveSequance(vector<step>& moveList,step& betterMove){
        for(step& move : moveList){
            if(move == betterMove){
                move.vl = SORT_MAX_VALUE;
            }else if(move.vl){
                move.vl--;
            }
        }
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
private:
    static bool vlCompare(const step& firstMove,const step& secondMove){
        return firstMove.vl > secondMove.vl;
    }
    friend class searchGroup;
};

class searchGroup{
public:
    searchGroup(){
        pvLine = vector<int>(0,MAX_MOVE_NUM);
    }
public:
    static int searchQuesic(evaluate& e,int vlAlpha,int vlBeta) {
        return e.getEvaluate(e.side,vlAlpha,vlBeta);
    }
    int searchPV(evaluate& e,int depth,int vlAlpha,int vlBeta){
        if(depth <= 0){
            return searchQuesic(e,vlAlpha,vlBeta);
        }
        int vl;
        int vlBest = MIN_VALUE;
        vector<step> moveList;
        genMove::genMoveList(e,moveList,all);
        for(step & move : moveList){
            if(e.makeMove(move.fromPos,move.toPos)){
                if(vlBest == MIN_VALUE){
                    vl = -searchPV(e, depth - 1,-vlBeta,-vlAlpha);
                }else{
                    vl = -searchNonPV(e,depth - 1,-vlBest,MAX_VALUE);
                    if(vl > vlAlpha && vl < vlBeta){
                        vl = -searchPV(e,depth - 1,-vlBeta,-vlAlpha);
                    }
                }
                e.unMakeMove();

                if(vl > vlBest){
                    vlBest = vl;
                    if(vl >= vlBeta){
                        return vlBest;
                    }
                    if(vl > vlAlpha){
                        vlAlpha = vl;
                    }
                }
            }
        }
        return (vlBest == MIN_VALUE) ? MIN_VALUE + e.getNowDistance() : vlBest;
    }
    int searchNonPV(evaluate& e,int depth,int vlBeta,bool noNull = false){
        if(depth <= 0){
            return searchQuesic(e,vlBeta - 1,vlBeta);
        }
        int vl;
        int vlBest = MIN_VALUE;
        vector<step> moveList;
        genMove::genMoveList(e,moveList,all);
        for(step & move : moveList){
            if(e.makeMove(move.fromPos,move.toPos)){
                vl = -searchNonPV(e,depth - 1,-vlBeta + 1);
                e.unMakeMove();

                if(vl > vlBest){
                    vlBest = vl;
                    if(vl >= vlBeta){
                        return vlBest;
                    }
                }
            }
        }
        return (vlBest == MIN_VALUE) ? MIN_VALUE + e.getNowDistance() : vlBest;
    }
    int searchRoot(evaluate& e,int maxDepth){
        int vl;
        int vlBest = MIN_VALUE;
        vector<step> moveList;
        genMove::genMoveList(e,moveList,all);
        for(step & move : moveList){
            if(e.makeMove(move.fromPos,move.toPos)){
                if(vlBest == MIN_VALUE){
                    vl = -searchPV(e, maxDepth - 1,MIN_VALUE,MAX_VALUE);
                }else{
                    vl = -searchNonPV(e,maxDepth - 1,-vlBest,MAX_VALUE);
                    if(vl > vlBest){
                        vl = -searchPV(e,maxDepth - 1,MIN_VALUE,-vlBest);
                    }
                }
                e.unMakeMove();

                if(vl > vlBest){
                    vlBest = vl;
                    moveSort::sortRootMoveSequance(moveList,move);
                }
            }
        }
        moveList.front().printMove();
        return vlBest;
    }
    int searchMain(evaluate& e,int maxDepth,int maxTime){
        //init
        e.resetEvaBoard();
        int vlBest = MIN_VALUE;
        //search
        for(int nMaxDepth = 1;nMaxDepth <= maxDepth;nMaxDepth++){
            int vl = searchRoot(e,nMaxDepth);
            cout<<nMaxDepth<<endl;
            if(vl > vlBest){
                vlBest = vl;
                //toDo something
            }
        }
        return vlBest;
    }
private:
    vector<int> pvLine;             //主要遍历路线
};