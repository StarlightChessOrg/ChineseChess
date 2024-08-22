#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include "preGenMove.hpp"

using namespace std;

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
                for(int from = std::min(i,a) + 1;from < std::max(i,a);from++){
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
        std::cout<<std::endl;
        std::cout<<"xBits:"<<std::endl;
        for(unsigned short xBit : xBits){
            printBitLine(xBit);
        }
        //打印转置棋盘的bit位
        if(printTranspose){
            std::cout<<"yBits:"<<std::endl;
            for(unsigned short yBit : yBits){
                printBitLine(yBit);
            }
        }
    }
    static void printBitLine(uint16 Bits){
        //打印单行的bit位
        for(int i = 0;i < 16;i++){
            if(Bits & ((uint16)1 << i)){
                std::cout <<1;
            }else{
                std::cout<<".";
            }
            if(i != 15){
                std::cout<<" ";
            }
        }
        std::cout<<std::endl;
    }
public:
    //只能用于射线类棋子
    static int selectTarget(int fromPos,int toPos){
        if(toPos - fromPos > 16){
            return downTarget;
        }
        if(toPos - fromPos < -16){
            return upTarget;
        }
        if(toPos - fromPos > 0){
            return rightTarget;
        }
        if(toPos - fromPos < 0){
            return leftTarget;
        }
        return -1;
    }
    bool checkLineExistBarrier(int fromPos,int toPos){
        const int xFrom = getX(fromPos);
        const int yFrom = getY(fromPos);
        const int xTo = getX(toPos);
        const int yTo = getY(toPos);
        assert(((xFrom != xTo) + (yFrom != yTo)) == 1);
        //检查啷个棋子中间是否有障碍物
        if(xFrom != xTo){
            return mayExistBarrier[xFrom][xTo] & yBits[yFrom];
        }
        return mayExistBarrier[yFrom][yTo] & xBits[xFrom];
    }
    void makeMove(int fromPos,int toPos){
        const int xFrom = getX(fromPos);
        const int yFrom = getY(fromPos);
        const int xTo = getX(toPos);
        const int yTo = getY(toPos);
        //步进
        yBits[yFrom] &= ~((uint16)1 << xFrom);
        yBits[yTo] |= ((uint16)1 << xTo);
        xBits[xFrom] &= ~((uint16)1 << yFrom);
        xBits[xTo] |= ((uint16)1 << yTo);
    }
    void unMakeMove(int fromPos,int toPos,bool eaten){
        const int xFrom = getX(fromPos);
        const int yFrom = getY(fromPos);
        const int xTo = getX(toPos);
        const int yTo = getY(toPos);
        //撤销步进
        yBits[yFrom] |= ((uint16)1 << xFrom);
        xBits[xFrom] |= ((uint16)1 << yFrom);
        if(!eaten){
            yBits[yTo] &= ~((uint16)1 << xTo);
            xBits[xTo] &= ~((uint16)1 << yTo);
        }
    }
    int getRayTargetPos(const int pos,const int target,const int num){
        return convertRayTargetToPos(pos,target, getRayTarget(pos,target,num));
    }
protected:
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
                std::cout<<"error occurred in the func named getRayTarget()"<<std::endl;
                break;
        }
        assert(rayTarget != -2);
        if(rayTarget != -1){
            return rayTarget + 3;
        }
        return rayTarget;
    }
    static int convertRayTargetToPos(const int pos,const int target,const int rayTarget){
        int convertPos = -1;
        if(rayTarget > -1){
            if(target == leftTarget || target == rightTarget){
                convertPos = getPos(getY(pos),rayTarget);
            }else{
                convertPos = getPos(rayTarget, getX(pos));
            }
        }
        return convertPos;
    }
    bool operator==(const bitBoard& other){
        for(int i = 0;i < 16;i++){
            if(yBits[i] != other.yBits[i]){
                return false;
            }
            if(xBits[i] != other.xBits[i]){
                return false;
            }
        }
        return true;
    }
    bool operator!=(const bitBoard& other){
        for(int i = 0;i < 16;i++){
            if(yBits[i] != other.yBits[i]){
                return true;
            }
            if(xBits[i] != other.xBits[i]){
                return true;
            }
        }
        return false;
    }
