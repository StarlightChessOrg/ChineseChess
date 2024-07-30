#include "calculateScore.hpp"
#include "defines.hpp"
#include "utils.hpp"

// -----类声明----- //

/// @brief 棋盘类
class Board
{
public: // 棋盘内容
    ChessMap chessMap{DEFAULT_CHESSMAP};
    bool isRedTurn = true;

public: // 对外接口
    TARGETS getMovesOf(CHESSID chessdef, int x, int y);
    void printBoard();
    void makeDecision(int depth);

protected: // 工具函数
    TEAM teamOn(int x, int y);
    CHESSID chessOn(int x, int y);
    TARGETS removeImpossible(TARGETS originalMoves, CHESSDEF chessdef, TEAM team);
    void moveTo(int x1, int y1, int x2, int y2);

protected: // 棋子函数
    TARGETS king(int x, int y, TEAM team);
    TARGETS guard(int x, int y, TEAM team);
    TARGETS bishop(int x, int y, TEAM team);
    TARGETS knight(int x, int y, TEAM team);
    TARGETS rook(int x, int y, TEAM team);
    TARGETS cannon(int x, int y, TEAM team);
    TARGETS pawn(int x, int y, TEAM team);

public: // 自动走棋实现
    int getScore(int x, int y);
    ACTIONS getAllActionsOfTeam(TEAM team);
    Node evaluateBestNode(int depth, int maxDepth, Node &father);
};

// -----对外接口----- //

/// @brief 获取指定棋子的全部可行着法
TARGETS Board::getMovesOf(CHESSID chessid, int x, int y)
{
    CHESSDEF chessdef = toChessdef(chessid);
    TEAM team = toTeam(chessid);

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
        Log::error("ERROR CHESSID in Board::getMovesOf!");
        return TARGETS{};
    }
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
            CHESSID chessid = chessOn(colIndex, rawIndex);
            string name = getName(chessid);
            cout << name;
        }
        cout << endl;
    }
    cout << endl;
}

/// @brief 人机做出决定
/// @param depth 深度
void Board::makeDecision(int depth)
{
    Node root = Node{MIN_NUMBER, MAX_NUMBER, isRedTurn ? MAX : MIN};
    Node bestNode = evaluateBestNode(0, depth, root);
    Action v = bestNode.action;
    moveTo(v.x1, v.y1, v.x2, v.y2);
    cout << "分数：" << bestNode.score << endl;
}

// -----工具函数----- //

/// @brief 获取指定位置的棋子队伍
TEAM Board::teamOn(int x, int y)
{
    return toTeam(chessMap.on(x, y));
}

/// @brief 过滤棋子行棋范围外的位置和己方棋子占据的位置，即排除不可能的行棋方案
/// @param originalMoves 全部可行着法
/// @param chessid 需要根据棋子id来判断这个棋子的行棋范围
TARGETS Board::removeImpossible(TARGETS originalMoves, CHESSDEF chessdef, TEAM team)
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
            if (v.x >= 0 && v.x <= 8 && v.y >= 0 && v.y <= 9 && teamOn(v.x, v.y) != team)
            {
                moves.push_back(Position(v.x, v.y));
            }
        }
    }
    return moves;
}

/// @brief 获取棋盘上某个位置的棋子
CHESSID Board::chessOn(int x, int y)
{
    if (x >= 0 && x <= 8 && y >= 0 && y <= 9)
    {
        return chessMap.on(x, y);
    }
    else
    {
        return -1;
    }
}

/// @brief 移动棋子
void Board::moveTo(int x1, int y1, int x2, int y2)
{
    chessMap.on(x2, y2) = chessMap.on(x1, y1);
    chessMap.on(x1, y1) = 0;
    isRedTurn = !isRedTurn;
}

// -----棋子函数----- //

