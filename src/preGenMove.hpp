#pragma once
#include "base.hpp"

class preStep{
public:
    preStep(){
        fromPos = toPos = 0;
        barrierPos = 0;
    }
    preStep(int fromPos,int toPos,int barrierPos = 0){
        this->fromPos = fromPos;
        this->toPos = toPos;
        this->barrierPos = barrierPos;
    }
protected:
    int fromPos;
    int toPos;
    int barrierPos;
    friend class test;
};

enum gameSide{
    red = 1,
    black = -1
};

class preGenMove{
public:
    preGenMove(){
        preGenKing();
        preGenAdvisor();
        preGenBishop();
        preGenKnight();
        preGenPawn();
    }
private:
    void preGenKing(){
        for(int pos = 51;pos < 205;pos++){
            if(inFort[pos]){
                for(int step : rayDelta) {
                    const int toPos = pos + step;
                    if(inFort[toPos]){
                        kingPreMoveList[pos].emplace_back(pos,toPos);
                    }
                }
            }
        }
    }
    void preGenAdvisor(){
        for(int pos = 51;pos < 205;pos++){
            if(inAdvisorLine[pos]){
                for(int step : advisorDelta){
                    const int toPos = pos + step;
                    if(inAdvisorLine[toPos]){
                        advisorPreMoveList[pos].emplace_back(pos,toPos);
                    }
                }
            }
        }
    }
    void preGenBishop(){
        for(int pos = 51;pos < 205;pos++){
            if(inBishopLine[pos]){
                int virtualSide = (inSideBoard[pos] > 0) ? red : black;
                for(int step : bishopDelta){
                    const int toPos = pos + step;
                    if(inBishopLine[toPos] && virtualSide * inSideBoard[toPos] > 0){
                        bishopPreMoveList[pos].emplace_back(pos,toPos,getBishopEye(pos,toPos));
                    }
                }
            }
        }
    }
    void preGenKnight(){
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                for(int step : knightDelta){
                    const int toPos = pos + step;
                    if(inBoard[toPos]){
                        knightPreMoveList[pos].emplace_back(pos,toPos,getKnightLeg(pos,toPos));
                    }
                }
            }
        }
    }
    void preGenPawn(){
        for(int pos = 51;pos < 205;pos++){
            if(inRedPawnLine[pos]){
                if(inSideBoard[pos] > 0){
                    const int toPos = pos - 16;
                    RedPawnPreMoveList[pos].emplace_back(pos,toPos);
                }else{
                    const int stepList[3] = {-1,1,-16};
                    for(int step : stepList){
                        const int toPos = pos + step;
                        if(inBoard[toPos]){
                            RedPawnPreMoveList[pos].emplace_back(pos,toPos);
                        }
                    }
                }
            }
            if(inBlackPawnLine[pos]){
                if(inSideBoard[pos] < 0){
                    const int toPos = pos + 16;
                    BlackPawnPreMoveList[pos].emplace_back(pos,toPos);
                }else{
                    const int stepList[3] = {-1,1,16};
                    for(int step : stepList){
                        const int toPos = pos + step;
                        if(inBoard[toPos]){
                            BlackPawnPreMoveList[pos].emplace_back(pos,toPos);
                        }
                    }
                }
            }
        }
    }
protected:
    vector<preStep> kingPreMoveList[256];
    vector<preStep> advisorPreMoveList[256];
    vector<preStep> bishopPreMoveList[256];
    vector<preStep> knightPreMoveList[256];
    vector<preStep> RedPawnPreMoveList[256];
    vector<preStep> BlackPawnPreMoveList[256];
}preGenMoveInstance;


