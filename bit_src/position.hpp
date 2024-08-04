#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "base.hpp"
#include "evaluate.hpp"

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
        cout<<endl;
        cout<<"xBits:"<<endl;
        for(unsigned short xBit : xBits){
            printBitLine(xBit);
        }
        //打印转置棋盘的bit位
        if(printTranspose){
            cout<<"yBits:"<<endl;
            for(unsigned short yBit : yBits){
                printBitLine(yBit);
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
                cout<<"error occurred in the func named getRayTarget()"<<endl;
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
                    cout<<setw(4)<<piece;
                }else{
                    cout<<setw(4)<<".";
                }
            }
            cout<<endl;
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
        return board[pos];
    }
private:
    int board[256]{};
    friend class position;
    friend class genMove;
    friend class evaluate;
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
            cout<<i<<" ";
        }
        cout<<endl;
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
private:
    int swapBoard[32]{};
    friend class position;
    friend class genMove;
    friend class evaluate;
};

enum gameSide{
    red = 1,
    mid = 0,
    black = -1
};

class position{
public:
    explicit position(const int anotherBoard[256] = initGameBoard, int initSide = red){
        this->side = initSide;
        this->board.readFromBoard(anotherBoard);
        this->swapBoard.readFromBoard(anotherBoard);
        this->bitBoard.readFromBoard(anotherBoard);
    }

