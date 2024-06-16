#include "defines.hpp"

// -----类声明----- //

/// @brief 棋盘类
class Board
{
  // 棋盘内容
public:
  CHESSMAP chessMap{
      {{R_ROOK, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_ROOK},
       {R_KNIGHT, 0, R_CANNON, 0, 0, 0, 0, B_CANNON, 0, B_KNIGHT},
       {R_BISHOP, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_BISHOP},
       {R_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, B_GUARD},
       {R_KING, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_KING},
       {R_GUARD, 0, 0, 0, 0, 0, 0, 0, 0, B_GUARD},
       {R_BISHOP, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_BISHOP},
       {R_KNIGHT, 0, R_CANNON, 0, 0, 0, 0, B_CANNON, 0, B_KNIGHT},
       {R_ROOK, 0, 0, R_PAWN, 0, 0, B_PAWN, 0, 0, B_ROOK}}};
  bool isRedTurn = true;

  // 对外接口
public:
  TARGETS getMovesOf(CHESSID chessdef, int x, int y);
  void moveTo(int x1, int y1, int x2, int y2);
  void printBoard();

  // 工具函数
protected:
  TEAM teamOn(int x, int y);
  CHESSID on(int x, int y);
  TARGETS filter(TARGETS originalMoves, CHESSDEF chessdef, TEAM team);

  // 棋子函数
protected:
  TARGETS king(int x, int y, TEAM team);
  TARGETS guard(int x, int y, TEAM team);
  TARGETS bishop(int x, int y, TEAM team);
  TARGETS knight(int x, int y, TEAM team);
  TARGETS rook(int x, int y, TEAM team);
  TARGETS cannon(int x, int y, TEAM team);
  TARGETS pawn(int x, int y, TEAM team);

  // 自动走棋函数
public:
  void makeDecision();

protected:
  int getScore(CHESSMAP chessMap, int x, int y);
  ACTIONS getAllAvailableActionsOfTeam(TEAM team);
  Result_ActionAndScore evaluateBestAction(Board board, bool isRedTurn, int depth, int maxDepth);
};

// -----对外接口----- //

/// @brief 获取指定棋子的全部可行着法
TARGETS Board::getMovesOf(CHESSID chessid, int x, int y)
{
  CHESSDEF chessdef = toChessdef(chessid);
  TEAM team = getTeam(chessid);
  switch (chessdef)
  {
  case KING:
    return king(x, y, team);
    break;
  case GUARD:
    return guard(x, y, team);
    break;
  case BISHOP:
    return bishop(x, y, team);
    break;
  case KNIGHT:
    return knight(x, y, team);
    break;
  case ROOK:
    return rook(x, y, team);
    break;
  case CANNON:
    return cannon(x, y, team);
    break;
  case PAWN:
    return pawn(x, y, team);
    break;
  default:
    throw "ERROR CHESSID in Board::getMovesOf!";
  }
}

/// @brief 移动棋子
void Board::moveTo(int x1, int y1, int x2, int y2)
{
  chessMap.at(x2).at(y2) = chessMap.at(x1).at(y1);
  chessMap.at(x1).at(y1) = 0;
  isRedTurn = !isRedTurn;
}

/// @brief 打印棋盘
void Board::printBoard()
{
  for (int colIndex = -1; colIndex <= 8; colIndex++)
  {
    if (colIndex != -1)
    {
      cout << colIndex << "  ";
    }
    else
    {
      cout << "X  ";
    }
    for (int rawIndex = 0; rawIndex <= 9; rawIndex++)
    {
      if (colIndex == -1)
      {
        cout << rawIndex << " ";
        continue;
      }
      CHESSID chessid = on(colIndex, rawIndex);
      string name = getName(chessid);
      cout << name;
    }
    cout << endl;
  }
  cout << endl;
}

// -----工具函数----- //

/// @brief 获取指定位置的棋子队伍
TEAM Board::teamOn(int x, int y)
{
  return getTeam(chessMap.at(x).at(y));
}

