#pragma once
#include "genMove.hpp"

// 常数表"inKnightEdgeSquares"给定了不利于马的位置，处于棋盘边缘和两个花心位置的马都是坏马
static const int inKnightEdge[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 中间子和被牵制子的距离越近，牵制的价值就越大
static const int stringValueTab[512] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
        12, 16, 20, 24, 28, 32, 36,  0, 36, 32, 28, 24, 20, 16, 12,  0,
        0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0
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
                this->vlRed -= vlRedBoard[toIndex][toPos];
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
                this->vlRed += vlRedBoard[toIndex][toPos];
            }else{
                this->vlBlack += vlBlackBoard[toIndex][toPos];
            }
        }

        position::unMakeMove(fromPos,toPos,fromPiece,toPiece);
    }
protected:
    //更新估值矩阵
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
    //车的机动性
    int rookMobility(int side){
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        const int stepList[4] = {1,1,16,16};
        int vlRedMobility = 0;
        int vlBlackMobility = 0;
        for(int piece : redRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int i = 0;i < 4;i++){
                    const int targetPos = position::bitBoard.getRayTargetPos(pos,targetPool[i],0);
                    if(targetPos > -1){
                        vlRedMobility += abs(targetPos - pos) / stepList[i];
                        if(position::board.getPieceByPos(targetPos) * piece > 0){
                            vlRedMobility--;
                        }
                    }
                }
            }
        }
        for(int piece : blackRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int i = 0;i < 4;i++){
                    const int targetPos = position::bitBoard.getRayTargetPos(pos,targetPool[i],0);
                    if(targetPos > -1){
                        vlBlackMobility += abs(targetPos - pos) / stepList[i];
                        if(position::board.getPieceByPos(targetPos) * piece > 0){
                            vlBlackMobility--;
                        }
                    }
                }
            }
        }
        if(side == red){
            return (vlRedMobility - vlBlackMobility) / 2;
        }
        return (vlBlackMobility - vlRedMobility) / 2;
    }
    //劣马
    int knightTrap(int side){
        int vlRedKnightTrap = 0;
        int vlBlackKnightTrap = 0;
        for(int piece : redKnightPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            int redPenalty = 10;
            if(pos){
                for(int step : knightDelta){
                    const int toPos = pos + step;
                    const int legPos = getKnightLeg(pos,toPos);
                    if(!inKnightEdge[toPos] &&
                        !position::board.getPieceByPos(legPos) &&
                        inBoard[toPos]){
                        redPenalty -= 5;
                        if(!redPenalty){
                            break;
                        }
                    }
                }
            }
            vlRedKnightTrap -= redPenalty;
        }
        for(int piece : blackKnightPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            int blackPenalty = 10;
            if(pos){
                for(int step : knightDelta){
                    const int toPos = pos + step;
                    const int legPos = getKnightLeg(pos,toPos);
                    if(!inKnightEdge[toPos] &&
                       !position::board.getPieceByPos(legPos) &&
                       inBoard[toPos]){
                        blackPenalty -= 5;
                        if(!blackPenalty){
                            break;
                        }
                    }
                }
            }
            vlBlackKnightTrap -= blackPenalty;
        }
        if(side == red){
            return vlRedKnightTrap - vlBlackKnightTrap;
        }
        return vlBlackKnightTrap - vlRedKnightTrap;
    }
    //士形
    int advisorShape(int side){

    }
    //牵制
    int stringHold(int side){
        int vlRedStringHold = 0;
        int vlBlackStringHold = 0;
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        //车
        for(int piece : redRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int target : targetPool){
                    vlRedStringHold += singleStringHold(pos,target);
                }
            }
        }
        for(int piece : blackRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int target : targetPool){
                    vlBlackStringHold += singleStringHold(pos,target);
                }
            }
        }
        //将帅
        int pos = position::swapBoard.getPosByPiece(redKingPiece);
        if(pos){
            for(int target : targetPool){
                vlRedStringHold += singleStringHold(pos,target);
            }
        }
        pos = position::swapBoard.getPosByPiece(blackKingPiece);
        if(pos){
            for(int target : targetPool){
                vlBlackStringHold += singleStringHold(pos,target);
            }
        }
        if(side == red){
            return vlRedStringHold - vlBlackStringHold;
        }
        return vlBlackStringHold - vlRedStringHold;
    }
    int singleStringHold(int pos,int target){
        int vlSingleStringHold = 0;
        const int toPos = position::bitBoard.getRayTargetPos(pos,target,0);
        const int toInvPos = position::bitBoard.getRayTargetPos(pos,target,1);
        if(toPos > -1){
            const int toPiece = position::board.getPieceByPos(toPos);
            const int toType = swapBasicBoard::pieceToAbsType(toPiece);
            if(toType == cannon){
                if(toInvPos > -1){
                    const int toInvPiece = position::board.getPieceByPos(toInvPos);
                    const int toInvType = swapBasicBoard::pieceToAbsType(toInvPiece);
                    if(toInvType == rook){
                        if(!genMove::getRelation(*this,toPos,beProtected,pos)){
                            vlSingleStringHold -= stringValueTab[256 + toPos - pos];
                        }
                    }
                }
            }else if(toType == knight){
                if(toInvPos > -1){
                    //受到车的牵制
                    const int toInvPiece = position::board.getPieceByPos(toInvPos);
                    const int toInvType = swapBasicBoard::pieceToAbsType(toInvPiece);
                    if(toInvType == rook){
                        if(!genMove::getRelation(*this,toPos,beProtected,pos)){
                            vlSingleStringHold -= stringValueTab[256 + toPos - pos];
                        }
                    }
                    //受到炮的牵制
                    const int toInvInvPos = position::bitBoard.getRayTargetPos(pos,target,2);
                    const int toInvInvPiece = position::board.getPieceByPos(toInvInvPos);
                    const int toInvInvType = swapBasicBoard::pieceToAbsType(toInvInvPiece);
                    if(toInvInvType == cannon){
                        if(!genMove::getRelation(*this,toPos,beProtected,pos)){
                            vlSingleStringHold -= stringValueTab[256 + toPos - pos];
                        }
                    }
                }
            }
        }
        return vlSingleStringHold;
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