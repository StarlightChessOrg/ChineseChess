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
    Action() : x1(0), y1(0), x2(0), y2(0) {}
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
    CHESSID &on(int x, int y);
    array<CHESSID, 10> lineAt(int x);
};

/// @brief 访问棋盘
CHESSID &ChessMap::on(int x, int y)
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

/// @brief 搜索节点
struct Node
{
    Node(int alpha, int beta, bool type) : alpha(alpha), beta(beta), type(type) {};

    int alpha;
    int beta;
    bool type;

    int score = 0; // 节点分数
    int scoreCumulation = 0; // 分数累计
    Action action {}; // 节点着法

    vector<Node> children {}; // 子节点
};
