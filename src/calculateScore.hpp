#ifndef DEFINES_HPP
#include "defines.hpp"
#endif

int calculateScore(CHESSMAP chessMap, int x, int y)
{
  // 暂时简单写写，以后会改进
  CHESSDEF chess = toChessdef(chessMap.at(x).at(y));
  switch (chess)
  {
  case KING:
    return 10000;
  case GUARD:
    return 10;
  case BISHOP:
    return 10;
  case KNIGHT:
    return 300;
  case ROOK:
    return 1000;
  case CANNON:
    return 400;
  case PAWN:
    return 4;
  default:
    return 0;
  }
}