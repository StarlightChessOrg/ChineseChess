#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <functional>

using namespace std;

// -----常量----- //
using CHESSID = int;
using TEAM = int;
using CHESSDEF = int;
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

// -----工具函数----- //

/// @brief 获取chessid的chessdef
CHESSDEF toChessdef(CHESSID chessid)
{
  return abs(chessid);
}

/// @brief 获取chessdef的team
TEAM getTeam(CHESSID chessid)
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

/// @brief 获取数组（或其他可迭代容器如vector）中最大值索引
template <typename Arraylike>
int getIndexOfMax(Arraylike array)
{
  int maxIndex = 0;
  int maxNumber = -1000000000;
  for (int i = 0; i < array.size(); i++)
  {
    int number = array.at(i);
    if (number > maxNumber)
    {
      maxNumber = number;
      maxIndex = i;
    }
  }
  return maxIndex;
}
// -----位置类----- //

/// @brief 位置类
struct Position
{
  Position(int x, int y) : x(x), y(y){};
  int x;
  int y;
};

using TARGETS = vector<Position>;

// -----着法类----- //

/// @brief 着法结构
struct Action
{
  Action(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2){};
  int x1;
  int y1;
  int x2;
  int y2;
};

using ACTIONS = vector<Action>;

// -----着法和分数的多返回值----- //

/// @brief 着法和分数的多返回值结构
struct Result_ActionAndScore
{
  Result_ActionAndScore(Action action, int score) : action(action), score(score){};
  Action action;
  int score;
};