    virtual void makeMove(int fromPos,int toPos){
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
    void changeSide(){
        this->side = -this->side;
    }

protected:
    int side;
    basicBoard board;
    swapBasicBoard swapBoard;
    bitBoard bitBoard;
    friend class genMove;
    friend class test;
};

class evaluate : public position{
public:
    explicit evaluate(const int anotherBoard[256] = initGameBoard, int initSide = red) : position(anotherBoard,initSide){
        clearEvaBoard();
    }
    void clearEvaBoard(){
        vlRed = 0;
        vlBlack = 0;
        vlFirstGo = 4;
        memset(vlRedBoard,0,sizeof(int) * 7 * 256);
        memset(vlBlackBoard,0,sizeof(int) * 7 * 256);
    }
    void makeMove(int fromPos,int toPos) override{
        const int fromPiece = position::board.getPieceByPos(fromPos);
        const int fromIndex = swapBasicBoard::pieceToAbsType(fromPiece) - 1;
        if(fromPiece > 0){
            this->vlRed += vlRedBoard[fromIndex][toPos] - vlRedBoard[fromIndex][fromPos];
        }else if(fromPiece < 0){
            this->vlBlack += vlBlackBoard[fromIndex][toPos] - vlBlackBoard[fromIndex][fromPos];
        }
        const int toPiece = position::board.getPieceByPos(toPos);
        if(toPiece){
            const int toIndex = swapBasicBoard::pieceToAbsType(toPiece) - 1;
            if(toPiece > 0){
                this->vlRed -= vlRedBoard[fromIndex][toPos];
            }else{
                this->vlBlack -= vlBlackBoard[toIndex][toPos];
            }
        }

        position::makeMove(fromPos,toPos);
    }
    void unMakeMove(int fromPos,int toPos,int fromPiece,int toPiece) override{
        const int fromIndex = swapBasicBoard::pieceToAbsType(fromPiece) - 1;
        if(fromPiece > 0){
            this->vlRed -= vlRedBoard[fromIndex][toPos] - vlRedBoard[fromIndex][fromPos];
        }else if(fromPiece < 0){
            this->vlBlack -= vlBlackBoard[fromIndex][toPos] - vlBlackBoard[fromIndex][fromPos];
        }
        if(toPiece){
            const int toIndex = swapBasicBoard::pieceToAbsType(toPiece) - 1;
            if(toPiece > 0){
                this->vlRed += vlRedBoard[fromIndex][toPos];
            }else{
                this->vlBlack += vlBlackBoard[toIndex][toPos];
            }
        }

        position::unMakeMove(fromPos,toPos,fromPiece,toPiece);
    }
protected:
    void resetEvaBoard(){
        int midgameValue = 0;
        int redAttackValue = 0;
        int blackAttackValue = 0;
        int pawnPieceAttacking[256] = {0};
        int pawnPieceAttackless[256] = {0};
        // 首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加
        // 然后判断各方是否处于进攻状态，方法是计算各种过河棋子的数量，按照车马2炮兵1相加
        getMidgameValue(midgameValue);
        getAttackValue(redAttackValue,blackAttackValue);
        //初始化估值矩阵
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                //将兵
                const int vlKingOrPawn = (kingPawnMidgameAttacking[pos] * midgameValue + kingPawnEndgameAttacking[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(king)][pos] = vlBlackBoard[vlIndex(king)][xyMirrorPos(pos)] = vlKingOrPawn;
                pawnPieceAttacking[pos] = vlKingOrPawn;
                pawnPieceAttackless[pos] = (kingPawnMidgameAttackless[pos] * midgameValue + kingPawnEndgameAttackless[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                //马
                const int vlKnight = (knightMidgame[pos] * midgameValue + knightEndgame[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(knight)][pos] = vlBlackBoard[vlIndex(knight)][xyMirrorPos(pos)] = vlKnight;
                //炮
                const int vlCannon = (cannonMidgame[pos] * midgameValue + cannonEndgame[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(cannon)][pos] = vlBlackBoard[vlIndex(cannon)][xyMirrorPos(pos)] = vlCannon;
                //车
                const int vlRook = (rookMidgame[pos] * midgameValue + rookEndgame[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(rook)][pos] = vlBlackBoard[vlIndex(rook)][xyMirrorPos(pos)] = vlRook;
            }
        }
        //计算士的危险程度
        vlBlackAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * redAttackValue / TOTAL_ATTACK_VALUE;
        vlRedAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * blackAttackValue / TOTAL_ATTACK_VALUE;
        //完善士象兵的估值矩阵
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                //士象
                const int vlAdvisorOrBishop = advisorBishopThreatless[pos] * (TOTAL_ATTACK_VALUE - blackAttackValue) / TOTAL_ATTACK_VALUE;
                vlRedBoard[vlIndex(advisor)][pos] = vlBlackBoard[vlIndex(advisor)][xyMirrorPos(pos)] = vlAdvisorOrBishop;
                vlRedBoard[vlIndex(bishop)][pos] = vlBlackBoard[vlIndex(bishop)][xyMirrorPos(pos)] = vlAdvisorOrBishop;
                //兵
                const int vlRedPawn = (pawnPieceAttacking[pos] * redAttackValue + pawnPieceAttackless[pos] * (TOTAL_ATTACK_VALUE - redAttackValue)) / TOTAL_ATTACK_VALUE;
                const int vlBlackPawn = (pawnPieceAttacking[xyMirrorPos(pos)] * blackAttackValue + pawnPieceAttackless[xyMirrorPos(pos)] * (TOTAL_ATTACK_VALUE - blackAttackValue)) / TOTAL_ATTACK_VALUE;
                vlRedBoard[vlIndex(pawn)][pos] = vlRedPawn;
                vlBlackBoard[vlIndex(pawn)][pos] = vlBlackPawn;
            }
        }
        //计算沉底炮的威胁向量
        for (int i = 0; i < 16; i ++) {
            vlRedBottomThreat[i] = cvlBottomThreat[i] * blackAttackValue / TOTAL_ATTACK_VALUE;
            vlBlackBottomThreat[i] = cvlBottomThreat[i] * redAttackValue / TOTAL_ATTACK_VALUE;
        }
        // 调整不受威胁方少掉的仕(士)相(象)分值
        this->vlRed = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - blackAttackValue) / TOTAL_ATTACK_VALUE;
        this->vlBlack = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - redAttackValue) / TOTAL_ATTACK_VALUE;

        // 最后重新计算子力位置分
        for(int pos = 51;pos < 205;pos++){
            const int piece = board.getPieceByPos(pos);
            const int index = vlIndex(swapBasicBoard::pieceToAbsType(piece));
            if(piece > 0){
                this->vlRed += vlRedBoard[index][pos];
            }else if(piece < 0){
                this->vlBlack += vlBlackBoard[index][pos];
            }
        }
    }

private:
    static int vlIndex(int type){
        return type - 1;
    }
    void getAttackValue(int& alRed,int& alBlack){
        alRed = alBlack = 0;
        int vlRedSimple = 0;
        int vlBlackSimple = 0;
        //车马
        const int rookOrKnightPieceList[8] = {8,9,-8,-9,6,7,-6,-7};
        for(int piece : rookOrKnightPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(piece > 0){
                if(inSideBoard[pos] * piece < 0){
                    alRed += 2;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == rook){
                    vlRedSimple += 2;
                }else{
                    vlRedSimple ++;
                }
            }else{
                if(inSideBoard[pos] * piece < 0){
                    alBlack += 2;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == rook){
                    vlBlackSimple += 2;
                }else{
                    vlBlackSimple ++;
                }
            }
        }
        const int cannonOrPawnPieceList[14] = {
                 10, 11, 12, 13, 14, 15, 16,
                -10,-11,-12,-13,-14,-15,-16
        };
        for(int piece : cannonOrPawnPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(piece > 0){
                if(inSideBoard[pos] * piece < 0){
                    alRed ++;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == cannon){
                    vlRedSimple ++;
                }
            }else{
                if(inSideBoard[pos] * piece < 0){
                    alBlack ++;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == cannon){
                    vlBlackSimple ++;
                }
            }
        }
        // 如果本方轻子数比对方多，那么每多一个轻子(车算2个轻子)威胁值加2。威胁值最多不超过8
        if(vlRedSimple > vlBlackSimple){
            alRed += abs(vlRedSimple - vlBlackSimple) * 2;
        }else if(vlBlackSimple > vlRedSimple){
            alBlack += abs(vlRedSimple - vlBlackSimple) * 2;
        }
        alRed = min(alRed,TOTAL_ATTACK_VALUE);
        alBlack = min(alBlack,TOTAL_ATTACK_VALUE);
    }
    void getMidgameValue(int& vl){
        //车
        const int rookPieceList[4] = {8,9,-8,-9};
        for(int piece : rookPieceList){
            if(position::swapBoard.getPosByPiece(piece)){
                vl += ROOK_MIDGAME_VALUE;
            }
        }
        //马炮
        const int knightOrCannonPieceList[8] = {6,7,-6,-7,10,11,-10,-11};
        for(int piece : knightOrCannonPieceList){
            if(position::swapBoard.getPosByPiece(piece)){
                vl += KNIGHT_CANNON_MIDGAME_VALUE;
            }
        }
        //其他
        const int OtherPieceList[18] = {
                 2, 3, 4, 5, 12, 13, 14, 15, 16,
                -2,-3,-4,-5,-12,-13,-14,-15,-16
        };
        for(int piece : OtherPieceList){
            if(position::swapBoard.getPosByPiece(piece)){
                vl += OTHER_MIDGAME_VALUE;
            }
        }
        vl = (2 * TOTAL_MIDGAME_VALUE - vl) * vl / TOTAL_MIDGAME_VALUE;
    }
private:
    int vlRed{};
    int vlBlack{};
    int vlFirstGo{};                  //先行权分值
    int vlRedBoard[7][256]{};         //红方估值矩阵
    int vlBlackBoard[7][256]{};       //黑方估值矩阵
    int vlRedAdvisorLeakage{};        //红士的危险值
    int vlBlackAdvisorLeakage{};      //黑士的危险值
    int vlRedBottomThreat[16]{};          //黑底炮的威胁向量
    int vlBlackBottomThreat[16]{};          //红底炮的威胁向量
    friend class test;
};