private:
    int RayBits[1024][10][2][4]{};          //用于检测车炮，即“射线”类棋子的攻击目标 | low : 0 & up : 1
    uint16 mayExistBarrier[16][16]{};       //用于检测棋子之间的障碍物
    uint16 yBits[16]{};                     //横向是位向量
    uint16 xBits[16]{};                     //纵向是位向量
    friend class position;
    friend class evaluate;
};

enum piece{
    empty = 0,
    king = 1,
    advisor = 2,
    bishop = 3,
    knight = 4,
    rook = 5,
    cannon = 6,
    pawn = 7
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
    void printBasicBoard(){
        for(int y = 0;y < 16;y++){
            for(int x = 0;x < 16;x++){
                const int pos = getPos(y,x);
                const int piece = this->board[pos];
                if(piece){
                    std::cout<<std::setw(4)<<piece;
                }else{
                    std::cout<<std::setw(4)<<".";
                }
            }
            std::cout<<std::endl;
        }
    }

    int makeMove(int fromPos,int toPos){
        const int toPiece = this->board[toPos];
        this->board[toPos] = this->board[fromPos];
        this->board[fromPos] = 0;
        return toPiece;
    }

    void unMakeMove(int fromPos,int toPos,int toPiece){
        this->board[fromPos] = this->board[toPos];
        this->board[toPos] = toPiece;
    }
protected:
    int getPieceByPos(int pos) const {
//        if(pos < 0 || pos > 255){
//            cout<<endl;
//        }
        assert(pos >= 0 && pos <= 255);
        return board[pos];
    }
    bool operator==(const basicBoard& other){
        for(int i = 0;i < 256;i++){
            if(board[i] != other.board[i]){
                return false;
            }
        }
        return true;
    }
    bool operator!=(const basicBoard& other){
        for(int i = 0;i < 256;i++){
            if(board[i] != other.board[i]){
                return true;
            }
        }
        return false;
    }
private:
    int board[256]{};
    friend class position;
    friend class genMove;
    friend class evaluate;
    friend class hashKey;
    friend class test;
};

//将士象马车炮兵
static const int swapVector[17] = {
    empty,king,advisor,advisor,bishop,bishop,
    knight,knight,rook,rook,cannon,cannon,
    pawn,pawn,pawn,pawn,pawn
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
    void printSwapBoard(){
        for(int i : this->swapBoard){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
    }

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
        assert(fromPieceIndex >= 0 && fromPieceIndex <= 31);
        this->swapBoard[fromPieceIndex] = fromPos;
        if(toPiece){
            const int toPieceIndex = pieceToSwapBoardIndex(toPiece);
            this->swapBoard[toPieceIndex] = toPos;
        }
    }

protected:
    int getPosByPiece(int piece){
        return swapBoard[pieceToSwapBoardIndex(piece)];
    }
    static int pieceToSwapBoardIndex(int piece){
        //转换棋子编号到反映射数组下标
        return piece > 0 ? piece - 1 : abs(piece) + 15;
    }
    static int pieceToType(int piece){
        //转换棋子编号到棋子类型
        return piece > 0 ? swapVector[piece] : -swapVector[abs(piece)];
    }
    static int pieceToAbsType(int piece){
        //转换棋子编号到棋子类型
        return swapVector[abs(piece)];
    }
    bool operator==(const swapBasicBoard& other){
        for(int i = 0;i < 32;i++){
            if(swapBoard[i] != other.swapBoard[i]){
                return false;
            }
        }
        return true;
    }
    bool operator!=(const swapBasicBoard& other){
        for(int i = 0;i < 32;i++){
            if(swapBoard[i] != other.swapBoard[i]){
                return true;
            }
        }
        return false;
    }
private:
    int swapBoard[32]{};
    friend class position;
    friend class genMove;
    friend class evaluate;
    friend class hashKey;
    friend class moveSort;
    friend class searchGroup;
};

class position{
public:
    explicit position(const int anotherBoard[256] = initGameBoard, int initSide = red){
        this->side = initSide;
        this->board.readFromBoard(anotherBoard);
        this->swapBoard.readFromBoard(anotherBoard);
        this->bitBoard.readFromBoard(anotherBoard);
    }

