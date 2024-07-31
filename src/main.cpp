#include "board.hpp"

int main()
{
    Board board {};
    board.printBoard();
    board.makeDecision(4);
    cout << searchCount << endl;
    board.printBoard();
    return 0;
}
