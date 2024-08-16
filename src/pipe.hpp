#pragma once
#include "search.hpp"
#include <windows.h>

void listen_from_ui(vector<string>& messagePool){
    ifstream file;
    file.open(R"(E:\Projects_chess\ChineseChess\ui\ui.txt)",ios::in | ios::out);
    string message;
    while(getline(file,message)){
        messagePool.push_back(message);
    }
}

void feedback_to_ui(vector<string>& messagePool){
    ofstream file;
    file.open(R"(E:\Projects_chess\ChineseChess\ui\ui.txt)",ios::in | ios::out);
    string message;
    for(const string& message : messagePool){
        file << message << endl;
    }
    messagePool.clear();
}