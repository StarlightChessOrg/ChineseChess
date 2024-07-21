#pragma once
#include "defines.hpp"
#include "utils.hpp"

int calculateScore(ChessMap chessMap, int x, int y)
{
    // 暂时简单写写，以后会改进
    CHESSID chessid = chessMap.at(x, y);
    CHESSDEF chessdef = toChessdef(chessid);
    int result;
    switch (chessdef)
    {
    case KING:
        result = MAX_NUMBER;
        break;
    case GUARD:
        result = 10;
        break;
    case BISHOP:
        result = 10;
        break;
    case KNIGHT:
        result = 300;
        break;
    case ROOK:
        result = 1000;
        break;
    case CANNON:
        result = 400;
        break;
    case PAWN:
        result = 4;
        break;
    case 0:
        result = 0;
    }
    if (toTeam(chessid) == RED)
    {
        result = -result;
    }
    return result;
}
