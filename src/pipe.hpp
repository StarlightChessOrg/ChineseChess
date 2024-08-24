#pragma once
#include "search.hpp"
#include "experiment.hpp"
#include <windows.h>

const string ui_path = R"(E:\Projects_chess\ChineseChess-nnue\ui\ui.txt)";
const string ai_path = R"(E:\Projects_chess\ChineseChess-nnue\ui\ai.txt)";

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

    void parse(vector<string>& messagePool){
        if(!messagePool.empty()){
            cout<<"-----------------------------------"<<endl;
            for(const string& message : messagePool){
                cout<<message<<endl;
                if(message == "init and ai is red"){
                    aSide = red;
                    m.s.initSearchGroup(initGameBoard,red);
                }else if(message=="init and ai is black"){
                    aSide = black;
                    m.s.initSearchGroup(initGameBoard,red);
                }else if(message == "regret"){
                    if(m.s.e.moveRoad.size() > 1){
                        m.s.unmakeMove();
                        m.s.unmakeMove();
                        vector<string> _messagePool;
                        getOtherMoveList(_messagePool);
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
                        const int fromPiece = m.s.e.board.getPieceByPos(fromPos);
                        const int toPiece = m.s.e.board.getPieceByPos(toPos);
                        step move = step(fromPos,toPos,fromPiece,toPiece);
                        m.s.makeMove(move);
                        cout<<endl;
                        m.s.e.board.printBasicBoard();
                        cout<<endl;
                    }else{
                        cout<<"error move message,that is "<<message<<endl;
                    }
                }
            }
        }
        messagePool.clear();
    }

    void getOtherMoveList(vector<string>& messagePool){
        messagePool.emplace_back("the move list of the other side as follows:");
        vector<step> moveList;
        genMove::genMoveList(m.s.e,moveList,all);
        for(step& move : moveList){
            if(m.s.makeMove(move)){
                string uiGoStr = to_string(move.fromPos) + ">" + to_string(move.toPos);
                messagePool.push_back(uiGoStr);
                m.s.unmakeMove();
            }
        }
    }

    void tryThink(){
        vector<string> messagePool;
        if(aSide == m.s.e.side && aSide){
            m.searchMain(16,4000);
            if(m.s.rootMoveList.empty()){
                messagePool.emplace_back("there is no best move");
                feedback(messagePool);
            }else{
                step aiGo = m.s.rootMoveList.front();
                m.s.makeMove(aiGo);
                string aiGoStr = to_string(aiGo.fromPos) + ">" + to_string(aiGo.toPos);
                messagePool.push_back(aiGoStr);
                getOtherMoveList(messagePool);
                feedback(messagePool);
            }
        }else if(aSide && m.s.e.moveRoad.empty()){
            getOtherMoveList(messagePool);
            feedback(messagePool);
        }
    }

    void work(){
        aSide = 0;
        vector<string> messagePool;
        initWork();
        while(true){
            listen(messagePool);
            Sleep(1);
            parse(messagePool);
            Sleep(1);
            tryThink();
            Sleep(1);
        }
    }

    void initWork(){
        ofstream file_writer(ai_path,ios_base::out);
    }
private:
    searchManager m;
    int aSide;
};

