#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

enum target{
    leftTarget = 0,
    rightTarget = 1,
    upTarget = 2,
    downTarget = 3
};

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
        //初始化检测车炮攻击目标的数组
        memset(RayBits,-1,sizeof(int) * 1024 * 10 * 2);
        for(uint16 bits = 0;bits < 1024;bits++){
            for(int from = 0;from < 10;from++){
                //low -> 下标减小的一侧
                const int low_index = 0;
                for(int to = from - 1;to >= 0;to--){
                    if(bits & ((uint16)1 << to)){
                        RayBits[bits][from][low_index] = to;
                        break;
                    }
                }
                //up -> 下标增加的一侧
                const int up_index = 1;
                for(int to = from + 1;to < 10;to++){
                    if(bits & ((uint16)1 << to)){
                        RayBits[bits][from][up_index] = to;
                        break;
                    }
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
    int getRayTarget(const int pos,const int target){
        const int x = getX(pos);
        const int y = getY(pos);
        int rayTarget = -2;
        switch (target) {
            case leftTarget:
                rayTarget = RayBits[yBits[y] >> 3][x - 3][0];
                break;
            case rightTarget:
                rayTarget = RayBits[yBits[y] >> 3][x - 3][1];
                break;
            case upTarget:
                rayTarget = RayBits[xBits[x] >> 3][y - 3][0];
                break;
            case downTarget:
                rayTarget = RayBits[xBits[x] >> 3][y - 3][1];
                break;
            default:
                cout<<"error occurred in the func named getRayTarget()"<<endl;
                break;
        }
        assert(rayTarget != -2);
        if(rayTarget != -1){
            return rayTarget + 3;
        }
        return rayTarget;
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
    int RayBits[1024][10][2]; //用于检测车炮，即“射线”类棋子的攻击目标 | low : 0 & up : 1
    uint16 mayExistBarrier[16][16]; //用于检测棋子之间的障碍物
private:
    uint16 yBits[16]{}; //横向是位向量
    uint16 xBits[16]{}; //纵向是位向量
};