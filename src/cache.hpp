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


class hashItem{
public:
    hashItem(){
        firstKey = secondKey = 0;
        vlAlpha = 0;
        vlBeta = 0;
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
    explicit hashCache(int n = 25){
        initCache(n);
    }
    ~hashCache(){
        delCache();
    }
    void initCache(int n){
        //clean entirely
        delCache();
        //reset
        cache.resize((uint64)1 << n);
        mask = ((uint64)1 << n) - (uint64)1;
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
            if(pH.alphaDepth >= depth && pH.vlAlpha <= vlAlpha){
                if(pH.move.fromPos){
                    move = pH.move;
                }
                if(readAdujstValue(e,pH.vlAlpha,vlGet)){
                    vl = vlGet;
                    return true;
                }
                return false;
            }
            if(pH.betaDepth >= depth && pH.vlBeta >= vlBeta){
                if(pH.move.fromPos){
                    move = pH.move;
                }
                if(readAdujstValue(e,pH.vlBeta,vlGet)){
                    vl = vlGet;
                    return true;
                }
                return false;
            }
        }
        return false;
    }

    static bool readAdujstValue(evaluate& e,int vlHash,int& vlGet){
        if(vlHash == e.getDrawValue()){
            return false;
        }
        if(abs(vlHash) <= (MAX_VALUE >> 1)){
            vlGet = vlHash;
            return true;
        }
        if(vlHash >= MAX_WIN_VALUE){
            vlGet = vlHash - e.getNowDistance();
            return true;
        }
        if(vlHash <= MIN_WIN_VALUE){
            vlGet = vlHash + e.getNowDistance();
            return true;
        }
        return false;
    }
    void recoardCache(evaluate& e,int nodeType,int vl,int depth,step* pMove = nullptr){
        hashItem& pH = cache[e.firstHashKey & mask];
        if(!pH.firstKey || !pH.secondKey){
            if(recoardAdujstValue(e,vl)){
                pH.firstKey = e.firstHashKey;
                pH.secondKey = e.secondHashKey;
                if(nodeType & alpha){
                    pH.vlAlpha = (int16)vl;
                    pH.alphaDepth = (int8)depth;
                }
                if(nodeType & beta){
                    pH.vlBeta = (int16)vl;
                    pH.betaDepth = (int8)depth;
                }
                if(pMove && pMove->fromPos){
                    pH.move = tinyMove(pMove->fromPos,pMove->toPos,pMove->fromPiece,pMove->toPiece);
                }
            }
        }else if(pH.firstKey == e.firstHashKey && pH.secondKey == e.secondHashKey){
            if(recoardAdujstValue(e,vl)){
                if((nodeType & alpha) && depth >= pH.alphaDepth && pH.vlAlpha >= vl){
                    pH.vlAlpha = (int16)vl;
                    pH.alphaDepth = depth;
                }
                if((nodeType & beta) && depth >= pH.betaDepth && pH.vlBeta <= vl){
                    pH.vlBeta = (int16)vl;
                    pH.betaDepth = depth;
                }
                if(pMove && pMove->fromPos){
                    pH.move = tinyMove(pMove->fromPos,pMove->toPos,pMove->fromPiece,pMove->toPiece);
                }
            }
        }
    }
    static int recoardAdujstValue(evaluate& e,int& vlGet){
        if(vlGet == e.getDrawValue()){
            return false;
        }
        if(abs(vlGet) <= SAFE_MAX_VALUE){
            return true;
        }
        if(vlGet >= MAX_WIN_VALUE){
            vlGet += e.getNowDistance();
            return true;
        }
        if(vlGet <= MIN_WIN_VALUE){
            vlGet -= e.getNowDistance();
            return true;
        }
        return false;
    }
protected:
    uint64 mask{};
    vector<hashItem> cache;
    friend class searchGroup;
};