/// @brief 过滤棋子行棋范围外的位置和己方棋子占据位置（避免黑吃黑）
/// 不负责过滤马脚、象腿
/// @param originalMoves 全部可行着法
/// @param chessid 需要根据棋子id来判断这个棋子的行棋范围
TARGETS Board::filter(TARGETS originalMoves, CHESSDEF chessdef, TEAM team)
{
  TARGETS moves;
  if (chessdef == KING || chessdef == GUARD) // 帅和士
  {
    for (Position v : originalMoves)
    {
      if (v.x >= 3 && v.x <= 5)
      {
        if (team == RED)
        {
          if (v.y >= 0 && v.y <= 2 && teamOn(v.x, v.y) != team)
            moves.push_back(Position(v.x, v.y));
        }
        else
        {
          if (v.y >= 7 && v.y <= 9 && teamOn(v.x, v.y) != team)
            moves.push_back(Position(v.x, v.y));
        }
      }
    }
  }
  else if (chessdef == BISHOP) // 象
  {
    for (Position v : originalMoves)
    {
      if (v.x >= 0 && v.x <= 8)
      {
        if (team == RED)
        {
          if (v.y >= 0 && v.y <= 4 && teamOn(v.x, v.y) != team)
            moves.push_back(Position(v.x, v.y));
        }
        else
        {
          if (v.y >= 5 && v.y <= 9 && teamOn(v.x, v.y) != team)
            moves.push_back(Position(v.x, v.y));
        }
      }
    }
  }
  else // 其他
  {
    for (Position v : originalMoves)
    {
      if (v.x >= 0 && v.x <= 8 &&
          v.y >= 0 && v.y <= 9 &&
          teamOn(v.x, v.y) != team)
      {
        moves.push_back(Position(v.x, v.y));
      }
    }
  }
  return moves;
}

/// @brief 获取棋盘上某个位置的棋子
CHESSID Board::on(int x, int y)
{
  if (x >= 0 && x <= 8 && y >= 0 && y <= 9)
  {
    return chessMap.at(x).at(y);
  }
  else
  {
    return OUT;
  }
}

// -----棋子函数----- //

/// @brief 获取帅、将的所有走法
TARGETS Board::king(int x, int y, TEAM team)
{
  TARGETS originalMoves = {
      Position(x + 1, y), Position(x - 1, y),
      Position(x, y + 1), Position(x, y - 1)};
  TARGETS moves = filter(originalMoves, KING, team);

  // 白脸将规则

  array<CHESSID, 10> col = chessMap.at(x);
  Position enemyKingPosition(-1, -1);
  bool state = false; // false：未发现将帅棋子之一；true：发现其中一个
  for (int i_y = 0; i_y < 10; i_y++)
  {
    CHESSID chessid = col.at(i_y);
    // 记录将帅棋子
    if (toChessdef(chessid) == KING)
    {
      if (getTeam(chessid) != team)
      {
        enemyKingPosition.x = x;
        enemyKingPosition.y = i_y;
      }
      // 如果已经发现过将帅棋子，还没有中断，则推断出中间一定没有棋子，白脸将成立
      if (state == true)
      {
        moves.push_back(enemyKingPosition);
        break;
      }

      state = true;
    }
    // 如果发现将帅棋子之一，且中间有棋子阻隔，中断
    if (state == true && chessid != 0 && toChessdef(chessid) != KING)
      break;
  }
  return moves;
};

/// @brief 获取士所有走法
TARGETS Board::guard(int x, int y, TEAM team)
{
  TARGETS originalMoves = {
      Position(x + 1, y + 1), Position(x - 1, y - 1),
      Position(x - 1, y + 1), Position(x + 1, y - 1)};
  TARGETS moves = filter(originalMoves, GUARD, team);
  return moves;
}

/// @brief 获取相所有走法
TARGETS Board::bishop(int x, int y, TEAM team)
{
  TARGETS originalMoves;

  Position pos(x + 1, y + 1);
  if (on(pos.x, pos.y) == 0)
    originalMoves.push_back(Position(x + 2, y + 2));

  pos.x = x + 1;
  pos.y = y - 1;
  if (on(pos.x, pos.y) == 0)
    originalMoves.push_back(Position(x + 2, y - 2));

  pos.x = x - 1;
  pos.y = y + 1;
  if (on(pos.x, pos.y) == 0)
    originalMoves.push_back(Position(x - 2, y + 2));

  pos.x = x - 1;
  pos.y = y - 1;
  if (on(pos.x, pos.y) == 0)
    originalMoves.push_back(Position(x - 2, y - 2));

  TARGETS moves = filter(originalMoves, BISHOP, team);
  return moves;
}

