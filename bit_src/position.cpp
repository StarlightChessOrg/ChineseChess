#include "position.hpp"

int main(){
    position p = position(initGameBoard);
    p.makeMove(5,4,12,4);
    p.board.printBasicBoard();
    p.swapBoard.printSwapBoard();
    p.bitBoard.printBitBoard();
    p.unMakeMove(5,4,12,4,-10,6);
    p.board.printBasicBoard();
    p.swapBoard.printSwapBoard();
    p.bitBoard.printBitBoard();
    cout<<endl;
    return 0;
}