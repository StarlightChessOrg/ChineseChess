#include "board.hpp"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Board board;
    board.printBoard();
    board.makeDecision(3);

    system("pause");

    return 0;
}
