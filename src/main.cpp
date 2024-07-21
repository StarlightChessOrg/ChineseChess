#include "board.hpp"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    try
    {
        Board board;
        board.printBoard();
        board.makeDecision(3);

        system("pause");

        return 0;
    }
    catch (exception err)
    {
        cerr << err.what() << endl;
        system("pause");
    }
}
