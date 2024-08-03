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

class genMove{
public:
    static void genPawnMove(position& p,vector<step>& moveList){
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
                        if(toPiece * pawnPiece <= 0){
                            moveList.emplace_back(pawnPos,toPos,pawnPiece,toPiece);
                        }
                    }
                }else {
                    const int toPos = pawnPos + 16 * (-p.side);
                    const int toPiece = p.board.getPieceByPos(toPos);
                    if(toPiece * pawnPiece <= 0){
                        moveList.emplace_back(pawnPos,toPos,pawnPiece,toPiece);
                    }
                }
            }
        }
    }

    static void genAdvisorMove(position& p,vector<step>& moveList){
        const int* advisorPieceList  = (p.side == red) ? redAdvisorPieceList : blackAdvisorPieceList;
        for(int i = 0;i < 2;i++){
            const int advisorPiece = advisorPieceList[i];
            const int advisorPos = p.swapBoard.getPosByPiece(advisorPiece);
            if(advisorPos){
                if(inFortCenter[advisorPos]){
                    for(int step : advisorDelta){
                        const int toPos = advisorPos + step;
                        const int toPiece = p.board.getPieceByPos(toPos);
                        if(toPiece * advisorPiece <= 0 && inFort[toPos]){
                            moveList.emplace_back(advisorPos,toPos,advisorPiece,toPiece);
                        }
                    }
                }else{
                    const int toPos = (p.side == red) ? redFortCenterPos : blackFortCenterPos;
                    const int toPiece = p.board.getPieceByPos(toPos);
                    if(toPiece * advisorPiece <= 0 && inFort[toPos]){
                        moveList.emplace_back(advisorPos,toPos,advisorPiece,toPiece);
                    }
                }
            }
        }
    }

    static void genKingMove(position& p,vector<step>& moveList){
        const int kingPiece = (p.side == red) ? redKingPiece : blackKingPiece;
        const int kingPos = p.swapBoard.getPosByPiece(kingPiece);
        assert(kingPos);
        for(int step : rayDelta){
            const int toPos = kingPos + step;
            const int toPiece = p.board.getPieceByPos(toPos);
            if(toPiece * kingPiece <= 0 && inFort[toPos]){
                moveList.emplace_back(kingPos,toPos,kingPiece,toPiece);
            }
        }
    }
};