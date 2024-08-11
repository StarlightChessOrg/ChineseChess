#pragma once
#include "search.hpp"
#include "ctime"
#include <random>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <io.h>

class test{
public:
    static void testRandomMove(){
        default_random_engine r;
        r.seed(7931);
        evaluate e = evaluate(initGameBoard);
        evaluate origin_e = evaluate(initGameBoard);
        assert(e == origin_e);
        for(int t = 0;t < 20000;t++){
            cout<<t<<endl;
            for(int s = 0;s < 80;s++){
                vector<step> moveList;
                genMove::genMoveList(e,moveList,all);
                if(!moveList.empty()){
                    uniform_int_distribution<int>u(0,(int)moveList.size() - 1);
                    step& move = moveList[u(r)];
                    assert(genMove::legalMove(e,move));
                    e.makeMove(move.fromPos,move.toPos);
                }else{
                    break;
                }
            }
            while(!e.moveRoad.empty()){
                e.unMakeMove();
            }
            if(e != origin_e){
                e.board.printBasicBoard();
            }
            assert(e == origin_e);
        }
    }
    static void testRecoardMove(){
        string rootPath = "E:\\Projects_chess\\dump_2";
        vector<string> filePaths;
        getFiles(rootPath,filePaths);
        cout<<filePaths.size()<<endl;
        for(const string& path : filePaths){
            evaluate e = evaluate(initGameBoard,red);
            ifstream in(path);
            string moveStr;
            vector<string> moveStrVec;
            while(getline(in,moveStr)) {
                moveStrVec.push_back(moveStr);
                //cout << moveStr << endl;
                const int combinatePos = atoi(moveStr.c_str());
                const int fromPos = combinatePos & ((1 << 8) - 1);
                const int toPos = combinatePos >> 8;
                //cout<<fromPos<<"\t"<<toPos<<endl;
                vector<step> moveList;
                genMove::genMoveList(e,moveList,all);
                bool findIt = false;
                for(step& move : moveList){
                    if(move.fromPos == fromPos && move.toPos == toPos){
                        findIt = true;
                        break;
                    }
                }
                if(!findIt){
                    cout<<"! -> "<<path<<endl;
//                    evaluate new_e = evaluate(initGameBoard,red);
//                    for(int i = 0;i < e.moveRoad.size();i++){
//                        new_e.makeMove(e.moveRoad[i].fromPos,e.moveRoad[i].toPos);
//                        new_e.board.printBasicBoard();
//                    }
                    break;
                }
                //assert(findIt);
                if(!e.makeMove(fromPos,toPos)){
                    e.board.printBasicBoard();
                    //e.makeMove(fromPos,toPos);
                    cout<<path<<endl;
//                    evaluate new_e = evaluate(initGameBoard,red);
//                    for(int i = 0;i < e.moveRoad.size();i++){
//                        new_e.makeMove(e.moveRoad[i].fromPos,e.moveRoad[i].toPos);
//                        new_e.board.printBasicBoard();
//                        cout<<endl;
//                    }
                    break;
                }
                //assert(e.makeMove(fromPos,toPos));
            }
        }
    }
    static void testInitEvaluateShouldMirror(){
        evaluate e = evaluate(initGameBoard,red);
        e.resetEvaBoard();
        for(int i = 0;i < 7;i++){
            bool allZero = true;
            for(int a = 0;a < 256;a++){
                if(e.vlRedBoard[i][a]){
                    allZero = false;
                }
                assert(e.vlRedBoard[i][a] == e.vlBlackBoard[i][getPos(yMirrorPos(getY(a)),getX(a))]);
                assert(e.vlRedBoard[i][a] == e.vlBlackBoard[i][xyMirrorPos(a)]);
            }
            assert(!allZero);
        }
    }

    static void testGenNewRookMove(){
        evaluate e = evaluate(initGameBoard,red);
        e.resetEvaBoard();
        vector<step> moveList;
        genMove::genMoveList(e,moveList,all);
        step::printMoveList(moveList);
        moveList.clear();
        genMove::genMoveList(e,moveList,justEat);
        step::printMoveList(moveList);
    }

    static void testSearch(){
        evaluate e = evaluate(initGameBoard,red);
        searchGroup s = searchGroup();
        time_t start = clock();
        s.searchMain(e,9,3000);
        time_t end = clock();
        cout<<(double)(end - start) / CLOCKS_PER_SEC<<endl;
    }
protected:
    static void getFiles(string path, vector<string>& files)
    {
        intptr_t hFile = 0;
        struct _finddata_t fileinfo{};
        string p;
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
        {
            do
            {
                if ((fileinfo.attrib &  _A_SUBDIR))
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                        getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                }
                else
                {
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            _findclose(hFile);
        }
    }
};

