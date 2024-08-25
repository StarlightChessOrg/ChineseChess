#pragma once
#include "base.hpp"

class tinyMove{
public:
    tinyMove(){
        fromPos = toPos = 0;
        fromPiece = toPiece = 0;
    }
    tinyMove(uint8 fromPos,uint8 toPos,int8 fromPiece,int8 toPiece){
        this->fromPos = fromPos;
        this->toPos = toPos;
        this->fromPiece = fromPiece;
        this->toPiece = toPiece;
    }
    uint8 fromPos;
    uint8 toPos;
    int8 fromPiece;
    int8 toPiece;
    friend class hashItem;
    friend class position;
    friend class evaluate;
    friend class searchGroup;
};


class step{
public:
    step(){
        fromPos = 0;
        toPos = 0;
        fromPiece = 0;
        toPiece = 0;
        vl = 0;
        sortType = 0;
    }
    step(int fromPos,int toPos,int fromPiece,int toPiece,int vl = 0){
        this->fromPos = fromPos;
        this->toPos = toPos;
        this->fromPiece = fromPiece;
        this->toPiece = toPiece;
        this->vl = vl;
        sortType = 0;
    }
    void printMove() const{
        cout<<setw(3)<<fromPiece<<" from "<<fromPos<<" to "<<toPos <<" and eat "<<toPiece<<" with vl = "<<vl<<endl;
    }
    static void printMoveList(vector<step>& moveList){
        cout<<"--------------------------------------"<<endl;
        for(step& s : moveList){
            s.printMove();
        }
        cout<<"total size of move list is "<<moveList.size()<<endl;
    }
    bool operator!=(const step& otherMove) const{
        return !((fromPos == otherMove.fromPos) &&
                 (toPos == otherMove.toPos) &&
                 (fromPiece == otherMove.fromPiece) &&
                 (toPiece == otherMove.toPiece));
    }
    bool operator==(const step& otherMove) const{
        return  (fromPos == otherMove.fromPos) &&
                (toPos == otherMove.toPos) &&
                (fromPiece == otherMove.fromPiece) &&
                (toPiece == otherMove.toPiece);
    }
public:
    int fromPos;
    int toPos;
    int fromPiece;
    int toPiece;
    int vl;
    int sortType;
};

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
    friend class genMove;
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
                    redPawnPreMoveList[pos].emplace_back(pos,toPos);
                }else{
                    const int stepList[3] = {-1,1,-16};
                    for(int step : stepList){
                        const int toPos = pos + step;
                        if(inBoard[toPos]){
                            redPawnPreMoveList[pos].emplace_back(pos,toPos);
                        }
                    }
                }
            }
            if(inBlackPawnLine[pos]){
                if(inSideBoard[pos] < 0){
                    const int toPos = pos + 16;
                    blackPawnPreMoveList[pos].emplace_back(pos,toPos);
                }else{
                    const int stepList[3] = {-1,1,16};
                    for(int step : stepList){
                        const int toPos = pos + step;
                        if(inBoard[toPos]){
                            blackPawnPreMoveList[pos].emplace_back(pos,toPos);
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
    vector<preStep> redPawnPreMoveList[256];
    vector<preStep> blackPawnPreMoveList[256];
    friend class test;
    friend class genMove;
}preGenMoveInstance;


