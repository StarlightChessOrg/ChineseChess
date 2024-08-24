#pragma once
#include "cache.hpp"
#include "status.hpp"
#include "nnue.hpp"

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
                move.vl = h.getCache(&move);
            }
        }
        sort(moveList.begin(),moveList.end(), vlAndTypeCompare);
    }
    static void sortQuesicMoveSequance(evaluate& e,vector<step>& moveList){
        for(step& move : moveList){
            if(move.toPiece){
                move.vl = getMvvLva(e,move);
            }
        }
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
    static void initSortRootMoveSeuqance(evaluate& e,vector<step>& moveList){
        vector<step> tempMoveList;
        for(step& move : moveList){
            if(e.makeMove(move.fromPos,move.toPos)){
                tempMoveList.push_back(move);
                e.unMakeMove();
            }
        }
        swap(tempMoveList,moveList);
        for(step& move : moveList){
            if(move.toPiece){
                move.vl = getMvvLva(e,move);
            }
        }
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
    static void refreshRootMoveSequance(vector<step>& moveList,step& betterMove){
        for(step& move : moveList){
            if(move == betterMove){
                move.vl = SORT_MAX_VALUE;
            }else if(move.vl != 0){
                move.vl--;
            }
        }
    }
    static void sortRootMoveSequance(vector<step>& moveList){
        sort(moveList.begin(),moveList.end(), vlCompare);
    }
private:
    static int inOtherStepList(step& move,vector<step>& moveList){
        for(int i = 0;i < moveList.size();i++){
            if(move == moveList[i]){
                return i + 1;
            }
        }
        return 0;
    }
    static int getMvvLva(evaluate& e,step& move){
        assert(move.toPiece);
        const int lva = vlMvvLva[abs(move.fromPiece) - 1];
        const int mvv = genMove::getRelation(e,move.toPos,move.toPiece,beProtected) ? vlMvvLva[abs(move.toPiece) - 1] : 0;
        const int toType = swapBasicBoard::pieceToAbsType(move.toPiece);
        if(mvv > lva){
            return mvv - lva + 1;
        }else if(mvv == lva && lva >= 9){
            return 1;
        }else if(inRiver[move.toPos] && toType == pawn){
            return 1;
        }
        return mvv - lva;
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
    friend class searchManager;
};


const string paraRootPath = "../linear/para";

class searchGroup{
public:
    searchGroup(const int anotherGameBoard[256] = initGameBoard,int initSide = red){
        initSearchGroup(anotherGameBoard,initSide);
    }
    void initSearchGroup(const int anotherGameBoard[256],int initSide){
        e.initEvaluate(anotherGameBoard,initSide);
        n.readPara(paraRootPath);
        n.initCache(this->e);
    }
protected:
    int getEvaluate(){
        return n.forward(this->e);
    }
    bool makeMove(step& move){
        if(e.makeMove(move.fromPos,move.toPos)){
            n.makeMove(move);
            return true;
        }
        return false;
    }
    void unmakeMove(){
        n.unmakeMove(e.moveRoad.back());
        e.unMakeMove();
    }
    void clearCache(){
        historyMap.clearCache();
        killerMap.clearCache();
        hashMap.clearCache();
    }
    void recoardCache(step *pBestMove,int nodeType,int depth,int vlRecoard){
        if(pBestMove){
            historyMap.recoardCache(pBestMove,depth);
            if(nodeType == beta){
                killerMap.recoardCache(this->e,pBestMove);
            }
            hashMap.recoardCache(this->e,nodeType,vlRecoard,depth,pBestMove);
        }
    }
protected:
    evaluate e;
    nnue n;
    historyCache historyMap;        //历史启发表
    killerCache killerMap;          //截断启发表
    hashCache hashMap;              //哈希启发表
    vector<step> rootMoveList;      //根节点走法表
    friend class searchManager;
    friend class pipe;
    friend class test;
};

class searchManager{
public:
    searchManager(const int anotherGameBoard[256] = initGameBoard,int initSide = red){
        s.initSearchGroup(anotherGameBoard,initSide);
    }
protected:
    int searchQuesic(searchGroup& s,int vlAlpha,int vlBeta) {
        evaluate& e = s.e;
        int vl = harmlessPruning(e,vlBeta);
        if(vl > MIN_VALUE){
            return vl;
        }

        if(e.getNowDistance() >= MAX_QUIESC_DISTANCE){
            return s.getEvaluate();
        }

        int vlBest = MIN_VALUE;
        vector<step> moveList;
        const bool bCheck = e.checkMoveStatus.back();
        if(bCheck){
            genMove::genMoveList(e,moveList,all);
        }else{
            vl = s.getEvaluate();
            if(vl >= vlBeta){
                return vl;
            }
            vlBest = vl;
            vlAlpha = max(vl,vlAlpha);

            genMove::genMoveList(e, moveList,justEat);
            moveSort::sortQuesicMoveSequance(e,moveList);
        }
        for(step& move : moveList){
            if(!bCheck && move.vl <= 0){
                break;
            }
            if(s.makeMove(move)){
                vl = -searchQuesic(s,-vlBeta,-vlAlpha);
                s.unmakeMove();

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
    int searchPV(searchGroup& s,int depth,int vlAlpha,int vlBeta){
        if(depth <= 0){
            return searchQuesic(s,vlAlpha,vlBeta);
        }
        evaluate& e = s.e;
        int vl = harmlessPruning(e,vlBeta);
        int vlBest = MIN_VALUE;
        int nodeType = alpha;
        vector<step> moveList;
        step* pBestMove = nullptr;
        const bool bCheck = !e.checkMoveStatus.empty() && e.checkMoveStatus.back();
        if(vl > MIN_VALUE){
            return vl;
        }
        bool quit = false;

        tinyMove tMove;
        step convert_move;
        if(s.hashMap.getCache(e,depth,vlAlpha,vlBeta,vl,tMove)){
            if(vl <= vlAlpha && searchQuesic(s,vlAlpha,vlAlpha + 1) <= vlAlpha){
                return vl;
            }else if(vl >= vlBeta && searchQuesic(s,vlBeta - 1,vlBeta) >= vlBeta){
                return vl;
            }
        }

        //置换表启发
        bool tMoveHit = false;
        if (genMove::legalMove(e, convert_move)) {
            const int newDepth = bCheck ? depth : depth - 1;
            if (s.makeMove(convert_move)) {
                vl = -searchPV(s, newDepth, -vlBeta, -vlAlpha);
                s.unmakeMove();

                if(vl > vlBest){
                    vlBest = vl;
                    if(vl >= vlBeta){
                        nodeType = beta;
                        quit = true;
                    }
                    if(vl > vlAlpha){
                        nodeType = pv;
                        vlAlpha = vl;
                    }
                }
                tMoveHit = true;
            }
        }

        //吃子搜索
        if(!quit){
            genMove::genMoveList(e,moveList,all);
            moveSort::sortNormalMoveSeuqance(e,s.historyMap,moveList);
            for(step & move : moveList){
                if(move != convert_move && move.toPiece){
                    const int newDepth = bCheck ? depth : depth - 1;
                    if(s.makeMove(move)){
                        if(vlBest == MIN_VALUE){
                            vl = -searchPV(s, newDepth,-vlBeta,-vlAlpha);
                        }else{
                            vl = -searchNonPV(s,newDepth,-vlAlpha);
                            if(vl > vlAlpha && vl < vlBeta){
                                vl = -searchPV(s,newDepth,-vlBeta,-vlAlpha);
                            }
                        }
                        s.unmakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                pBestMove = &move;
                                nodeType = beta;
                                quit = true;
                                break;
                            }
                            if(vl > vlAlpha){
                                pBestMove = &move;
                                nodeType = pv;
                                vlAlpha = vl;
                            }
                        }
                    }
                }
            }
        }

        //截断启发
        vector<step> killerMoveList;
        if(!quit){
            s.killerMap.getCache(e,killerMoveList);
            for(step & move : killerMoveList){
                if(move != convert_move && !move.toPiece){
                    const int newDepth = bCheck ? depth : depth - 1;
                    if(s.makeMove(move)){
                        if(vlBest == MIN_VALUE){
                            vl = -searchPV(s, newDepth,-vlBeta,-vlAlpha);
                        }else{
                            vl = -searchNonPV(s,newDepth,-vlAlpha);
                            if(vl > vlAlpha && vl < vlBeta){
                                vl = -searchPV(s,newDepth,-vlBeta,-vlAlpha);
                            }
                        }
                        s.unmakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                quit = true;
                                nodeType = beta;
                                break;
                            }
                            if(vl > vlAlpha){
                                vlAlpha = vl;
                                nodeType = pv;
                            }
                        }
                    }
                }
            }
        }

        //剩余走法
        if(!quit){
            for(step & move : moveList){
                if(!moveSort::inOtherStepList(move,killerMoveList) &&
                   move != convert_move &&
                   !move.toPiece){
                    const int newDepth = bCheck ? depth : depth - 1;
                    if(s.makeMove(move)){
                        if(vlBest == MIN_VALUE){
                            vl = -searchPV(s, newDepth,-vlBeta,-vlAlpha);
                        }else{
                            vl = -searchNonPV(s,newDepth,-vlAlpha);
                            if(vl > vlAlpha && vl < vlBeta){
                                vl = -searchPV(s,newDepth,-vlBeta,-vlAlpha);
                            }
                        }
                        s.unmakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                pBestMove = &move;
                                nodeType = beta;
                                break;
                            }
                            if(vl > vlAlpha){
                                pBestMove = &move;
                                nodeType = pv;
                                vlAlpha = vl;
                            }
                        }
                    }
                }
            }
        }

        s.recoardCache(pBestMove,nodeType,depth,vlBest);

        if(vlBest == MIN_VALUE){
            return MIN_VALUE + e.getNowDistance();
        }
        return vlBest;
    }
    int searchNonPV(searchGroup& s,int depth,int vlBeta,bool noNull = false){
        evaluate& e = s.e;
        if(depth <= 0){
            return searchQuesic(s,vlBeta - 1,vlBeta);
        }

        int vl = harmlessPruning(e,vlBeta);
        int vlBest = MIN_VALUE;
        step* pBestMove = nullptr;
        const bool bCheck = !e.checkMoveStatus.empty() && e.checkMoveStatus.back();
        if(vl > MIN_VALUE){
            return vl;
        }

        tinyMove tMove;
        if(s.hashMap.getCache(e,depth,vlBeta - 1,vlBeta,vl,tMove)){
            const int static_vl = searchQuesic(s,vlBeta - 1,vlBeta);
            if(vl >= vlBeta){
                if(static_vl >= vlBeta){
                    return vl;
                }
            }else if(vl < vlBeta && static_vl < vlBeta){
                return vl;
            }
        }

        //空着启发
        if(!noNull && !bCheck && e.nullTry()){
            if(e.makeNullMove()){
                vl = -searchNonPV(s,depth - NULL_DEPTH - 1,-vlBeta + 1,true);
                e.unMakeNullMove();

                if(vl >= vlBeta){
                    if(e.nullOkay()){
                        s.recoardCache(nullptr,beta,max(depth,NULL_DEPTH + 1),vlBeta);
                        return vl;
                    }else if(searchNonPV(s,depth - NULL_DEPTH,vlBeta,true) >= vlBeta){
                        s.recoardCache(nullptr,beta,max(depth,NULL_DEPTH + 1),vlBeta);
                        return vl;
                    }
                }
            }
        }

        //置换表启发
        bool quit = false;
        //bool tMoveHit = false;
        step convert_move = step(tMove.fromPos,tMove.toPos,tMove.fromPiece,tMove.toPiece);
        if (genMove::legalMove(e, convert_move)) {
            const int newDepth = bCheck ? depth : depth - 1;
            if (s.makeMove(convert_move)) {
                vl = -searchNonPV(s, newDepth, -vlBeta + 1);
                s.unmakeMove();

                if (vl > vlBest) {
                    vlBest = vl;
                    if (vl >= vlBeta) {
                        quit = true;
                    }
                }
                //tMoveHit = true;
            }
        }

        //吃子启发
        vector<step> moveList;
        if(!quit){
            genMove::genMoveList(e,moveList,all);
            moveSort::sortNormalMoveSeuqance(e,s.historyMap,moveList);
            for(step & move : moveList){
                if(move != convert_move && move.toPiece){
                    const int newDepth = bCheck ? depth : depth - 1;
                    if(s.makeMove(move)){
                        vl = -searchNonPV(s,newDepth,-vlBeta + 1);
                        s.unmakeMove();

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
        }

        //截断启发
        vector<step> killerMoveList;
        if(!quit){
            s.killerMap.getCache(e,killerMoveList);
            for(step & move : killerMoveList){
                const int newDepth = bCheck ? depth : depth - 1;
                if(move != convert_move && !move.toPiece){
                    if(s.makeMove(move)){
                        vl = -searchNonPV(s,newDepth,-vlBeta + 1);
                        s.unmakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                quit = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

//        if(!quit && !tMoveHit && !bCheck){
//            if(depth >= 8){
//                depth -= 2;
//            }
//        }

        //剩余走法
        if(!quit){
            int cnt = 0;
            for(step & move : moveList){
                if(!moveSort::inOtherStepList(move,killerMoveList) &&
                   move != convert_move &&
                   !move.toPiece){
                    //将军延伸
                    int newDepth = bCheck ? depth : depth - 1;
//                    if(!bCheck && newDepth >= 4 && cnt > 2){
//                        newDepth--;
//                    }
                    if(s.makeMove(move)){
                        vl = -searchNonPV(s,newDepth,-vlBeta + 1);
                        s.unmakeMove();

                        if(vl > vlBest){
                            vlBest = vl;
                            if(vl >= vlBeta){
                                pBestMove = &move;
                                break;
                            }
                        }
                        cnt++;
                    }
                }
            }
        }
        s.recoardCache(pBestMove,beta,depth,vlBest);
        if(vlBest == MIN_VALUE){
            return MIN_VALUE + e.getNowDistance();
        }
        return vlBest;
    }
    int searchRoot(searchGroup& s,int maxDepth){
        int vl;
        int vlBest = MIN_VALUE;
        evaluate& e = s.e;
        const bool bCheck = !e.checkMoveStatus.empty() && e.checkMoveStatus.back();
        moveSort::sortRootMoveSequance(s.rootMoveList);
        for(step & move : s.rootMoveList){
            const int newDepth = bCheck ? maxDepth : maxDepth - 1;
            if(s.makeMove(move)){
                if(vlBest == MIN_VALUE){
                    vl = -searchPV(s, newDepth,MIN_VALUE,MAX_VALUE);
                }else{
                    vl = -searchNonPV(s,newDepth,-vlBest);
                    if(vl > vlBest){
                        vl = -searchPV(s,newDepth,MIN_VALUE,-vlBest);
                    }
                }
                s.unmakeMove();

                if(vl > vlBest){
                    moveSort::refreshRootMoveSequance(s.rootMoveList,move);
                    vlBest = vl;
                }
            }
        }
        if(!s.rootMoveList.empty()){
            s.rootMoveList.front().printMove();
        }
        return vlBest;
    }
    int searchMain(int maxDepth,int maxTime){
        //init
        initSearch();
        int vlBest = MIN_VALUE;
        evaluate& e = s.e;
        if(e.isDraw()){
            cout<<"draw"<<endl;
            return e.getDrawValue();
        }else{
            const int repType = e.isRep();
            if(repType == draw_rep){
                cout<<"draw"<<endl;
                return e.getDrawValue();
            }else if(repType == kill_rep){
                cout<<"kill"<<endl;
                return MAX_VALUE;
            }else if(repType == killed_rep){
                cout<<"killed"<<endl;
                return MIN_VALUE;
            }
        }
        //search
        clock_t start = clock();
        for(int depth = 1;depth <= maxDepth;depth++){
            int vl = searchRoot(s,depth);
            if(vl > vlBest){
                vlBest = vl;
                //toDo something
            }
            clock_t now = clock();
            cout<<"depth = "<<depth<<" | vl = "<<vl<<" | time_sum =  "<<setprecision(3)<<(double)(now - start) / CLOCKS_PER_SEC<<"s"<<endl;
            if(now - start >= maxTime / 2){
                break;
            }
        }
        return vlBest;
    }
protected:
    void initSearch(){
        s.clearCache();
        vector<step>().swap(s.rootMoveList);
        genMove::genMoveList(s.e,s.rootMoveList,all);
        moveSort::initSortRootMoveSeuqance(s.e,s.rootMoveList);
    }
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
        if(repType != none_rep){
            if(repType == draw_rep){
                return e.getDrawValue();
            }else if(repType == kill_rep){
                return MAX_BAN_VALUE - nDistance;
            }else if(repType == killed_rep){
                return MIN_BAN_VALUE + nDistance;
            }
        }
        return MIN_VALUE;
    }
protected:
    searchGroup s;
    friend class pipe;
    friend class test;
};