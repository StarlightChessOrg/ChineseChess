#pragma once
#include "genMove.hpp"

enum repStatus{
    none_rep = 0,
    draw_rep = 1,
    kill_rep = 2,
    killed_rep = 3
};

class evaluate : public position{
public:
    explicit evaluate(const int anotherBoard[256] = initGameBoard, int initSide = red) : position(anotherBoard,initSide){
        clearEvaBoard();
        drawMoveStatus.reserve(MAX_MOVE_NUM);
        checkMoveStatus.reserve(MAX_MOVE_NUM);
        chaseMoveStatus.reserve(MAX_MOVE_NUM);
        moveRoad.reserve(MAX_MOVE_NUM);
        firstHashKeyRoad.reserve(MAX_MOVE_NUM);
        seconHashdKeyRoad.reserve(MAX_MOVE_NUM);
        initEvaluate(anotherBoard,initSide);
    }
    void initEvaluate(const int anotherBoard[256] = initGameBoard, int initSide = red){
        clearEvaBoard();
        memset(miniHashCache,(uint64)0,sizeof(uint64) * 4096);
        hashKeyResource.initHashKey();
        hashKeyResource.entireKey(*this,this->firstHashKey,this->secondHashKey);
        position::initPosition(anotherBoard,initSide);
        initSimpleValue();
    }
    void initSimpleValue(){
        vlSimpleAttackRed = vlSimpleAttackBlack = 0;
        vlSimpleDefenseRed = vlSimpleDefenseBlack = 0;
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                const int piece = board.getPieceByPos(pos);
                if(piece){
                    const int type = swapBasicBoard::pieceToAbsType(piece);
                    const int vlSimplePiece = vlSimple[type - 1];
                    if(type != king){
                        if(piece > 0){
                            if(vlSimplePiece >= 9){
                                vlSimpleAttackRed += vlSimplePiece;
                            }else{
                                vlSimpleDefenseRed += vlSimplePiece;
                            }
                        }else{
                            if(vlSimplePiece >= 9){
                                vlSimpleAttackBlack += vlSimplePiece;
                            }else{
                                vlSimpleDefenseBlack += vlSimplePiece;
                            }
                        }
                    }
                }
            }
        }
    }
    void clearEvaBoard(){
        drawMoveStatus.clear();
        checkMoveStatus.clear();
        chaseMoveStatus.clear();
        moveRoad.clear();
    }
    bool makeNullMove(){
        if(genMove::Check(*this,position::side)){
            return false;
        }
        changeSide();
        assert(!drawMoveStatus.empty());
        drawMoveStatus.push_back(drawMoveStatus.back() + 1);
        return true;
    }
    void unMakeNullMove(){
        changeSide();
        assert(!drawMoveStatus.empty());
        drawMoveStatus.pop_back();
    }
    bool makeMove(int fromPos,int toPos) {
        const int fromPiece = position::board.getPieceByPos(fromPos);
        const int toPiece = position::board.getPieceByPos(toPos);
        assert(abs(toPiece) != king);
        //检查将军
        position::makeMove(fromPos,toPos);
        if(genMove::Check(*this,-position::side)){
            position::unMakeMove(fromPos,toPos,fromPiece,toPiece);
            return false;
        }
        if(toPiece){
            const int vlToSimple = vlSimple[abs(toPiece) - 1];
            assert(vlToSimple <= 20);
            if(toPiece > 0){
                if(vlToSimple >= 9){
                    vlSimpleAttackRed -= vlToSimple;
                }else{
                    vlSimpleDefenseRed -= vlToSimple;
                }
            }else{
                if(vlToSimple >= 9){
                    vlSimpleAttackBlack -= vlToSimple;
                }else{
                    vlSimpleDefenseBlack -= vlToSimple;
                }
            }
        }
        //记录迷你置换表
        if(!drawMoveStatus.empty() && !miniHashCache[firstHashKey & 4095]){
            miniHashCache[firstHashKey & 4095] = drawMoveStatus.back();
        }
        //步进哈希键
        firstHashKeyRoad.push_back(firstHashKey);
        seconHashdKeyRoad.push_back(secondHashKey);
        hashKeyResource.stepKey(firstHashKey,secondHashKey,step(fromPos,toPos,fromPiece,toPiece));
        //优先记录走法
        moveRoad.emplace_back(fromPos,toPos,fromPiece,toPiece);
        //判断对方是否被我方将军和捉子
        const bool chase = genMove::Chase(*this,moveRoad.back());
        const bool check = genMove::Check(*this,position::side);
        const int lastDrawMoveSum = drawMoveStatus.empty() ? 0 : drawMoveStatus.back();
        if(check || (!checkMoveStatus.empty() && checkMoveStatus.back())){
            drawMoveStatus.push_back(lastDrawMoveSum);
        }else if(toPiece){
            drawMoveStatus.push_back(0);
        }else {
            drawMoveStatus.push_back(lastDrawMoveSum + 1);
        }
        //记录剩余棋规状态
        checkMoveStatus.push_back(check);
        chaseMoveStatus.push_back(chase);
        return true;
    }
    void unMakeMove() {
        const int fromPos = moveRoad.back().fromPos;
        const int toPos = moveRoad.back().toPos;
        const int fromPiece = moveRoad.back().fromPiece;
        const int toPiece = moveRoad.back().toPiece;
        if(toPiece){
            const int vlToSimple = vlSimple[abs(toPiece) - 1];
            assert(vlToSimple <= 20);
            if(toPiece > 0){
                if(vlToSimple >= 9){
                    vlSimpleAttackRed += vlToSimple;
                }else{
                    vlSimpleDefenseRed += vlToSimple;
                }
            }else{
                if(vlToSimple >= 9){
                    vlSimpleAttackBlack += vlToSimple;
                }else{
                    vlSimpleDefenseBlack += vlToSimple;
                }
            }
        }
        //步进棋盘
        position::unMakeMove(fromPos,toPos,fromPiece,toPiece);
        //撤销迷你置换表
        if(!drawMoveStatus.empty() && miniHashCache[firstHashKey & 4095] == drawMoveStatus.back()){
            miniHashCache[firstHashKey & 4095] = 0;
        }
        //步进哈希键
        assert(!firstHashKeyRoad.empty() && !seconHashdKeyRoad.empty());
        firstHashKey = firstHashKeyRoad.back();
        secondHashKey = seconHashdKeyRoad.back();
        firstHashKeyRoad.pop_back();
        seconHashdKeyRoad.pop_back();
        //撤销棋规记录
        assert(!moveRoad.empty());
        drawMoveStatus.pop_back();
        checkMoveStatus.pop_back();
        chaseMoveStatus.pop_back();
        moveRoad.pop_back();
    }
