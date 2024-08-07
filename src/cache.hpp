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
        clearCache();
    }
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

enum nodeType{
    alpha = 1,
    beta = 2,
    pv = 3
};

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

class hashItem{
public:
    hashItem(){
        firstKey = secondKey = 0;
        vlAlpha = vlBeta = 0;
        alphaDepth = betaDepth = 0;
    }
protected:
    uint64 firstKey;
    int16 vlAlpha;
    int16 vlBeta;
    uint8 alphaDepth;
    uint8 betaDepth;
    tinyMove move;
    uint64 secondKey;
    friend class hashCache;
    friend class searchGroup;
};

class hashCache{
public:
    hashCache(int n = 25){
        initCache(n);
    }
    ~hashCache(){
        delCache();
    }
    void initCache(int n){
        //clean entirely
        delCache();
        //reset
        this->n = n;
        cache.resize(1 << n);
        mask = (1 << n) - 1;
    }
    void delCache(){
        vector<hashItem>().swap(cache);
    }
    void clearCache(){
        for(hashItem& h : cache){
            h = hashItem();
        }
    }
protected:
    bool getCache(evaluate& e,int depth,int vlAlpha,int vlBeta,int& vl,tinyMove& move){
        hashItem& pH = cache[e.firstHashKey & mask];
        int vlGet = 0;
        if(pH.firstKey == e.firstHashKey && pH.secondKey == e.secondHashKey){
            if(pH.alphaDepth > 0 && pH.alphaDepth >= depth && pH.vlAlpha <= vlAlpha){
                if(readAdujstValue(e,pH.vlAlpha,vlGet)){
                    vl = vlGet;
                }
                move = pH.move;
                return true;
            }
            if(pH.betaDepth > 0 && pH.betaDepth >= depth && pH.vlBeta >= vlBeta){
                if(readAdujstValue(e,pH.vlBeta,vlGet)){
                    vl = vlGet;
                }
                move = pH.move;
                return true;
            }
        }
        return false;
    }

    static bool readAdujstValue(evaluate& e,int vlHash,int& vlGet){
        if(vlHash != e.getDrawValue()){
            if(vlHash >= MAX_WIN_VALUE){
                vlGet = vlHash - e.getNowDistance();
                return true;
            }else if(vlHash <= MIN_WIN_VALUE){
                vlGet = vlHash + e.getNowDistance();
                return true;
            }
        }
        return false;
    }
    void recoardCache(evaluate& e,int nodeType,int vl,int vlAlpha,int vlBeta,int depth,step& move){
        hashItem& pH = cache[e.firstHashKey & mask];
        if(pH.firstKey == e.firstHashKey && pH.secondKey == e.secondHashKey){
            if((nodeType & alpha) && (depth >= pH.alphaDepth || vlAlpha >= pH.vlAlpha)){
                if(recoardAdujstValue(e,vl)){
                    pH.vlAlpha = vl;
                    pH.alphaDepth = depth;
                }
            }else if((nodeType & beta) && (depth >= pH.betaDepth || vlBeta <= pH.vlBeta)){
                if(recoardAdujstValue(e,vl)){
                    pH.vlBeta = vl;
                    pH.betaDepth = depth;
                }
            }
        }else if(!pH.firstKey || !pH.secondKey){
            if(nodeType & alpha){
                if(recoardAdujstValue(e,vl)){
                    pH.vlAlpha = vl;
                    pH.alphaDepth = depth;
                }
            }else if(nodeType & beta){
                if(recoardAdujstValue(e,vl)){
                    pH.vlBeta = vl;
                    pH.betaDepth = depth;
                }
            }
        }
        pH.move = tinyMove(move.fromPos,move.toPos,move.fromPiece,move.toPiece);
    }
    static int recoardAdujstValue(evaluate& e,int& vlGet){
        if(vlGet != e.getDrawValue()){
            if(vlGet >= MAX_WIN_VALUE){
                vlGet += e.getNowDistance();
                return true;
            }else if(vlGet <= MIN_WIN_VALUE){
                vlGet -= e.getNowDistance();
                return true;
            }
        }
        return false;
    }
protected:
    int n;
    int mask{};
    vector<hashItem> cache;
    friend class searchGroup;
};
