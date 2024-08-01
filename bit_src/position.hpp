#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <algorithm>
#include "base.hpp"

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

enum lowOrUp{
    lowIndex = 0,
    upIndex = 1
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
        memset(RayBits,-1,sizeof(int) * 1024 * 10 * 2 * 4);
        for(uint16 bits = 0;bits < 1024;bits++){
            for(int from = 0;from < 10;from++){
                //low -> 下标减小的一侧
                const int low_index = 0;
                for(int to = from - 1,cnt = 0;to >= 0 && cnt < 4;to--){
                    if(bits & ((uint16)1 << to)){
                        RayBits[bits][from][low_index][cnt] = to;
                        cnt++;
                    }
                }
                //up -> 下标增加的一侧
                const int up_index = 1;
                for(int to = from + 1,cnt = 0;to < 10 && cnt < 4;to++){
                    if(bits & ((uint16)1 << to)){
                        RayBits[bits][from][up_index][cnt] = to;
                        cnt++;
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
    void unMakeMove(int yFrom,int xFrom,int yTo,int xTo,bool eaten){
        //撤销步进
        yBits[yFrom] |= ((uint16)1 << xFrom);
        xBits[xFrom] |= ((uint16)1 << yFrom);
        if(!eaten){
            yBits[yTo] &= ~((uint16)1 << xTo);
            xBits[xTo] &= ~((uint16)1 << yTo);
        }
    }
    int getRayTarget(const int pos,const int target,int num){
        assert(num >= 0 && num < 4);
        //pos 棋子位置 | target 攻击方向 | num 第几个棋子(从0开始数)
        const int x = getX(pos);
        const int y = getY(pos);
        //-2 错误值 | -1 没有被攻击到的棋子 | 攻击到的棋子索引(需要±3和存储格式对齐)
        int rayTarget = -2;
        switch (target) {
            case leftTarget:
                rayTarget = RayBits[yBits[y] >> 3][x - 3][lowIndex][num];
                break;
            case rightTarget:
                rayTarget = RayBits[yBits[y] >> 3][x - 3][upIndex][num];
                break;
            case upTarget:
                rayTarget = RayBits[xBits[x] >> 3][y - 3][lowIndex][num];
                break;
            case downTarget:
                rayTarget = RayBits[xBits[x] >> 3][y - 3][upIndex][num];
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
    int RayBits[1024][10][2][4]; //用于检测车炮，即“射线”类棋子的攻击目标 | low : 0 & up : 1
    uint16 mayExistBarrier[16][16]; //用于检测棋子之间的障碍物
private:
    uint16 yBits[16]{}; //横向是位向量
    uint16 xBits[16]{}; //纵向是位向量
    friend class position;
};

enum piece{
    Empty = 0,
    King = 1,
    Advisor = 2,
    Bishop = 3,
    Knight = 4,
    Rook = 5,
    Cannon = 6,
    Pawn = 7
};

const int initBasicBoard[256] = {

};

class basicBoard{
public:
    basicBoard(){
        clearBoard();
    }
    explicit basicBoard(const int anotherBoard[256]){
        readFromBoard(anotherBoard);
    }
    void readFromBoard(const int anotherBoard[256]){
        memcpy(this->board,anotherBoard,sizeof(int)*256);
    }
    void clearBoard(){
        memset(this->board,0,sizeof(int)*256);
    }
protected:
    int makeMove(int yFrom,int xFrom,int yTo,int xTo){
        const int fromPos = getPos(yFrom,xFrom);
        const int toPos = getPos(yTo,xTo);
        const int toPiece = this->board[toPos];
        this->board[toPos] = this->board[fromPos];
        this->board[fromPos] = 0;
        return toPiece;
    }
    void unMakeMove(int yFrom,int xFrom,int yTo,int xTo,int toPiece){
        const int fromPos = getPos(yFrom,xFrom);
        const int toPos = getPos(yTo,xTo);
        this->board[fromPos] = this->board[toPos];
        this->board[toPos] = toPiece;
    }
    static int getPos(int y,int x){
        //转换二维坐标到一维坐标
        return ((y << 4) | x);
    }
private:
    int board[256]{};
    friend class position;
};

//将士象马车炮兵
static const int swapVector[17] = {
    Empty,King,Advisor,Advisor,Bishop,Bishop,
    Knight,Knight,Rook,Rook,Cannon,Cannon,
    Pawn,Pawn,Pawn,Pawn, Pawn
};

class swapBasicBoard{
public:
    swapBasicBoard(){
        clearBoard();
    }
    explicit swapBasicBoard(const int anotherBoard[256]){
        readFromBoard(anotherBoard);
    }
    void readFromBoard(const int anotherBoard[256]){
        clearBoard();
        for(int pos = 51;pos < 205;pos++){
            const int piece = anotherBoard[pos];
            if(piece){
                this->swapBoard[pieceToSwapBoardIndex(piece)] = pos;
            }
        }
    }
    void clearBoard(){
        memset(this->swapBoard,0,sizeof(int) * 32);
    }
protected:
    void makeMove(int fromPiece,int toPiece,int toPos){
        const int fromPieceIndex = pieceToSwapBoardIndex(fromPiece);
        this->swapBoard[fromPieceIndex] = toPos;
        if(toPiece){
            const int toPieceIndex = pieceToSwapBoardIndex(toPiece);
            this->swapBoard[toPieceIndex] = 0;
        }
    }
    void unmakeMove(int fromPiece,int toPiece,int fromPos,int toPos){
        const int fromPieceIndex = pieceToSwapBoardIndex(fromPiece);
        this->swapBoard[fromPieceIndex] = fromPos;
        if(toPiece){
            const int toPieceIndex = pieceToSwapBoardIndex(toPiece);
            this->swapBoard[toPieceIndex] = toPos;
        }
    }
protected:
    static int pieceToSwapBoardIndex(int piece){
        //转换棋子编号到反映射数组下标
        return piece > 0 ? piece - 1 : abs(piece) + 15;
    }
    static int pieceToType(int piece){
        //转换棋子编号到棋子类型
        return piece > 0 ? swapVector[piece] : -swapVector[abs(piece)];
    }
private:
    int swapBoard[32]{};
    friend class position;
};

class position{
public:
    position(const int anotherBoard[256]){
        this->Board.readFromBoard(anotherBoard);
        this->swapBoard.readFromBoard(anotherBoard);
        this->bitBoard.readFromBoard(anotherBoard);
    }
    void makeMove(int yFrom,int xFrom,int yTo,int xTo){
        const int fromPos = basicBoard::getPos(yFrom,xFrom);
        const int toPos = basicBoard::getPos(yTo,xTo);
        const int fromPiece = this->Board.board[fromPos];
        const int toPiece = this->Board.makeMove(yFrom,xFrom,yTo,xTo);
        this->swapBoard.makeMove(fromPiece,toPiece,toPos);
        this->bitBoard.makeMove(yFrom,xFrom,yTo,xTo);
    }
    void unMakeMove(int yFrom,int xFrom,int yTo,int xTo,int fromPiece,int toPiece){
        this->Board.unMakeMove(yFrom,xFrom,yTo,xTo,toPiece);
        this->swapBoard.unmakeMove(fromPiece,toPiece,basicBoard::getPos(yFrom,xFrom),basicBoard::getPos(yTo,xTo));
        this->bitBoard.unMakeMove(yFrom,xFrom,yTo,xTo,toPiece);
    }
protected:
private:
    basicBoard Board;
    swapBasicBoard swapBoard;
    bitBoard bitBoard;
};