protected:
    bool nullOkay(){
        return ((side == red) ? (vlSimpleAttackRed + vlSimpleDefenseRed) : (vlSimpleAttackBlack + vlSimpleDefenseBlack)) >= 40;
    }
    bool nullTry(){
        return ((side == red) ? (vlSimpleAttackRed + vlSimpleDefenseRed) : (vlSimpleAttackBlack + vlSimpleDefenseBlack)) >= 20;
    }
protected:
    //判断是否自然和棋
    bool isDraw(){
        if(!vlSimpleAttackRed && !vlSimpleAttackBlack){
            return true;
        }
        return !drawMoveStatus.empty() && drawMoveStatus.back() >= 100;
    }
    //获取和棋值
    int getDrawValue(){
        return (getNowDistance() & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
    }
    //判断局面是否稳定(当前局面存在重复)
    bool Stable(){
        return !miniHashCache[firstHashKey & 4095];
    }
    //判断走法线路的重复类型
    bool isRep(){
        if(!miniHashCache[firstHashKey & 4095]){
            return none_rep;
        }
        int iRepLevel = 0;
        int otherRepLevel = 0;
        for(int i = (int)moveRoad.size() - 1,virtualSide = side;i >= 0;i--,virtualSide = -virtualSide){
            const step& nowMove = moveRoad[i];
            if(nowMove.toPiece){
                return none_rep;
            }
            if (swapBasicBoard::pieceToAbsType(nowMove.fromPiece) == pawn){
                if(abs(nowMove.fromPos - nowMove.toPos) == 16){
                    return none_rep;
                }
            }
            const int Check = checkMoveStatus[i];
            const int Chase = chaseMoveStatus[i];
            if(virtualSide == side){
                iRepLevel += (Check << 1) + Chase;
            }else{
                otherRepLevel += (Check << 1) + Chase;
            }

            if(firstHashKeyRoad[i] == firstHashKey){
                if(iRepLevel > otherRepLevel){
                    return killed_rep;
                }else if(iRepLevel == otherRepLevel){
                    return draw_rep;
                }else{
                    return kill_rep;
                }
            }
        }
        return none_rep;
    }
    //获取路线长度
    int getNowDistance(){
        return (int)moveRoad.size();
    }
protected:
    vector<int> drawMoveStatus;             //走法路线对应的和棋走法数
    vector<bool> checkMoveStatus;           //走法路线对应的将军状态
    vector<bool> chaseMoveStatus;           //走法瑞安对应的捉子状态
    vector<step> moveRoad;                  //走法路线
    vector<uint64> firstHashKeyRoad;        //第一哈希键路线
    vector<uint64> seconHashdKeyRoad;       //第二哈希键路线
    uint64 miniHashCache[4096];             //长将和长捉专用迷你置换表
    hashKey hashKeyResource;                //哈希键公共资源
    int vlSimpleAttackRed;                  //红方简单进攻分值
    int vlSimpleAttackBlack;                //黑方简单进攻分值
    int vlSimpleDefenseRed;                 //红方简单防守分值
    int vlSimpleDefenseBlack;               //黑方简单防守分值
private:
    friend class test;
    friend class nnue;
    friend class moveSort;
    friend class killerCache;
    friend class hashCache;
    friend class searchGroup;
    friend class searchManager;
    friend class pipe;
};