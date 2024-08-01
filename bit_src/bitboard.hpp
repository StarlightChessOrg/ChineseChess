#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>

using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

class bitBoard{
public:
    bitBoard(){
        initPublicResource();
        clearBitBoard();
    }
    explicit bitBoard(const int board[256]){
        initPublicResource();
        readFromBoard(board);
    }
    void initPublicResource(){
        //初始化判断棋子间障碍的屏蔽位
        for(int i = 3;i < 13;i++){
            for(int a = 3;a < 13;a++){
                for(int from = min(i,a) + 1;from < max(i,a);from++){
                    mayExistBarrier[i][a] |= ((uint16)1 << from);
                }
            }
        }
    }
    void readFromBoard(const int board[256]){
        clearBitBoard();
        //通过外部数组，初始化位棋盘
        for(int pos = 51;pos < 205;pos++){
            if(board[pos]){
                yBits[getY(pos)] |= ((uint16)1 << (getX(pos)));
                xBits[getX(pos)] |= ((uint16)1 << (getY(pos)));
            }
        }
    }
    void clearBitBoard(){
        //清空数组
        memset(yBits,(uint16)0,16 * sizeof(uint16));
        memset(xBits,(uint16)0,16 * sizeof(uint16));
    }
    void printBitBoard(bool printTranspose = true){
        //打印整个棋盘的bit位
        cout<<"yBits:"<<endl;
        for(unsigned short yBit : yBits){
            printBitLine(yBit);
        }
        //打印转置棋盘的bit位
        if(printTranspose){
            cout<<endl;
            cout<<"xBits:"<<endl;
            for(unsigned short xBit : xBits){
                printBitLine(xBit);
            }
        }
    }
    static void printBitLine(uint16 Bits){
        //打印单行的bit位
        for(int i = 0;i < 16;i++){
            if(Bits & ((uint16)1 << i)){
                cout <<1;
            }else{
                cout<<".";
            }
            if(i != 15){
                cout<<" ";
            }
        }
        cout<<endl;
    }
public:
    bool checkLineExistBarrier(int yFrom,int xFrom,int yTo,int xTo){
        assert((xFrom != xTo) || (yFrom != yTo));
        //检查啷个棋子中间是否有障碍物
        if(xFrom != xTo){
            return mayExistBarrier[xFrom][xTo] & yBits[yFrom];
        }
        return mayExistBarrier[yFrom][yTo] & xBits[xFrom];
    }
    void makeMove(int yFrom,int xFrom,int yTo,int xTo){
        //步进
        yBits[yFrom] &= ~((uint16)1 << xFrom);
        yBits[yTo] |= ((uint16)1 << xTo);
        xBits[xFrom] &= ~((uint16)1 << yFrom);
        xBits[xTo] |= ((uint16)1 << yTo);
    }
    void unMakeMove(int yFrom,int xFrom,int yTo,int xTo){
        //撤销步进
        yBits[yFrom] |= ((uint16)1 << xFrom);
        yBits[yTo] &= ~((uint16)1 << xTo);
        xBits[xFrom] |= ((uint16)1 << yFrom);
        xBits[xTo] &= ~((uint16)1 << yTo);
    }
protected:
    static int getX(int pos){
        //获得棋子的列索引
        return pos & 15;
    }
    static int getY(int pos){
        //获得棋子的行索引
        return (pos >> 4);
    }
protected:
    uint16 mayExistBarrier[16][16]{};
private:
    uint16 yBits[16]{}; //横向是位向量
    uint16 xBits[16]{}; //纵向是位向量
};