    void initPosition(const int anotherBoard[256] = initGameBoard, int initSide = red){
        this->side = initSide;
        this->board.readFromBoard(anotherBoard);
        this->swapBoard.readFromBoard(anotherBoard);
        this->bitBoard.readFromBoard(anotherBoard);
    }

    void makeMove(int fromPos, int toPos){
        changeSide();
        const int fromPiece = this->board.board[fromPos];
        const int toPiece = this->board.makeMove(fromPos,toPos);
        this->swapBoard.makeMove(fromPiece,toPiece,toPos);
        this->bitBoard.makeMove(fromPos,toPos);
    }

    virtual void unMakeMove(int fromPos,int toPos,int fromPiece,int toPiece){
        changeSide();
        this->board.unMakeMove(fromPos,toPos,toPiece);
        this->swapBoard.unmakeMove(fromPiece,toPiece,fromPos,toPos);
        this->bitBoard.unMakeMove(fromPos,toPos,toPiece);
    }
protected:
    bool operator==(const position& other){
        if(side != other.side){
            return false;
        }
        if(firstHashKey != other.firstHashKey ||
           secondHashKey != other.secondHashKey){
            return false;
        }
        if(board != other.board ||
           swapBoard != other.swapBoard ||
           bitBoard != other.bitBoard){
            return false;
        }
        return true;
    }
    bool operator!=(const position& other){
        if(side != other.side){
            return true;
        }
        if(firstHashKey != other.firstHashKey ||
           secondHashKey != other.secondHashKey){
            return true;
        }
        if(board != other.board ||
           swapBoard != other.swapBoard ||
           bitBoard != other.bitBoard){
            return true;
        }
        return false;
    }
    void changeSide(){
        this->side = -this->side;
    }
protected:
    int side;
    basicBoard board;
    swapBasicBoard swapBoard;
    class bitBoard bitBoard;
    uint64 firstHashKey{};
    uint64 secondHashKey{};
    friend class genMove;
    friend class test;
    friend class hashKey;
    friend class searchGroup;
};

class hashKey{
public:
    hashKey(){
        initHashKey();
    }

protected:
    void entireKey(position& p,uint64& firstkey,uint64& secondKey){
        firstkey = secondKey = 0;
        for(int pos = 51;pos < 205;pos++){
            const int piece = p.board.getPieceByPos(pos);
            if(piece){
                int convert_type = swapBasicBoard::pieceToAbsType(piece) - 1;
                firstkey ^= keyMatrix[0][convert_type][pos];
                secondKey ^= keyMatrix[1][convert_type][pos];
            }
        }
        this->keyPlayer = getKey();
    }
    void stepKey(uint64& firstkey,uint64& secondKey,step move){
        int from_convert_type = swapBasicBoard::pieceToAbsType(move.fromPiece) - 1;
        if(move.fromPiece < 0){
            from_convert_type += 7;
        }
        assert(from_convert_type >= 0 && from_convert_type <= 13);
        firstkey ^= keyMatrix[0][from_convert_type][move.fromPos];
        firstkey ^= keyMatrix[0][from_convert_type][move.toPos];
        secondKey ^= keyMatrix[1][from_convert_type][move.fromPos];
        secondKey ^= keyMatrix[1][from_convert_type][move.toPos];
        if(move.toPiece){
            int to_convert_type = swapBasicBoard::pieceToAbsType(move.toPiece) - 1;
            assert(to_convert_type >= 0 && to_convert_type <= 13);
            if(move.toPiece < 0){
                to_convert_type += 7;
            }
            firstkey ^= keyMatrix[0][to_convert_type][move.toPos];
            secondKey ^= keyMatrix[1][to_convert_type][move.toPos];
        }
    }
    void initHashKey(){
        e.seed(7931);
        for(auto& i : keyMatrix){
            for(auto& a : i){
                for(auto& c : a){
                    c = getKey();
                }
            }
        }
        keyPlayer = getKey();
    }
private:
    uint64 getKey(){
        uniform_int_distribution<uint64> u(16383,65535);
        return u(e) ^ (u(e) << 15) ^ (u(e) << 30) ^ (u(e) << 45) ^ (u(e) << 60);
    }
protected:
    default_random_engine e;
    uint64 keyMatrix[2][14][256]{};
    uint64 keyPlayer{};
    friend class searchGroup;
    friend class evaluate;
};