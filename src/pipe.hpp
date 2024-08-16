#pragma once
#include "search.hpp"
#include "experiment.hpp"
#include <windows.h>

const string ui_path = R"(E:\Projects_chess\ChineseChess\ui\ui.txt)";
const string ai_path = R"(E:\Projects_chess\ChineseChess\ui\ai.txt)";

class pipe{
public:
    pipe(){
        aSide = 0;
    }
    void listen(vector<string>& messagePool){
        ifstream file;
        file.open(ui_path,ios::in | ios::out);
        string message;
        while(getline(file,message)){
            messagePool.push_back(message);
        }
        ofstream file_writer(ui_path,ios_base::out);
    }

    void feedback(vector<string>& messagePool){
        ofstream file;
        file.open(ai_path,ios::in | ios::out);
        for(const string& message : messagePool){
            file << message << endl;
        }
    }

    void parse(vector<string>& messagePool,evaluate& e){
        if(!messagePool.empty()){
            cout<<"-----------------------------------"<<endl;
            for(const string& message : messagePool){
                cout<<message<<endl;
                if(message == "init and ai is red"){
                    aSide = red;
                    e.initEvaluate(initGameBoard,red);
                }else if(message=="init and ai is black"){
                    aSide = black;
                    e.initEvaluate(initGameBoard,red);
                }else if(message == "regret"){
                    if(e.moveRoad.size() > 1){
                        e.unMakeMove();
                        e.unMakeMove();
                        vector<string> _messagePool;
                        getOtherMoveList(_messagePool,e);
                        feedback(_messagePool);
                    }
                }else if(message == "quit"){
                    exit(0);
                }else{
                    vector<string> StepStr;
                    stringSplit(message,'>',StepStr);
                    if(StepStr.size() == 2){
                        const int fromPos = atoi(StepStr.front().c_str());
                        const int toPos = atoi(StepStr.back().c_str());
                        e.makeMove(fromPos,toPos);
                        cout<<endl;
                        e.board.printBasicBoard();
                        cout<<endl;
                    }else{
                        cout<<"error move message,that is "<<message<<endl;
                    }
                }
            }
        }
        messagePool.clear();
    }

    void getOtherMoveList(vector<string>& messagePool,evaluate& e){
        messagePool.emplace_back("the move list of the other side as follows:");
        vector<step> moveList;
        genMove::genMoveList(e,moveList,all);
        for(step& move : moveList){
            if(e.makeMove(move.fromPos,move.toPos)){
                string uiGoStr = to_string(move.fromPos) + ">" + to_string(move.toPos);
                messagePool.push_back(uiGoStr);
                e.unMakeMove();
            }
        }
    }

    void tryThink(evaluate& e,searchGroup& s){
        vector<string> messagePool;
        if(aSide == e.side && aSide){
            s.searchMain(e,16,4000);
            if(s.rootMoveList.empty()){
                messagePool.emplace_back("there is no best move");
            }else{
                step aiGo = s.rootMoveList.front();
                e.makeMove(aiGo.fromPos,aiGo.toPos);
                string aiGoStr = to_string(aiGo.fromPos) + ">" + to_string(aiGo.toPos);
                messagePool.push_back(aiGoStr);
                getOtherMoveList(messagePool,e);
                feedback(messagePool);
            }
        }else if(aSide && e.moveRoad.empty()){
            getOtherMoveList(messagePool,e);
            feedback(messagePool);
        }
    }

    void work(){
        evaluate e;
        searchGroup s;
        aSide = 0;
        vector<string> messagePool;
        initWork();
        while(true){
            listen(messagePool);
            parse(messagePool,e);
            tryThink(e,s);
            Sleep(1);
        }
    }

    void initWork(){
        ofstream file_writer(ai_path,ios_base::out);
    }

private:
    int aSide;
};