/// @brief 获取帅、将的所有走法
TARGETS Board::king(int x, int y, TEAM team)
{
    TARGETS originalMoves = {Position(x + 1, y), Position(x - 1, y),
                             Position(x, y + 1), Position(x, y - 1)};
    TARGETS moves = removeImpossible(originalMoves, KING, team);

    // 白脸将规则

    array<CHESSID, 10> col = chessMap.lineAt(x);
    Position enemyKingPosition(-1, -1);
    bool state = false; // false：未发现将帅棋子之一；true：发现其中一个
    for (int i_y = 0; i_y < 10; i_y++)
    {
        CHESSID chessid = col.at(i_y);
        // 记录将帅棋子
        if (toChessdef(chessid) == KING)
        {
            if (toTeam(chessid) != team)
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
    TARGETS originalMoves = {Position(x + 1, y + 1), Position(x - 1, y - 1),
                             Position(x - 1, y + 1), Position(x + 1, y - 1)};
    TARGETS moves = removeImpossible(originalMoves, GUARD, team);
    return moves;
}

/// @brief 获取相所有走法
TARGETS Board::bishop(int x, int y, TEAM team)
{
    TARGETS originalMoves;

    Position pos(x + 1, y + 1);
    if (chessOn(pos.x, pos.y) == 0)
        originalMoves.push_back(Position(x + 2, y + 2));

    pos.x = x + 1;
    pos.y = y - 1;
    if (chessOn(pos.x, pos.y) == 0)
        originalMoves.push_back(Position(x + 2, y - 2));

    pos.x = x - 1;
    pos.y = y + 1;
    if (chessOn(pos.x, pos.y) == 0)
        originalMoves.push_back(Position(x - 2, y + 2));

    pos.x = x - 1;
    pos.y = y - 1;
    if (chessOn(pos.x, pos.y) == 0)
        originalMoves.push_back(Position(x - 2, y - 2));

    TARGETS moves = removeImpossible(originalMoves, BISHOP, team);
    return moves;
}

/// @brief 获取马所有走法
TARGETS Board::knight(int x, int y, TEAM team)
{
    TARGETS originalMoves;

    Position pos(x + 1, y);
    if (chessOn(pos.x, pos.y) == 0)
    {
        originalMoves.push_back(Position(x + 2, y + 1));
        originalMoves.push_back(Position(x + 2, y - 1));
    }

    pos.x = x - 1;
    if (chessOn(pos.x, pos.y) == 0)
    {
        originalMoves.push_back(Position(x - 2, y + 1));
        originalMoves.push_back(Position(x - 2, y - 1));
    }

    pos.x = x;
    pos.y = y + 1;
    if (chessOn(pos.x, pos.y) == 0)
    {
        originalMoves.push_back(Position(x - 1, y + 2));
        originalMoves.push_back(Position(x + 1, y + 2));
    }

    pos.y = y - 1;
    if (chessOn(pos.x, pos.y) == 0)
    {
        originalMoves.push_back(Position(x - 1, y - 2));
        originalMoves.push_back(Position(x + 1, y - 2));
    }

    TARGETS moves = removeImpossible(originalMoves, BISHOP, team);
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
        if (chessOn(pos.x, pos.y) == 0)
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
        if (chessOn(pos.x, pos.y) == 0)
        {
            moves.push_back(pos);
        }
        else
        {
            for (pos.x++; pos.x <= 8; pos.x++)
            {
                CHESSID chessid = chessOn(pos.x, pos.y);
                if (chessid == 0)
                {
                    continue;
                }
                else if (toTeam(chessid) == team)
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
        if (chessOn(pos.x, pos.y) == 0)
        {
            moves.push_back(pos);
        }
        else
        {
            for (pos.x--; pos.x >= 0; pos.x--)
            {
                CHESSID chessid = chessOn(pos.x, pos.y);
                if (chessid == 0)
                {
                    continue;
                }
                else if (toTeam(chessid) == team)
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
        if (chessOn(pos.x, pos.y) == 0)
        {
            moves.push_back(pos);
        }
        else
        {
            for (pos.y++; pos.y <= 9; pos.y++)
            {
                CHESSID chessid = chessOn(pos.x, pos.y);
                if (chessid == 0)
                {
                    continue;
                }
                else if (toTeam(chessid) == team)
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
        if (chessOn(pos.x, pos.y) == 0)
        {
            moves.push_back(pos);
        }
        else
        {
            for (pos.y--; pos.y >= 0; pos.y--)
            {
                CHESSID chessid = chessOn(pos.x, pos.y);
                if (chessid == 0)
                {
                    continue;
                }
                else if (toTeam(chessid) == team)
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
            moves = removeImpossible(originalMoves, PAWN, team);
        }
        else
        {
            originalMoves.push_back(Position(x, y + 1));
            originalMoves.push_back(Position(x + 1, y));
            originalMoves.push_back(Position(x - 1, y));
            moves = removeImpossible(originalMoves, PAWN, team);
        }
    }
    else
    {
        if (x >= 0 && x <= 8 && y >= 5 && y <= 9)
        {
            originalMoves.push_back(Position(x, y - 1));
            moves = removeImpossible(originalMoves, PAWN, team);
        }
        else
        {
            originalMoves.push_back(Position(x, y - 1));
            originalMoves.push_back(Position(x + 1, y));
            originalMoves.push_back(Position(x - 1, y));
            moves = removeImpossible(originalMoves, PAWN, team);
        }
    }

    return moves;
}

// -----自动走棋实现----- //

/// @brief 获取指定位置上的棋子的分数
int Board::getScore(int x, int y)
{
    return calculateScore(chessMap, x, y);
}

/// @brief 获取某一个队伍所有可行着法
/// @param team 队伍
/// @return 所有可行着法的vector
ACTIONS Board::getAllActionsOfTeam(TEAM team)
{
    ACTIONS result{};
    int x = 0, y = 0;
    bool kingDetected = false; // 是否有将

    for (auto col : chessMap.main)
    {
        for (auto chessid : col)
        {
            if (toTeam(chessid) == team)
            {
                TARGETS targets = getMovesOf(chessid, x, y);
                for (auto v : targets)
                {
                    result.push_back(Action(x, y, v.x, v.y));
                }
                if (chessid == R_KING || chessid == B_KING)
                {
                    kingDetected = true;
                }
            }
            y++;
        }
        x++;
        y = 0;
    }
    if (kingDetected)
    {
        return result;
    }
    else // 如果将被吃了，则禁止所有棋子着法
    {
        return ACTIONS{};
    }
}

/// @brief 评估最佳着法（函数外调用时须传空父节点）
/// @param board Board对象
/// @param depth 深度（用于递归）
/// @param maxDepth 最大深度（用于递归）
Node Board::evaluateBestNode(int depth, int maximumDepth, Node &father)
{
    searchCount++;
    // 节点类型
    NODE_TYPE currentType = !father.type;
    // 列出的所有可行着法
    ACTIONS availableActions = this->getAllActionsOfTeam(isRedTurn ? RED : BLACK);
    // 着法分数表
    vector<int> scores{};

    // 遍历
    for (Action v : availableActions)
    {
        // 初始化
        Node node{father.alpha, father.beta, currentType};
        node.action = v;
        node.scoreCumulation = father.scoreCumulation + this->getScore(v.x2, v.y2);

        // 若没有到达最大深度
        if (depth < maximumDepth)
        {
            // 创建一个虚拟棋盘，并假装走棋
            Board boardCopy = *this;
            boardCopy.moveTo(v.x1, v.y1, v.x2, v.y2);
            // 获取最佳子节点
            Node bestChild = boardCopy.evaluateBestNode(depth + 1, maximumDepth, node);
            // 设置节点分数
            node.score = bestChild.score;
        }
        else
        {
            // 设置节点分数
            node.score = node.scoreCumulation;
        }

        // 向父节点推送当前节点
        father.children.push_back(node);
        // 分数表内推送当前节点
        scores.push_back(node.score);

        // Alpha beta剪枝算法
        if (father.type == MAX && node.score > father.alpha)
        {
            father.alpha = node.score;
        }
        else if (father.type == MIN && node.score < father.beta)
        {
            father.beta = node.score;
        }
        if (father.alpha > father.beta)
        {
            break;
        }
    }

    // 若是没有任何可行着法
    if (father.children.empty())
    {
        // 创建一个最差分数的着法并传给父亲
        Node placeholder{father.alpha, father.beta, currentType};
        placeholder.score = this->isRedTurn ? MIN_NUMBER : MAX_NUMBER;
        return placeholder;
    }

    // 获取最好的节点
    int bestIndex = -1;
    if (father.type == MAX)
    {
        bestIndex = getIndexOfMax<vector<int>>(scores);
    }
    else
    {
        bestIndex = getIndexOfMin<vector<int>>(scores);
    }

    // 返回给父亲
    return father.children.at(bestIndex);
}
