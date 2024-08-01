#include "position.hpp"

int main(){
    position p = position(initGameBoard);
    p.makeMove(5,4,12,4);
    p.unMakeMove(5,4,12,4,-10,6);
    cout<<endl;
    return 0;
}