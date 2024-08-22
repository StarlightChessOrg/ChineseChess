#pragma once
#include "position.hpp"

//全部走法 or 吃子走法
enum genMoveType{
    all = 0,
    justEat = -1
};

enum relationType{
    beProtected = 1,
    beThreatened = -1
};

class genMove{
public:
    static void genMoveList(position& p,vector<step>& moveList,int genType = all){
        moveList.reserve(64);
        genCannonMove(p,moveList,genType);
        genKnightMove(p,moveList,genType);
        genRookMove(p,moveList,genType);
        genBishopMove(p,moveList,genType);
        genPawnMove(p,moveList,genType);
        genAdvisorMove(p,moveList,genType);
        genKingMove(p,moveList,genType);
    }
protected:
    //将军检测
    static bool CheckedBy(position& p,int side){
        const int kingPiece = (side == red) ? redKingPiece : blackKingPiece;
        const int kingPos = p.swapBoard.getPosByPiece(kingPiece);
        return getRelation(p,kingPos,kingPiece,beThreatened);
    }
    //捉子判断
    static bool ChasedBy(position& p,step& move){
        const int fromType = swapBasicBoard::pieceToAbsType(move.fromPiece);
        if(fromType == rook){
            const int pIndex = 2 * (getX(move.fromPos) != getX(move.toPos));
            //横向移动看列方向上是否有被捉的子(马炮过河兵)，反之亦然
            const int targetPool[2] = {leftTarget + pIndex,rightTarget + pIndex};
            for(int target : targetPool){
                const int toChasePos = p.bitBoard.getRayTargetPos(move.toPos,target,0);
                if(toChasePos > -1){
                    const int toChasePiece = p.board.getPieceByPos(toChasePos);
                    const int toChaseType = swapBasicBoard::pieceToAbsType(toChasePiece);
                    if(move.fromPiece * toChasePiece < 0){
                        if((toChaseType == cannon || toChaseType == knight)){
                            return !getRelation(p,toChasePos,toChasePiece,beProtected);
                        }else if(toChaseType == pawn){
                            if(inSideBoard[toChasePos] * toChasePiece < 0){
                                return !getRelation(p,toChasePos,toChasePiece,beProtected);
                            }
                        }
                    }
                }
            }
        }else if(fromType == knight){
            for(int step : knightDelta){
                const int toChasePos = move.toPos + step;
                const int toChaseLegPos = getKnightLeg(move.toPos,toChasePos);
                if(!p.board.getPieceByPos(toChaseLegPos)){
                    const int toChasePiece = p.board.getPieceByPos(toChasePos);
                    const int toChaseType = swapBasicBoard::pieceToAbsType(toChasePiece);
                    if(move.fromPiece * toChasePiece < 0){
                        if(toChaseType == rook){
                            return true;
                        }else if((toChaseType == cannon || toChaseType == knight)){
                            return !getRelation(p,toChasePos,toChasePiece,beProtected);
                        }else if(toChaseType == pawn){
                            if(inSideBoard[toChasePos] * toChasePiece < 0){
                                return !getRelation(p,toChasePos,toChasePiece,beProtected);
                            }
                        }
                    }
                }
            }
        }else if(fromType == cannon){
            const int pIndex = 2 * (getX(move.fromPos) != getX(move.toPos));
            //横向移动看列方向上是否有被捉的子(马炮过河兵)，反之亦然
            const int targetPool[2] = {leftTarget + pIndex,rightTarget + pIndex};
            for(int target : targetPool){
                const int toChasePos = p.bitBoard.getRayTargetPos(move.toPos,target,1);
                if(toChasePos > -1){
                    const int toChasePiece = p.board.getPieceByPos(toChasePos);
                    const int toChaseType = swapBasicBoard::pieceToAbsType(toChasePiece);
                    if(move.fromPiece * toChasePiece < 0){
                        if((toChaseType == cannon || toChaseType == knight)){
                            return !getRelation(p,toChasePos,toChasePiece,beProtected);
                        }else if(toChaseType == pawn){
                            if(inSideBoard[toChasePos] * toChasePiece < 0){
                                return !getRelation(p,toChasePos,toChasePiece,beProtected);
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    //走法的合法性检查，仅用于截断启发
    static bool legalMove(position& p,step& s){
        if(!s.fromPiece || !s.fromPos){
            return false;
        }
        const int trueFromPiece = p.board.getPieceByPos(s.fromPos);
        const int trueToPiece = p.board.getPieceByPos(s.toPos);
        if(trueFromPiece != s.fromPiece || trueToPiece != s.toPiece){
            return false;
        }
        const int pieceType = swapBasicBoard::pieceToAbsType(s.fromPiece);
        if(pieceType == rook){
            return !p.bitBoard.checkLineExistBarrier(s.fromPos,s.toPos);
        }
        if(pieceType == knight){
            return !p.board.getPieceByPos(getKnightLeg(s.fromPos,s.toPos));
        }
        if(pieceType == cannon){
            const int target = bitBoard::selectTarget(s.fromPos,s.toPos);
            if(!p.bitBoard.checkLineExistBarrier(s.fromPos,s.toPos) && !s.toPiece){
                return true;
            }else if(p.bitBoard.getRayTargetPos(s.fromPos,target,1) == s.toPos && s.toPiece){
                return true;
            }
            return false;
        }
        if(pieceType == bishop){
            return !p.board.getPieceByPos(getBishopEye(s.fromPos,s.toPos));
        }
        return true;
    }
    //检查该棋子是否和其他子有联系(被攻击/被保护)
    static bool getRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        if(getRookRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        if(getKnightRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        if(getCannonRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        if(getAdvisorRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        if(getBishopRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        if(getPawnRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        if(getKingRelation(p,fromPos,fromPiece,relationType,exceptPos)){
            return true;
        }
        return false;
    }
private:
    static bool getRookRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        for(int target : targetPool){
            const int targetPos = p.bitBoard.getRayTargetPos(fromPos,target,0);
            if(targetPos > -1){
                const int toPiece = p.board.getPieceByPos(targetPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == rook &&
                   toPiece * fromPiece * relationType > 0 &&
                   targetPos != exceptPos){
                    return true;
                }
            }

        }
        return false;
    }
    static bool getKnightRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        for(int step : knightCheckDelta){
            const int toPos = fromPos + step;
            const int reverseLegPos = getKnightLeg(toPos,fromPos);
            const int toPiece = p.board.getPieceByPos(toPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == knight &&
                toPiece * fromPiece * relationType > 0 &&
                !p.board.getPieceByPos(reverseLegPos) &&
                toPos != exceptPos){
                return true;
            }
        }
        return false;
    }
    static bool getCannonRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        for(int target : targetPool){
            const int targetPos = p.bitBoard.getRayTargetPos(fromPos,target,1);
            if(targetPos > -1){
                const int toPiece = p.board.getPieceByPos(targetPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == cannon &&
                   toPiece * fromPiece * relationType > 0 &&
                   targetPos != exceptPos){
                    return true;
                }
            }
        }
        return false;
    }
    static bool getPawnRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        if(relationType == beThreatened){
            const int stepList[3] = {1,-1,16 * (-p.side) * relationType};
            for(int step : stepList){
                const int toPos = fromPos + step;
                const int toPiece = p.board.getPieceByPos(toPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == pawn &&
                   toPiece * fromPiece * relationType > 0 &&
                   toPos != exceptPos) {
                    if(abs(step) != 1){
                        return true;
                    }else if(inSideBoard[toPos] * toPiece < 0){
                        return true;
                    }
                }
            }
        }
        return false;
    }
    static bool getAdvisorRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        if(inAdvisorLine[fromPos]){
            for(int step : advisorDelta){
                const int toPos = fromPos + step;
                const int toPiece = p.board.getPieceByPos(toPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == advisor &&
                    toPiece * fromPiece * relationType > 0 &&
                    toPos != exceptPos){
                    return true;
                }
            }
        }
        return false;
    }
    static bool getBishopRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        if(inBishopLine[fromPos]){
            for(int step : bishopDelta){
                const int toPos = fromPos + step;
                const int toPiece = p.board.getPieceByPos(toPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == bishop &&
                    toPiece * fromPiece * relationType > 0 &&
                    toPos != exceptPos){
                    return true;
                }
            }
        }
        return false;
    }
    static bool getKingRelation(position& p,int fromPos,int fromPiece,int relationType,int exceptPos = 0){
        if(relationType == beThreatened && swapBasicBoard::pieceToAbsType(fromPiece) == king){
            const int toPos = p.swapBoard.getPosByPiece(-fromPiece);
            if(getX(fromPos) == getX(toPos) &&
                !p.bitBoard.checkLineExistBarrier(fromPos,toPos) &&
                toPos != exceptPos){
                return true;
            }
        }

        if(inFort[fromPos]){
            for(int step : rayDelta){
                const int toPos = fromPos + step;
                const int toPiece = p.board.getPieceByPos(toPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == king &&
                    toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                    toPos != exceptPos){
                    return true;
                }
            }
        }

        return false;
    }
private:
    static void genPawnMove(position& p,vector<step>& moveList,int genType = all){
        const int *pawnPieceList = (p.side == red) ? redPawnPieceList : blackPawnPieceList;
        vector<preStep> *pMoveMatrix = (p.side == red) ? preGenMoveInstance.redPawnPreMoveList : preGenMoveInstance.blackPawnPreMoveList;
        assert(pMoveMatrix);
        for(int i = 0;i < 5;i++){
            const int pawnPiece = pawnPieceList[i];
            const int pawnPos = p.swapBoard.getPosByPiece(pawnPiece);
            if(pawnPos){
                for(preStep& preMove : pMoveMatrix[pawnPos]){
                    const int toPiece = p.board.getPieceByPos(preMove.toPos);
                    if(toPiece * pawnPiece <= genType){
                        moveList.emplace_back(pawnPos,preMove.toPos,pawnPiece,toPiece);
                    }
                }
            }
        }
    }
    static void genAdvisorMove(position& p,vector<step>& moveList,int genType = all){
        const int *advisorPieceList  = (p.side == red) ? redAdvisorPieceList : blackAdvisorPieceList;
        for(int i = 0;i < 2;i++){
            const int advisorPiece = advisorPieceList[i];
            const int advisorPos = p.swapBoard.getPosByPiece(advisorPiece);
            if(advisorPos){
                for(preStep& preMove : preGenMoveInstance.advisorPreMoveList[advisorPos]){
                    const int toPiece = p.board.getPieceByPos(preMove.toPos);
                    if(toPiece * advisorPiece <= genType){
                        moveList.emplace_back(advisorPos,preMove.toPos,advisorPiece,toPiece);
                    }
                }
            }
        }
    }
    static void genKingMove(position& p,vector<step>& moveList,int genType = all){
        const int kingPiece = (p.side == red) ? redKingPiece : blackKingPiece;
        const int kingPos = p.swapBoard.getPosByPiece(kingPiece);
        assert(kingPos);
        for (preStep& preMove: preGenMoveInstance.kingPreMoveList[kingPos]) {
            const int toPiece = p.board.getPieceByPos(preMove.toPos);
            if (toPiece * kingPiece <= genType) {
                moveList.emplace_back(kingPos, preMove.toPos, kingPiece, toPiece);
            }
        }
    }
    static void genBishopMove(position& p,vector<step>& moveList,int genType = all){
        const int *bishopPieceList = (p.side == red) ? redBishopPieceList : blackBishopPieceList;
        for(int i = 0;i < 2;i++){
            const int bishopPiece = bishopPieceList[i];
            const int bishopPos = p.swapBoard.getPosByPiece(bishopPiece);
            if(bishopPos){
                for(preStep& preMove : preGenMoveInstance.bishopPreMoveList[bishopPos]){
                    const int toPiece = p.board.getPieceByPos(preMove.toPos);
                    assert(preMove.barrierPos);
                    if (toPiece * bishopPiece <= genType && !p.board.getPieceByPos(preMove.barrierPos)) {
                        moveList.emplace_back(bishopPos, preMove.toPos, bishopPiece, toPiece);
                    }
                }
            }
        }
    }
    static void genKnightMove(position& p,vector<step>& moveList,int genType = all){
        const int *knightPieceList = (p.side == red) ? redKnightPieceList : blackKnightPieceList;
        for(int i = 0;i < 2;i++){
            const int knightPiece = knightPieceList[i];
            const int knightPos = p.swapBoard.getPosByPiece(knightPiece);
            if(knightPos){
                for(preStep& preMove : preGenMoveInstance.knightPreMoveList[knightPos]){
                    const int toPiece = p.board.getPieceByPos(preMove.toPos);
                    assert(preMove.barrierPos);
                    if (toPiece * knightPiece <= genType && !p.board.getPieceByPos(preMove.barrierPos)) {
                        moveList.emplace_back(knightPos, preMove.toPos,knightPiece, toPiece);
                    }
                }
            }
        }
    }
    static void genRookMove(position& p,vector<step>& moveList,int genType = all){
        const int *rookPieceList = (p.side == red) ? redRookPieceList : blackRookPieceList;
        for(int i = 0;i < 2;i++){
            const int rookPiece = rookPieceList[i];
            const int rookPos = p.swapBoard.getPosByPiece(rookPiece);
            if(rookPos){
                const int stepList[4] = {-1,1,-16,16};
                const int targetList[4] = {leftTarget,rightTarget,upTarget,downTarget};
                for(int a = 0; a < 4; a++){
                    const int mayEatToPos = p.bitBoard.getRayTargetPos(rookPos, targetList[a], 0);
                    //p.bitBoard.printBitBoard();
                    if(mayEatToPos > -1){
                        const int toPiece = p.board.getPieceByPos(mayEatToPos);
                        if(rookPiece * toPiece < 0){
                            moveList.emplace_back(rookPos,mayEatToPos,rookPiece,toPiece);
                        }
                    }
                    if(genType != justEat){
                        for(int toPos = rookPos + stepList[a]; inBoard[toPos] && toPos != mayEatToPos; toPos += stepList[a]){
                            moveList.emplace_back(rookPos,toPos,rookPiece,empty);
                        }
                    }
                }
            }
        }
    }
    static void genCannonMove(position& p,vector<step>& moveList,int genType = all){
        const int *cannonPieceList = (p.side == red) ? redCannonPieceList : blackCannonPieceList;
        for(int i = 0;i < 2;i++){
            const int cannonPiece = cannonPieceList[i];
            const int cannonPos = p.swapBoard.getPosByPiece(cannonPiece);
            if(cannonPos){
                const int stepList[4] = {-1,1,-16,16};
                const int targetList[4] = {leftTarget,rightTarget,upTarget,downTarget};
                for(int a = 0;a < 4;a++){
                    const int mayEatToPos = p.bitBoard.getRayTargetPos(cannonPos,targetList[a],1);
                    if(mayEatToPos > -1){
                        const int toPiece = p.board.getPieceByPos(mayEatToPos);
                        if(cannonPiece * toPiece < 0){
                            moveList.emplace_back(cannonPos,mayEatToPos,cannonPiece,toPiece);
                        }
                    }
                    if(genType != justEat){
                        const int mayGoToPos = p.bitBoard.getRayTargetPos(cannonPos,targetList[a],0);
                        for(int toPos = cannonPos + stepList[a];inBoard[toPos] && toPos != mayGoToPos;toPos += stepList[a]){
                            moveList.emplace_back(cannonPos,toPos,cannonPiece,empty);
                        }
                    }
                }
            }
        }
    }
private:
    friend class test;
    friend class evaluate;
    friend class moveSort;
    friend class killerCache;
    friend class searchGroup;
};



