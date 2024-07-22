#pragma once
#include "defines.hpp"

/// @brief 日志
class Log
{
  public:
    static void error(string msg = "An Error Occured")
    {
        cerr << msg << endl;
        system("pause");
    }
};

/// @brief 获取数组（或其他可迭代容器如vector）中最大值索引
template <typename T> int getIndexOfMax(T array)
{
    int maxIndex = 0;
    int maxNumber = array.at(0);

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

/// @brief 获取数组（或其他可迭代容器如vector）中最小值索引
template <typename T> int getIndexOfMin(T array)
{
    int minIndex = 0;
    int minNumber = array.at(0);

    for (int i = 0; i < array.size(); i++)
    {
        int number = array.at(i);
        if (number < minNumber)
        {
            minNumber = number;
            minIndex = i;
        }
    }

    return minIndex;
}

/// @brief 位置类
struct Position
{
    Position(int x, int y) : x(x), y(y) {};
    int x;
    int y;
};

using TARGETS = vector<Position>;

/// @brief 着法结构
struct Action
{
    Action(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {};
    Action()
    {
    } // 空构造函数
    int x1;
    int y1;
    int x2;
    int y2;
};

using ACTIONS = vector<Action>;

/// @brief 棋盘类
struct ChessMap
{
    ChessMap(CHESSMAP chessMap) : main(chessMap) {};
    CHESSMAP main;
    CHESSID &at(int x, int y);
    array<CHESSID, 10> lineAt(int x);
};

/// @brief 访问棋盘
CHESSID &ChessMap::at(int x, int y)
{
    if (x > 8 || x < 0 || y > 9 || y < 0)
    {
        Log::error("Error when accessing chessmap: out of range");
    }
    CHESSID &v = this->main[x][y];
    return v;
}

/// @brief 获取一行的棋
array<CHESSID, 10> ChessMap::lineAt(int x)
{
    if (x > 8 || x < 0)
    {
        Log::error("Error when accessing chessmap: out of range");
    }
    array<CHESSID, 10> v = this->main[x];
    return v;
}

enum NODE_TYPE
{
    MIN,
    MAX
};

/// @brief 搜索节点
struct SearchNode
{
    SearchNode() {};
    SearchNode(int alpha, int beta) : alpha(alpha), beta(beta) {};
    Action bestAction;
    ACTIONS childActions;
    vector<int> childScores;
    vector<SearchNode> children;
    NODE_TYPE type;
    int score;
    int alpha;
    int beta;
};