/// @brief 获取马所有走法
TARGETS Board::knight(int x, int y, TEAM team)
{
  TARGETS originalMoves;

  Position pos(x + 1, y);
  if (on(pos.x, pos.y) == 0)
  {
    originalMoves.push_back(Position(x + 2, y + 1));
    originalMoves.push_back(Position(x + 2, y - 1));
  }

  pos.x = x - 1;
  if (on(pos.x, pos.y) == 0)
  {
    originalMoves.push_back(Position(x - 2, y + 1));
    originalMoves.push_back(Position(x - 2, y - 1));
  }

  pos.x = x;
  pos.y = y + 1;
  if (on(pos.x, pos.y) == 0)
  {
    originalMoves.push_back(Position(x - 1, y + 2));
    originalMoves.push_back(Position(x + 1, y + 2));
  }

  pos.y = y - 1;
  if (on(pos.x, pos.y) == 0)
  {
    originalMoves.push_back(Position(x - 1, y - 2));
    originalMoves.push_back(Position(x + 1, y - 2));
  }

  TARGETS moves = filter(originalMoves, BISHOP, team);
  return moves;
}

/// @brief 获取车所有走法
TARGETS Board::rook(int x, int y, TEAM team)
{
  TARGETS moves;
  Position pos(x, y);

  /// 函数简写：对于车当前遍历到的着法位置进行判断，并推送到moves
  /// @return 是否break
  auto infer = [&]() -> bool
  {
    if (on(pos.x, pos.y) == 0)
    {
      moves.push_back(pos);
      return false;
    }
    else if (teamOn(pos.x, pos.y) != team)
    {
      moves.push_back(pos);
      return true;
    }
    else
    {
      return true;
    }
  };

  for (pos.x++; pos.x <= 8; pos.x++)
  {
    if (infer() == true)
      break;
  }

  pos.x = x;
  for (pos.x--; pos.x >= 0; pos.x--)
  {
    if (infer() == true)
      break;
  }

  pos.x = x;
  for (pos.y++; pos.y <= 9; pos.y++)
  {
    if (infer() == true)
      break;
  }

  pos.y = y;
  for (pos.y--; pos.y >= 0; pos.y--)
  {
    if (infer() == true)
      break;
  }

  return moves;
}

/// @brief 获取炮所有走法
TARGETS Board::cannon(int x, int y, TEAM team)
{
  TARGETS moves;
  Position pos(x, y);

  for (pos.x++; pos.x <= 8; pos.x++)
  {
    if (on(pos.x, pos.y) == 0)
    {
      moves.push_back(pos);
    }
    else
    {
      for (pos.x++; pos.x <= 8; pos.x++)
      {
        CHESSID chessid = on(pos.x, pos.y);
        if (chessid == 0)
        {
          continue;
        }
        else if (getTeam(chessid) == team)
        {
          break;
        }
        else
        {
          moves.push_back(pos);
          break;
        }
      }
      break;
    }
  }

  pos.x = x;
  for (pos.x--; pos.x >= 0; pos.x--)
  {
    if (on(pos.x, pos.y) == 0)
    {
      moves.push_back(pos);
    }
    else
    {
      for (pos.x--; pos.x >= 0; pos.x--)
      {
        CHESSID chessid = on(pos.x, pos.y);
        if (chessid == 0)
        {
          continue;
        }
        else if (getTeam(chessid) == team)
        {
          break;
        }
        else
        {
          moves.push_back(pos);
          break;
        }
      }
      break;
    }
  }

  pos.x = x;
  for (pos.y++; pos.y <= 9; pos.y++)
  {
    if (on(pos.x, pos.y) == 0)
    {
      moves.push_back(pos);
    }
    else
    {
      for (pos.y++; pos.y <= 9; pos.y++)
      {
        CHESSID chessid = on(pos.x, pos.y);
        if (chessid == 0)
        {
          continue;
        }
        else if (getTeam(chessid) == team)
        {
          break;
        }
        else
        {
          moves.push_back(pos);
          break;
        }
      }
      break;
    }
  }

  pos.y = y;
  for (pos.y--; pos.y >= 0; pos.y--)
  {
    if (on(pos.x, pos.y) == 0)
    {
      moves.push_back(pos);
    }
    else
    {
      for (pos.y--; pos.y >= 0; pos.y--)
      {
        CHESSID chessid = on(pos.x, pos.y);
        if (chessid == 0)
        {
          continue;
        }
        else if (getTeam(chessid) == team)
        {
          break;
        }
        else
        {
          moves.push_back(pos);
          break;
        }
      }
      break;
    }
  }

  return moves;
}

