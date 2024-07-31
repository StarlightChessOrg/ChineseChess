#pragma once
#include "defines.hpp"
#include "utils.hpp"

const int SCORE_KING = 100000;
const int SCORE_GUARD = 200;
const int SCORE_BISHOP = 200;
const int SCORE_KNIGHT = 450;
const int SCORE_ROOK = 950;
const int SCORE_CANNON = 450;
const int SCORE_PAWN = 130;

const CHESSMAP POS_KING{
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {1, -8, -9, 0, 0, 0, 0, 0, 0, 0},
     {5, -8, -9, 0, 0, 0, 0, 0, 0, 0},
     {1, -8, -9, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};

const CHESSMAP POS_GUARD{
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};

const CHESSMAP POS_BISHOP{
    {{0, 0, -2, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 3, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, -2, 0, 0, 0, 0, 0, 0, 0}}};

const CHESSMAP POS_KNIGHT{
    {{0, 0, 4, 2, 4, 6, 8, 12, 4, 4},
     {-4, 2, 2, 6, 12, 16, 24, 14, 10, 8},
     {0, 4, 8, 8, 16, 14, 18, 16, 28, 16},
     {0, 4, 8, 6, 14, 18, 24, 20, 16, 12},
     {0, -2, 4, 10, 12, 16, 20, 18, 8, 4},
     {0, 4, 8, 6, 14, 18, 24, 20, 16, 12},
     {0, 4, 8, 8, 16, 14, 18, 16, 28, 16},
     {-4, 2, 2, 6, 12, 16, 24, 14, 10, 8},
     {0, 0, 4, 2, 4, 6, 8, 12, 4, 4}}};

const CHESSMAP POS_ROOK{
    {{-2, 8, 4, 6, 12, 12, 12, 12, 16, 14},
     {10, 4, 8, 10, 16, 14, 18, 12, 20, 14},
     {6, 8, 6, 8, 14, 12, 16, 12, 18, 12},
     {14, 16, 14, 14, 20, 18, 22, 18, 24, 18},
     {12, 8, 12, 14, 20, 18, 22, 18, 26, 16},
     {14, 16, 14, 14, 20, 18, 22, 18, 24, 18},
     {6, 8, 6, 8, 14, 12, 16, 12, 18, 12},
     {10, 4, 8, 10, 16, 14, 18, 12, 20, 14},
     {-2, 8, 4, 6, 12, 12, 12, 12, 16, 14}}};

const CHESSMAP POS_CANNON{
    {{0, 0, 4, 0, -2, 0, 0, 2, 2, 6},
     {0, 2, 0, 0, 0, 0, 0, 2, 2, 4},
     {2, 4, 8, 0, 2, 0, -2, 0, 0, 0},
     {6, 6, 6, 2, 4, 2, 4, -10, -4, -10},
     {6, 6, 10, 4, 6, 8, 10, -8, -14, -12},
     {6, 6, 6, 2, 4, 2, 4, -10, -4, -10},
     {2, 4, 8, 0, 2, 0, -2, 0, 0, 0},
     {0, 2, 0, 0, 0, 0, 0, 2, 2, 4},
     {0, 0, 4, 0, -2, 0, 0, 2, 2, 6}}};

const CHESSMAP POS_PAWN{
    {{0, 0, 0, 0, 2, 6, 10, 14, 18, 0},
     {0, 0, 0, 0, 0, 12, 20, 26, 36, 3},
     {0, 0, 0, -2, 8, 18, 30, 42, 56, 6},
     {0, 0, 0, 0, 0, 18, 34, 60, 80, 9},
     {0, 0, 0, 4, 8, 20, 40, 80, 120, 12},
     {0, 0, 0, 0, 0, 18, 34, 60, 80, 9},
     {0, 0, 0, -2, 8, 18, 30, 42, 56, 6},
     {0, 0, 0, 0, 0, 12, 20, 26, 36, 3},
     {0, 0, 0, 0, 2, 6, 10, 14, 18, 0}}};

/// @brief 算分函数
int calculateScore(ChessMap chessMap, int x, int y)
{
    CHESSID chessid = chessMap.on(x, y);
    CHESSDEF chessdef = toChessdef(chessid);
    TEAM team = toTeam(chessid);
    // 棋子位置权重索引
    int _x = x;
    int _y = y;
    // 如果队伍是黑，则反向读取棋子位置权重值
    if (team == BLACK)
    {
        _y = 9 - y;
    }

    int result = 0;
    switch (chessdef)
    {
    case KING:
        result = SCORE_KING + POS_KING.at(_x).at(_y);
        break;
    case GUARD:
        result = SCORE_GUARD + POS_GUARD.at(_x).at(_y);
        break;
    case BISHOP:
        result = SCORE_BISHOP + POS_BISHOP.at(_x).at(_y);
        break;
    case KNIGHT:
        result = SCORE_KNIGHT + POS_KNIGHT.at(_x).at(_y);
        break;
    case ROOK:
        result = SCORE_ROOK + POS_ROOK.at(_x).at(_y);
        break;
    case CANNON:
        result = SCORE_CANNON + POS_CANNON.at(_x).at(_y);
        break;
    case PAWN:
        result = SCORE_PAWN + POS_PAWN.at(_x).at(_y);
        break;
    case 0:
        result = 0;
    }
    if (team == RED)
    {
        result = -result;
    }
    return result;
}
