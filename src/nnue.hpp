#pragma once
#include "position.hpp"
#include <omp.h>

class nnue{
public:
    nnue(){
        initPara();
    }
    void initPara(){
        memset(layerWeight_1,0,sizeof(int) * 7168 * 128);
        memset(layerBias_1,0,sizeof(int) * 128);
        memset(layerWeight_2,0,sizeof(int) * 128);
        memset(inputCache,0,sizeof(int) * 128);
        layerBias_2 = 0;
    }
    void readPara(const string& rootPath,const double magnify = 4096.0){
        //w1
        ifstream weight_1(rootPath + "\\layer_1_weight.txt");
        string tStr;
        int lineIndex = 0;
        while(getline(weight_1,tStr)){
            vector<string> splits;
            stringSplit(tStr.c_str(),' ',splits);
            lineIndex++;
            for(int a = 0;a < splits.size();a++){
                const int tPara = atof(splits[a].c_str()) * magnify;
                layerWeight_1[a][lineIndex] = tPara;
            }
        }
        weight_1.close();
        //b1
        ifstream bias_1(rootPath + "\\layer_1_bias.txt");
        tStr.clear();
        getline(bias_1,tStr);
        vector<string> splits;
        stringSplit(tStr.c_str(),' ',splits);
        lineIndex++;
        for(int a = 0;a < splits.size();a++){
            const int tPara = atof(splits[a].c_str()) * magnify;
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
            const int tPara = atof(splits[a].c_str()) * magnify;
            layerWeight_2[a] = tPara;
        }
        weight_2.close();
        //b2
        ifstream bias_2(rootPath + "\\layer_2_bias.txt");
        tStr.clear();
        bias_2 >> tStr;
        const int tPara = atof(tStr.c_str()) * magnify;
        layerBias_2 = tPara;
        bias_2.close();
    }
protected:
    void makeMove(evaluate& e,step& move){
        const int fromIndex = getCacheIndex(move.fromPiece,move.fromPos,e.side);
        const int toIndex = getCacheIndex(move.fromPiece,move.toPos,e.side);
#pragma omp simd
        for(int i = 0;i < 128;i++){
            inputCache[i] += layerWeight_1[toIndex][i] - layerWeight_1[fromIndex][i];
        }
        if(move.toPiece){
            const int _toIndex= getCacheIndex(move.toPiece,move.toPos,-e.side);
#pragma omp simd
            for(int i = 0;i < 128;i++){
                inputCache[i] -= layerWeight_1[_toIndex][i];
            }
        }
    }
    void unmakeMove(evaluate& e,step& move){
        const int fromIndex = getCacheIndex(move.fromPiece,move.fromPos,e.side);
        const int toIndex = getCacheIndex(move.fromPiece,move.toPos,e.side);
#pragma omp simd
        for(int i = 0;i < 128;i++){
            inputCache[i] -= layerWeight_1[toIndex][i] - layerWeight_1[fromIndex][i];
        }
        if(move.toPiece){
            const int _toIndex= getCacheIndex(move.toPiece,move.toPos,-e.side);
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
                    const int index = getCacheIndex(piece,pos,e.side);
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
    int getCacheIndex(int piece,int pos,int side){
        const int type = swapBasicBoard::pieceToAbsType(piece);
        if(piece > 0 && side > 0){
            return ((type - 1) << 8) + pos;
        }else if(piece > 0 && side < 0){
            return ((type - 1 + 14) << 8) + pos;
        }else if(piece < 0 && side < 0){
            return ((type + 6) << 8) + pos;
        }else if(piece < 0 && side > 0){
            return ((type + 6 + 14) << 8) + pos;
        }
        assert(piece && side);
        return -1;
    }
private:
    int inputCache[128];
    int layerWeight_1[7168][128];
    int layerBias_1[128];
    int layerWeight_2[128];
    int layerBias_2;
    friend class test;
};