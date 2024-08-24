#pragma once
#include "position.hpp"
#include <omp.h>
#include <cmath>

class nnue{
public:
    nnue(){
        initPara();
    }
    void initPara(){
        memset(layerWeight_1,0,sizeof(int) * 3584 * 128);
        memset(layerBias_1,0,sizeof(int) * 128);
        memset(layerWeight_2,0,sizeof(int) * 128);
        memset(inputCache,0,sizeof(int) * 128);
        layerBias_2 = 0;
    }
    void readPara(const string& rootPath){
        const double firstMagnify = 128.0;
        const double secondMagnify = 64.0;
        //w1
        ifstream weight_1(rootPath + "\\layer_1_weight.txt");
        string tStr;
        int lineIndex = 0;
        while(getline(weight_1,tStr)){
            vector<string> splits;
            stringSplit(tStr.c_str(),' ',splits);
            for(int a = 0;a < splits.size();a++){
                const int tPara = atof(splits[a].c_str()) * firstMagnify;
                layerWeight_1[a][lineIndex] = tPara;
            }
            lineIndex++;
        }
        weight_1.close();
        //b1
        ifstream bias_1(rootPath + "\\layer_1_bias.txt");
        tStr.clear();
        getline(bias_1,tStr);
        vector<string> splits;
        stringSplit(tStr.c_str(),' ',splits);
        for(int a = 0;a < splits.size();a++){
            const int tPara = atof(splits[a].c_str()) * firstMagnify;
            layerBias_1[a] = tPara;
        }
        bias_1.close();
        //w2
        ifstream weight_2(rootPath + "\\layer_2_weight.txt");
        tStr.clear();
        getline(weight_2,tStr);
        splits.clear();
        stringSplit(tStr.c_str(),' ',splits);
        for(int a = 0;a < splits.size();a++){
            const int tPara = atof(splits[a].c_str()) * secondMagnify;
            layerWeight_2[a] = tPara;
        }
        weight_2.close();
        //b2
        ifstream bias_2(rootPath + "\\layer_2_bias.txt");
        tStr.clear();
        bias_2 >> tStr;
        const int tPara = atof(tStr.c_str()) * secondMagnify;
        layerBias_2 = tPara;
        bias_2.close();
    }
protected:
    int forward(evaluate& e){
        int _inputCache[128] = {0};
        memcpy(_inputCache,inputCache,sizeof(int) * 128);
#pragma omp simd
        for(int & v : _inputCache){
            v = (v > 0) ? v : 0;
        }
        int output = 0;
#pragma omp simd
        for(int i = 0;i < 128;i++){
            output += _inputCache[i] * layerWeight_2[i];
        }
        output += layerBias_2;
        output = min(max(SAFE_MIN_VALUE,output),SAFE_MAX_VALUE);
        if(e.side < 0){
            return -output;
        }
        return output;
    }
    void makeMove(step& move){
        const int fromIndex = getAheadCacheIndex(move.fromPiece,move.fromPos);
        const int toIndex = getAheadCacheIndex(move.fromPiece,move.toPos);
#pragma omp simd
        for(int i = 0;i < 128;i++){
            inputCache[i] += layerWeight_1[toIndex][i] - layerWeight_1[fromIndex][i];
        }
        if(move.toPiece){
            const int _toIndex= getAheadCacheIndex(move.toPiece,move.toPos);
#pragma omp simd
            for(int i = 0;i < 128;i++){
                inputCache[i] -= layerWeight_1[_toIndex][i];
            }
        }
    }
    void unmakeMove(step& move){
        const int fromIndex = getAheadCacheIndex(move.fromPiece,move.fromPos);
        const int toIndex = getAheadCacheIndex(move.fromPiece,move.toPos);
#pragma omp simd
        for(int i = 0;i < 128;i++){
            inputCache[i] -= layerWeight_1[toIndex][i] - layerWeight_1[fromIndex][i];
        }
        if(move.toPiece){
            const int _toIndex = getAheadCacheIndex(move.toPiece,move.toPos);
#pragma omp simd
            for(int i = 0;i < 128;i++){
                inputCache[i] += layerWeight_1[_toIndex][i];
            }
        }
    }
    void initCache(evaluate& e){
        memset(inputCache,0,sizeof(int) * 128);
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                const int piece = e.board.getPieceByPos(pos);
                if(piece){
                    const int index = getAheadCacheIndex(piece,pos);
#pragma omp simd
                    for(int i = 0;i < 128;i++){
                        inputCache[i] += layerWeight_1[index][i];
                    }
                }
            }
        }
#pragma omp simd
        for(int i = 0;i < 128;i++){
            inputCache[i] += layerBias_1[i];
        }
    }
private:
    int getAheadCacheIndex(int piece,int pos){
        assert(piece && pos);
        const int type = swapBasicBoard::pieceToAbsType(piece);
        if(piece > 0){
            return ((abs(type) - 1) << 8) + pos;
        }else if(piece < 0){
            return ((abs(type) + 6) << 8) + pos;
        }
        return -1;
    }
    int getBehindCacheIndex(int side){
        assert(abs(side) == 1);
        if(side == red){
            return 1;
        }else if(side == black){
            return 0;
        }
        return -1;
    }
private:
    int inputCache[128];
    int layerWeight_1[3584][128];
    int layerBias_1[128];
    int layerWeight_2[128];
    int layerBias_2;
    friend class test;
    friend class searchGroup;
};