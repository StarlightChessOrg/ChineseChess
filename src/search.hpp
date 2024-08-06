#pragma once
#include "evaluate.hpp"
#include "cache.hpp"

static const int MAX_MOVE_NUM = 1024;
static const int MIN_VALUE = -32768;
static const int MAX_VALUE = 32768;
static const int MIN_BAN_VALUE = MIN_VALUE + MAX_MOVE_NUM;
static const int MAX_BAN_VALUE = MAX_VALUE - MAX_MOVE_NUM;
static const int SORT_MAX_VALUE = 65536;
static const int DRAW_VALUE = 20;
static const int MAX_QUIESC_DISTANCE = 64;
static const int NULL_DEPTH = 2;

enum moveType{
    hashMove = 4,
    justEatMove = 3,
    killerMove = 2,
    historyMove = 1
};

class moveSort{
public:
    static void sortNormalMoveSeuqance(evaluate& e,historyCache& h,vector<step>& moveList){
        for(step& move : moveList){
            if(move.toPiece){
                move.sortType = justEatMove;
                move.vl = getMvvLva(e,move);
            }else{
                move.sortType = historyMove;
                move.vl = h.getCache(move);
            }
        }
        sort(moveList.begin(),moveList.end(), vlAndTypeCompare);
    }
    static void sortQuesicMoveSequance(evaluate& e,vector<step>& moveList){
        for(step& move : moveList){
            move.vl = getMvvLva(e,move);
        }
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
    static void initSortRootMoveSeuqance(evaluate& e,historyCache& h,vector<step>& moveList){
        for(step& move : moveList){
            move.vl = getMvvLva(e,move);
        }
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
    static void sortRootMoveSequance(vector<step>& moveList,step& betterMove){
        for(step& move : moveList){
            if(move == betterMove){
                move.vl = SORT_MAX_VALUE;
            }else if(move.vl > SORT_MAX_VALUE / 2){
                move.vl--;
            }else{
                move.vl = 0;
            }
        }
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
private:
    static bool inOtherStepList(step& move,vector<step>& moveList){
        for(int i = 0;i < moveList.size();i++){
            if(move == moveList[i]){
                return i + 1;
            }
        }
        return 0;
    }
    static int getMvvLva(evaluate& e,step& move){
        const int mvv = vlMvvLva[abs(move.fromPiece) - 1];
        const int lva = genMove::getRelation(e,move.fromPos,beProtected) ? vlMvvLva[abs(move.toPiece) - 1] : 0;
        if(mvv >= lva){
            return mvv - lva + 1;
        }else if(lva >= 4 || (inRiver[move.toPos] && lva == 1)){
            return 1;
        }
        return 0;
    }
    static bool vlAndTypeCompare(const step& firstMove,const step& secondMove){
        if(firstMove.sortType != secondMove.sortType) {
            return firstMove.sortType > secondMove.sortType;
        }
        return firstMove.vl > secondMove.vl;
    }
    static bool vlCompare(const step& firstMove,const step& secondMove){
        return firstMove.vl > secondMove.vl;
    }
    friend class searchGroup;
};

class searchGroup{
public:
    searchGroup(){
        pvLine = vector<int>(0,MAX_MOVE_NUM);
    }
public:
    static int searchQuesic(evaluate& e,int vlAlpha,int vlBeta) {
        int vl = harmlessPruning(e,vlBeta);
        if(vl > MIN_VALUE){
            return vl;
        }

        if(e.getNowDistance() >= MAX_QUIESC_DISTANCE){
            return e.getEvaluate(e.side,vlAlpha,vlBeta);
        }

        int vlBest = MIN_VALUE;
        vector<step> moveList;
        const bool bCheck = e.checkMoveStatus.back();
        if(bCheck){
            genMove::genMoveList(e,moveList,all);
        }else{
            vl = e.getEvaluate(e.side,vlAlpha,vlBeta);
            if(vl >= vlBeta){
                return vl;
            }
            vlBest = vl;
            vlAlpha = max(vl,vlAlpha);

            genMove::genMoveList(e, moveList,justEat);
            moveSort::sortQuesicMoveSequance(e,moveList);
        }
        for(step& move : moveList){
            if(!bCheck && !move.vl){
                break;
            }
            if(e.makeMove(move.fromPos,move.toPos)){
                vl = -searchQuesic(e,-vlBeta,-vlAlpha);
                e.unMakeMove();

                if(vl > vlBest){
                    if(vl >= vlBeta){
                        return vl;
                    }
                    vlBest = vl;
                    vlAlpha = max(vl,vlAlpha);
                }
            }
        }
        if(vlBest == MIN_VALUE){
            return MIN_VALUE + e.getNowDistance();
        }
        return vlBest;
    }
    int searchPV(evaluate& e,int depth,int vlAlpha,int vlBeta){
        if(depth <= 0){
            return searchQuesic(e,vlAlpha,vlBeta);
        }

        int vl = harmlessPruning(e,vlBeta);
        int vlBest = MIN_VALUE;
        vector<step> moveList;
        step* pBestMove = nullptr;
        const bool bCheck = !e.checkMoveStatus.empty() && e.checkMoveStatus.back();
        if(vl > MIN_VALUE){
            return vl;
        }
        bool quit = false;

        //截断启发
        vector<step> killerMoveList;
        if(!quit){
            killerMap.getCache(e,killerMoveList);
            for(step & move : killerMoveList){
                const int newDepth = bCheck ? depth : depth - 1;
                if(e.makeMove(move.fromPos,move.toPos)){
                    if(vlBest == MIN_VALUE){
                        vl = -searchPV(e, newDepth,-vlBeta,-vlAlpha);
                    }else{
                        vl = -searchNonPV(e,newDepth,-vlAlpha);
                        if(vl > vlAlpha && vl < vlBeta){
                            vl = -searchPV(e,newDepth,-vlBeta,-vlAlpha);
                        }
                    }
                    e.unMakeMove();

                    if(vl > vlBest){
                        vlBest = vl;
                        if(vl >= vlBeta){
                            pBestMove = &move;
                            quit = true;
                            break;
                        }
                        if(vl > vlAlpha){
                            vlAlpha = vl;
                        }
                    }
                }
            }
        }

        //常规搜索
        if(!quit){
            genMove::genMoveList(e,moveList,all);
            moveSort::sortNormalMoveSeuqance(e,historyMap,moveList);
            for(step & move : moveList){
                if(!moveSort::inOtherStepList(move,killerMoveList)){
                    const int newDepth = bCheck ? depth : depth - 1;
                    if(e.makeMove(move.fromPos,move.toPos)){
                        if(vlBest == MIN_VALUE){
                            vl = -searchPV(e, newDepth,-vlBeta,-vlAlpha);
                        }else{
                            vl = -searchNonPV(e,newDepth,-vlAlpha);
                            if(vl > vlAlpha && vl < vlBeta){
                                vl = -searchPV(e,newDepth,-vlBeta,-vlAlpha);
                            }
                        }
                        e.unMakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                pBestMove = &move;
                                break;
                            }
                            if(vl > vlAlpha){
                                pBestMove = &move;
                                vlAlpha = vl;
                            }
                        }
                    }
                }
            }
        }

        if(pBestMove){
            historyMap.recoardCache(*pBestMove,depth);
            killerMap.recoardCache(e,*pBestMove);
        }
        if(vlBest == MIN_VALUE){
            return MIN_VALUE + e.getNowDistance();
        }
        return vlBest;
    }
    int searchNonPV(evaluate& e,int depth,int vlBeta,bool noNull = false){
        if(depth <= 0){
            return searchQuesic(e,vlBeta - 1,vlBeta);
        }

        int vl = harmlessPruning(e,vlBeta);
        int vlBest = MIN_VALUE;
        step* pBestMove = nullptr;
        const bool bCheck = !e.checkMoveStatus.empty() && e.checkMoveStatus.back();
        if(vl > MIN_VALUE){
            return vl;
        }

        if(!noNull && !bCheck && e.nullOkay()){
            e.makeNullMove();
            vl = -searchNonPV(e,depth - NULL_DEPTH - 1,-vlBeta + 1,true);
            e.unMakeNullMove();

            if(vl >= vlBeta){
                if(e.nullSafe()){
                    return vl;
                }else if(searchNonPV(e,depth - NULL_DEPTH,vlBeta,true) >= vlBeta){
                    return vl;
                }
            }
        }

        bool quit = false;

        vector<step> killerMoveList;
        if(!quit){
            killerMap.getCache(e,killerMoveList);
            for(step & move : killerMoveList){
                const int newDepth = bCheck ? depth : depth - 1;
                if(e.makeMove(move.fromPos,move.toPos)){
                    vl = -searchNonPV(e,newDepth,-vlBeta + 1);
                    e.unMakeMove();

                    if(vl > vlBest){
                        vlBest = vl;
                        if(vl >= vlBeta){
                            pBestMove = &move;
                            quit = true;
                            break;
                        }
                    }
                }
            }
        }

        vector<step> moveList;
        if(!quit){
            genMove::genMoveList(e,moveList,all);
            moveSort::sortNormalMoveSeuqance(e,historyMap,moveList);

            for(step & move : moveList){
                if(!moveSort::inOtherStepList(move,killerMoveList)){
                    const int newDepth = bCheck ? depth : depth - 1;
                    if(e.makeMove(move.fromPos,move.toPos)){
                        vl = -searchNonPV(e,newDepth,-vlBeta + 1);
                        e.unMakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                pBestMove = &move;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if(pBestMove){
            historyMap.recoardCache(*pBestMove,depth);
            killerMap.recoardCache(e,*pBestMove);
        }
        if(vlBest == MIN_VALUE){
            return MIN_VALUE + e.getNowDistance();
        }
        return vlBest;
    }
    int searchRoot(evaluate& e,int maxDepth){
        int vl;
        int vlBest = MIN_VALUE;
        const bool bCheck = !e.checkMoveStatus.empty() && e.checkMoveStatus.back();
        for(step & move : rootMoveList){
            const int newDepth = bCheck ? maxDepth : maxDepth - 1;
            if(e.makeMove(move.fromPos,move.toPos)){
                if(vlBest == MIN_VALUE){
                    vl = -searchPV(e, newDepth,MIN_VALUE,MAX_VALUE);
                }else{
                    vl = -searchNonPV(e,newDepth,-vlBest);
                    if(vl > vlBest){
                        vl = -searchPV(e,newDepth,MIN_VALUE,-vlBest);
                    }
                }
                e.unMakeMove();

                if(vl > vlBest){
                    vlBest = vl;
                    moveSort::sortRootMoveSequance(rootMoveList,move);
                }
            }
        }
        rootMoveList.front().printMove();
        return vlBest;
    }
    int searchMain(evaluate& e,int maxDepth,int maxTime){
        //init
        initSearch(e);
        //init para
        int vlBest = MIN_VALUE;
        //search
        for(int nMaxDepth = 1;nMaxDepth <= maxDepth;nMaxDepth++){
            int vl = searchRoot(e,nMaxDepth);
            cout<<nMaxDepth<<endl;
            if(vl > vlBest){
                vlBest = vl;
                //toDo something
            }
        }
        return vlBest;
    }
protected:
    void initSearch(evaluate& e){
        e.resetEvaBoard();
        historyMap.clearCache();
        killerMap.clearCache();
        vector<step>().swap(rootMoveList);
        genMove::genMoveList(e,rootMoveList,all);
        moveSort::initSortRootMoveSeuqance(e,historyMap,rootMoveList);
    }
private:
    static int harmlessPruning(evaluate& e,int vlBeta){
        //杀棋步数裁剪
        const int nDistance = e.getNowDistance();
        int vl = MIN_VALUE + nDistance;
        if(vl >= vlBeta){
            return vl;
        }

        //自然限着裁剪
        if(e.isDraw()){
            return 0;
        }

        //重复走法路线裁剪
        const int repType = e.isRep();
        if(repType == draw_rep){
            return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
        }else if(repType == kill_rep){
            return MAX_VALUE - nDistance;
        }else if(repType == killed_rep){
            return MIN_VALUE + nDistance;
        }

        return MIN_VALUE;
    }
private:
    historyCache historyMap;        //历史启发表
    killerCache killerMap;          //历史启发表
    vector<int> pvLine;             //主要遍历路线
    vector<step> rootMoveList;      //根节点走法表
};