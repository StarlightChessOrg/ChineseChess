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
    }
    step(int fromPos,int toPos,int fromPiece,int toPiece,int vl = 0){
        this->fromPos = fromPos;
        this->toPos = toPos;
        this->fromPiece = fromPiece;
        this->toPiece = toPiece;
        this->vl = vl;
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
public:
    int fromPos;
    int toPos;
    int fromPiece;
    int toPiece;
    int vl;
};

//全部走法 or 吃子走法
enum genMoveType{
    all = 0,
    justEat = -1
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
};