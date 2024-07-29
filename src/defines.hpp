#pragma once
#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

using CHESSID = int;
using TEAM = int;
using CHESSDEF = int;
using NODE_TYPE = bool;
using CHESSMAP = array<array<CHESSID, 10>, 9>;
const CHESSDEF EMPTY = 0;
const CHESSDEF OUT = 8;
const CHESSDEF KING = 1;
const CHESSDEF GUARD = 2;
const CHESSDEF BISHOP = 3;
const CHESSDEF KNIGHT = 4;
const CHESSDEF ROOK = 5;
const CHESSDEF CANNON = 6;
const CHESSDEF PAWN = 7;
const CHESSID R_KING = 1;
const CHESSID R_GUARD = 2;
const CHESSID R_BISHOP = 3;
const CHESSID R_KNIGHT = 4;
const CHESSID R_ROOK = 5;
const CHESSID R_CANNON = 6;
const CHESSID R_PAWN = 7;
const CHESSID B_KING = -1;
const CHESSID B_GUARD = -2;
const CHESSID B_BISHOP = -3;
const CHESSID B_KNIGHT = -4;
const CHESSID B_ROOK = -5;
const CHESSID B_CANNON = -6;
const CHESSID B_PAWN = -7;
const TEAM RED = 1;
const TEAM BLACK = -1;
const int MIN_NUMBER = -100000000;
const int MAX_NUMBER = 100000000;
const NODE_TYPE MAX = true;
const NODE_TYPE MIN = false;
const CHESSMAP DEFAULT_CHESSMAP{
    {{R_ROOK, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_ROOK},
     {R_KNIGHT, 0, R_CANNON, 0, 0, 0, 0, B_CANNON, 0, B_KNIGHT},
     {R_BISHOP, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_BISHOP},
     {R_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, B_GUARD},
     {R_KING, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_KING},
     {R_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, B_GUARD},
     {R_BISHOP, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_BISHOP},
     {R_KNIGHT, 0, R_CANNON, 0, 0, 0, 0, B_CANNON, 0, B_KNIGHT},
     {R_ROOK, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_ROOK}}};
int searchCount = 0;

/// @brief 获取chessid的chessdef
CHESSDEF toChessdef(CHESSID chessid)
{
    return abs(chessid);
}

/// @brief 获取chessdef的team
TEAM toTeam(CHESSID chessid)
{
    if (chessid == 0)
    {
        return 0;
    }
    else
    {
        return chessid > 0 ? RED : BLACK;
    }
}

/// @brief 获取指定chessid的名称
string getName(CHESSID chessid)
{
    switch (chessid)
    {
    case R_KING:
        return "帅";
    case B_KING:
        return "将";
    case R_GUARD:
        return "士";
    case B_GUARD:
        return "仕";
    case R_BISHOP:
        return "相";
    case B_BISHOP:
        return "象";
    case R_KNIGHT:
        return "马";
    case B_KNIGHT:
        return "马";
    case R_ROOK:
        return "车";
    case B_ROOK:
        return "车";
    case R_CANNON:
        return "炮";
    case B_CANNON:
        return "炮";
    case R_PAWN:
        return "兵";
    case B_PAWN:
        return "卒";
    default:
        return "  ";
    }
}
