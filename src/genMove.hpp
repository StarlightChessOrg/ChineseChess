#pragma once
#include "position.hpp"

class step{
public:
    step(){
        fromPos = 0;
        toPos = 0;
        fromPiece = 0;
        toPiece = 0;
        vl = 0;
        sortType = 0;
    }
    step(int fromPos,int toPos,int fromPiece,int toPiece,int vl = 0){
        this->fromPos = fromPos;
        this->toPos = toPos;
        this->fromPiece = fromPiece;
        this->toPiece = toPiece;
        this->vl = vl;
        sortType = 0;
    }
    void printMove() const{
        cout<<setw(3)<<fromPiece<<" from "<<fromPos<<" to "<<toPos <<" and eat "<<toPiece<<" with vl = "<<vl<<endl;
     }
    static void printMoveList(vector<step>& moveList){
        cout<<"--------------------------------------"<<endl;
        for(step& s : moveList){
            s.printMove();
        }
        cout<<"total size of move list is "<<moveList.size()<<endl;
    }
    bool operator!=(const step& otherMove) const{
        return !((fromPos == otherMove.fromPos) &&
                (toPos == otherMove.toPos) &&
                (fromPiece == otherMove.fromPiece) &&
                (toPiece == otherMove.toPiece));
    }
    bool operator==(const step& otherMove) const{
        return  (fromPos == otherMove.fromPos) &&
                (toPos == otherMove.toPos) &&
                (fromPiece == otherMove.fromPiece) &&
                (toPiece == otherMove.toPiece);
    }
public:
    int fromPos;
    int toPos;
    int fromPiece;
    int toPiece;
    int vl;
    int sortType;
};

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
        return getRelation(p,kingPos,beThreatened);
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
                            return !getRelation(p,toChasePos,beProtected);
                        }else if(toChaseType == pawn){
                            if(inSideBoard[toChasePos] * toChasePiece < 0){
                                return !getRelation(p,toChasePos,beProtected);
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
                            return !getRelation(p,toChasePos,beProtected);
                        }else if(toChaseType == pawn){
                            if(inSideBoard[toChasePos] * toChasePiece < 0){
                                return !getRelation(p,toChasePos,beProtected);
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
                            return !getRelation(p,toChasePos,beProtected);
                        }else if(toChaseType == pawn){
                            if(inSideBoard[toChasePos] * toChasePiece < 0){
                                return !getRelation(p,toChasePos,beProtected);
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
    static bool getRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        if(getRookRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        if(getKnightRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        if(getCannonRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        if(getAdvisorRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        if(getBishopRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        if(getPawnRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        if(getKingRelation(p,fromPos,relationType,exceptPos)){
            return true;
        }
        return false;
    }
private:
    static bool getRookRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        for(int target : targetPool){
            const int targetPos = p.bitBoard.getRayTargetPos(fromPos,target,0);
            const int toPiece = p.board.getPieceByPos(targetPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == rook &&
                toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                targetPos != exceptPos){
                return true;
            }
        }
        return false;
    }
    static bool getKnightRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        for(int step : knightCheckDelta){
            const int toPos = fromPos + step;
            const int reverseLegPos = getKnightLeg(toPos,fromPos);
            const int toPiece = p.board.getPieceByPos(toPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == knight &&
                toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                !p.board.getPieceByPos(reverseLegPos) &&
                toPos != exceptPos){
                return true;
            }
        }
        return false;
    }
    static bool getCannonRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        for(int target : targetPool){
            const int targetPos = p.bitBoard.getRayTargetPos(fromPos,target,1);
            const int toPiece = p.board.getPieceByPos(targetPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == cannon &&
                toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                targetPos != exceptPos){
                return true;
            }
        }
        return false;
    }
    static bool getPawnRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        if(inRiver[fromPos]){
            const int step = 16 * p.side * relationType;
            const int toPos = fromPos + step;
            const int toPiece = p.board.getPieceByPos(toPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == pawn &&
                toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                toPos != exceptPos){
                return true;
            }
        }
        return false;
    }
    static bool getAdvisorRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        if(inAdvisorLine[fromPos]){
            for(int step : advisorDelta){
                const int toPos = fromPos + step;
                const int toPiece = p.board.getPieceByPos(toPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == advisor &&
                    toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                    toPos != exceptPos){
                    return true;
                }
            }
        }
        return false;
    }
    static bool getBishopRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        if(inBishopLine[fromPos]){
            for(int step : bishopDelta){
                const int toPos = fromPos + step;
                const int toPiece = p.board.getPieceByPos(toPos);
                if(swapBasicBoard::pieceToAbsType(toPiece) == bishop &&
                    toPiece * p.board.getPieceByPos(fromPos) * relationType > 0 &&
                    toPos != exceptPos){
                    return true;
                }
            }
        }
        return false;
    }
    static bool getKingRelation(position& p,int fromPos,int relationType,int exceptPos = 0){
        const int fromPiece = p.board.getPieceByPos(fromPos);
        if(relationType == beThreatened && swapBasicBoard::pieceToAbsType(fromPiece) == king){
            const int toPos = p.swapBoard.getPosByPiece(-fromPiece);
            assert(toPos);
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
        for(int i = 0;i < 5;i++){
            const int pawnPiece = pawnPieceList[i];
            const int pawnPos = p.swapBoard.getPosByPiece(pawnPiece);
            if(pawnPos){
                //判断是否在本方界限内
                if(inSideBoard[pawnPos] * pawnPiece < 0){
                    const int stepList[3] = {1,-1,16 * (-p.side)};
                    for(int t : stepList){
                        const int toPos = pawnPos + t;
                        const int toPiece = p.board.getPieceByPos(toPos);
                        if(toPiece * pawnPiece <= genType && inBoard[toPos]){
                            moveList.emplace_back(pawnPos,toPos,pawnPiece,toPiece);
                        }
                    }
                }else {
                    const int toPos = pawnPos + 16 * (-p.side);
                    const int toPiece = p.board.getPieceByPos(toPos);
                    if(toPiece * pawnPiece <= genType && inBoard[toPos]){
                        moveList.emplace_back(pawnPos,toPos,pawnPiece,toPiece);
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
                if(inFortCenter[advisorPos]){
                    for(int step : advisorDelta){
                        const int toPos = advisorPos + step;
                        const int toPiece = p.board.getPieceByPos(toPos);
                        if(toPiece * advisorPiece <= genType && inFort[toPos]){
                            moveList.emplace_back(advisorPos,toPos,advisorPiece,toPiece);
                        }
                    }
                }else{
                    const int toPos = (p.side == red) ? redFortCenterPos : blackFortCenterPos;
                    const int toPiece = p.board.getPieceByPos(toPos);
                    if(toPiece * advisorPiece <= genType && inFort[toPos]){
                        moveList.emplace_back(advisorPos,toPos,advisorPiece,toPiece);
                    }
                }
            }
        }
    }
    static void genKingMove(position& p,vector<step>& moveList,int genType = all){
        const int kingPiece = (p.side == red) ? redKingPiece : blackKingPiece;
        const int kingPos = p.swapBoard.getPosByPiece(kingPiece);
        assert(kingPos);
        for(int step : rayDelta){
            const int toPos = kingPos + step;
            const int toPiece = p.board.getPieceByPos(toPos);
            if(toPiece * kingPiece <= genType && inFort[toPos]){
                moveList.emplace_back(kingPos,toPos,kingPiece,toPiece);
            }
        }
    }
    static void genBishopMove(position& p,vector<step>& moveList,int genType = all){
        const int *bishopPieceList = (p.side == red) ? redBishopPieceList : blackBishopPieceList;
        for(int i = 0;i < 2;i++){
            const int bishopPiece = bishopPieceList[i];
            const int bishopPos = p.swapBoard.getPosByPiece(bishopPiece);
            if(bishopPos){
                for(int step : bishopDelta){
                    const int toPos = bishopPos + step;
                    const int toPiece = p.board.getPieceByPos(toPos);
                    const int eyePos = getBishopEye(bishopPos,toPos);
                    if(toPiece * bishopPiece <= genType &&
                        inSideBoard[toPos] * bishopPiece > 0 &&
                        !p.board.getPieceByPos(eyePos)){
                        moveList.emplace_back(bishopPos,toPos,bishopPiece,toPiece);
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
                for(int step : knightDelta){
                    const int toPos = knightPos + step;
                    const int toPiece = p.board.getPieceByPos(toPos);
                    const int legPos = getKnightLeg(knightPos,toPos);
                    if(toPiece * knightPiece <= genType &&
                        inBoard[toPos] &&
                        !p.board.getPieceByPos(legPos)){
                        moveList.emplace_back(knightPos,toPos,knightPiece,toPiece);
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
                for(int step : rayDelta){
                    for(int toPos = rookPos + step;inBoard[toPos];toPos += step){
                        const int toPiece = p.board.getPieceByPos(toPos);
                        if(!toPiece && genType != justEat){
                            moveList.emplace_back(rookPos,toPos,rookPiece,toPiece);
                        }else if(rookPiece * toPiece < 0){
                            moveList.emplace_back(rookPos,toPos,rookPiece,toPiece);
                            break;
                        }else if(rookPiece * toPiece > 0){
                            break;
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
                if(genType != justEat){
                    for(int step : rayDelta){
                        for(int toPos = cannonPos + step;inBoard[toPos] && !p.board.getPieceByPos(toPos);toPos += step){
                            moveList.emplace_back(cannonPos,toPos,cannonPiece,empty);
                        }
                    }
                }
                const int targetList[4] = {leftTarget,rightTarget,upTarget,downTarget};
                for(int target : targetList){
                    const int mayEatToPos = p.bitBoard.getRayTargetPos(cannonPos,target,1);
                    if(mayEatToPos != -1){
                        const int toPiece = p.board.getPieceByPos(mayEatToPos);
                        if(cannonPiece * toPiece < 0){
                            moveList.emplace_back(cannonPos,mayEatToPos,cannonPiece,toPiece);
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
};