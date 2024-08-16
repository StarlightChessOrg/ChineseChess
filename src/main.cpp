#include "experiment.hpp"
#include "pipe.hpp"

void testFunc(){
    vector<string> messagePool;
    while(true){
        listen_from_ui(messagePool);
        if(!messagePool.empty()){
            for(string s : messagePool){
                cout<<s<<endl;
            }
            ofstream file_writer(R"(E:\Projects_chess\ChineseChess\ui\ui.txt)",ios_base::out);
            break;
        }
        Sleep(1);
    }
}

int main(){
    test::testSearch();
    return 0;
}