#pragma once
#include "position.hpp"

class tinyMove{
public:
    tinyMove(){
        fromPos = toPos = 0;
        fromPiece = toPiece = 0;
    }
    tinyMove(uint8 fromPos,uint8 toPos,int8 fromPiece,int8 toPiece){
        this->fromPos = fromPos;
        this->toPos = toPos;
        this->fromPiece = fromPiece;
        this->toPiece = toPiece;
    }
    uint8 fromPos;
    uint8 toPos;
    int8 fromPiece;
    int8 toPiece;
    friend class hashItem;
    friend class position;
    friend class evaluate;
    friend class searchGroup;
};


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
        this->fromPos = (uint8)fromPos;
        this->toPos = (uint8)toPos;
        this->fromPiece = (int8)fromPiece;
        this->toPiece = (int8)toPiece;
        this->vl = vl;
        sortType = 0;
    }
    void printMove() const{
        cout<<setw(3)<<(int)fromPiece<<" from "<<(int)fromPos<<" to "<<(int)toPos <<" and eat "<<(int)toPiece<<" with vl = "<<vl<<endl;
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
    uint8 fromPos;
    uint8 toPos;
    int8 fromPiece;
    int8 toPiece;
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
            const int toPiece = p.board.getPieceByPos(targetPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == rook &&
                toPiece * fromPiece * relationType > 0 &&
                targetPos != exceptPos){
                return true;
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
            const int toPiece = p.board.getPieceByPos(targetPos);
            if(swapBasicBoard::pieceToAbsType(toPiece) == cannon &&
                toPiece * fromPiece * relationType > 0 &&
                targetPos != exceptPos){
                return true;
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
            if(!toPos){
                p.board.printBasicBoard();
            }
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
                const int stepList[4] = {-1,1,-16,16};
                const int targetList[4] = {leftTarget,rightTarget,upTarget,downTarget};
                for(int a = 0; a < 4; a++){
                    const int mayEatToPos = p.bitBoard.getRayTargetPos(rookPos, targetList[a], 0);
                    if(mayEatToPos != -1){
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
                    if(mayEatToPos != -1){
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


class hashKey{
public:
    hashKey(){
        initHashKey();
    }

protected:
    void entireKey(position& p,uint64& firstkey,uint64& secondKey,uint64& playerKey){
        firstkey = secondKey = 0;
        for(int pos = 51;pos < 205;pos++){
            const int piece = p.board.getPieceByPos(pos);
            if(piece){
                int convert_type = swapBasicBoard::pieceToAbsType(piece) - 1;
                firstkey ^= keyMatrix[0][convert_type][pos];
                secondKey ^= keyMatrix[1][convert_type][pos];
            }
        }
        playerKey = getKey();
    }
    void stepKey(uint64& firstkey,uint64& secondKey,uint64 playerKey,step move){
        int from_convert_type = swapBasicBoard::pieceToAbsType(move.fromPiece) - 1;
        if(move.fromPiece < 0){
            from_convert_type += 7;
        }
//        if(from_convert_type < 0){
//            cout<<from_convert_type<<endl;
//        }
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
        playerKey ^= keyPlayer;
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
        uniform_int_distribution<uint64> u(16384,32767);
        return u(e) ^ (u(e) << 15) ^ (u(e) << 30) ^ (u(e) << 45) ^ (u(e) << 60);
    }
protected:
    default_random_engine e;
    uint64 keyMatrix[2][14][256]{};
    uint64 keyPlayer{};
    friend class searchGroup;
    friend class evaluate;
};