/// @brief 获取兵、卒所有走法
TARGETS Board::pawn(int x, int y, TEAM team)
{
  TARGETS originalMoves;
  TARGETS moves;

  if (team == RED)
  {
    if (x >= 0 && x <= 8 && y >= 0 && y <= 4)
    {
      originalMoves.push_back(Position(x, y + 1));
      moves = filter(originalMoves, PAWN, team);
    }
    else
    {
      originalMoves.push_back(Position(x, y + 1));
      originalMoves.push_back(Position(x + 1, y));
      originalMoves.push_back(Position(x - 1, y));
      moves = filter(originalMoves, PAWN, team);
    }
  }
  else
  {
    if (x >= 0 && x <= 8 && y >= 5 && y <= 9)
    {
      originalMoves.push_back(Position(x, y - 1));
      moves = filter(originalMoves, PAWN, team);
    }
    else
    {
      originalMoves.push_back(Position(x, y - 1));
      originalMoves.push_back(Position(x + 1, y));
      originalMoves.push_back(Position(x - 1, y));
      moves = filter(originalMoves, PAWN, team);
    }
  }

  return moves;
}

// -----自动走棋----- //

/// @brief 获取某一个队伍所有可行着法
/// @param team 队伍
/// @return 所有可行着法的vector
ACTIONS Board::getAllAvailableActionsOfTeam(TEAM team)
{
  ACTIONS result{};
  int x = 0, y = 0;
  for (auto col : chessMap)
  {
    for (auto chessid : col)
    {
      if (getTeam(chessid) == team)
      {
        TARGETS targets = getMovesOf(chessid, x, y);
        for (auto v : targets)
        {
          result.push_back(Action(x, y, v.x, v.y));
        }
      }
      y++;
    }
    x++;
    y = 0;
  }
  return result;
}

/// @brief 获取指定位置上的棋子的分数
int Board::getScore(CHESSMAP chessMap, int x, int y)
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

/// @brief AI决定最佳着法（对外接口）
void Board::makeDecision()
{
  Result_ActionAndScore result = evaluateBestAction(*this, isRedTurn, 0, 3);
  Action &action = result.action;
  moveTo(action.x1, action.y1, action.x2, action.y2);
}

/// @brief 评估最佳着法
/// @param board Board对象
/// @param isRedTurn 是否是红方行棋
/// @param depth 深度（用于递归）
/// @param maxDepth 最大深度（用于递归）
/// @return 最佳着法及其得分
Result_ActionAndScore Board::evaluateBestAction(Board board, bool isRedTurn, int depth, int maxDepth)
{
  vector<int> scores;
  TEAM team = isRedTurn ? RED : BLACK;
  ACTIONS actions = board.getAllAvailableActionsOfTeam(team);

  for (Action v : actions)
  {
    // 获取分数
    Board board2 = board;
    CHESSMAP &chessMap = board2.chessMap;
    int score = getScore(chessMap, v.x2, v.y2);
    // 如果深度没有达到
    if (depth < maxDepth)
    {
      // 模拟走棋
      chessMap.at(v.x2).at(v.y2) = chessMap.at(v.x1).at(v.y1);
      chessMap.at(v.x1).at(v.y1) = 0;

      // 递归
      Result_ActionAndScore _ = evaluateBestAction(board2, !isRedTurn, depth + 1, maxDepth);
      int scoreReduce = _.score;

      score -= scoreReduce;
    }

    // 推送分数
    scores.push_back(score);
  }

  int maxScoreIndex = getIndexOfMax<vector<int>>(scores); // 获取scores最大值的index

  Action action = actions.at(maxScoreIndex);
  int score = scores.at(maxScoreIndex);

  return Result_ActionAndScore(action, score);
}
