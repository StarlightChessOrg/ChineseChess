#pragma once
#include "genMove.hpp"

static const int DRAW_MOVE_NUM = 100;

// 开中局、有进攻机会的帅(将)和兵(卒)，参照“象眼”
const int kingPawnMidgameAttacking[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
        0,  0,  0, 39, 49, 69, 84, 89, 84, 69, 49, 39,  0,  0,  0,  0,
        0,  0,  0, 39, 49, 64, 74, 74, 74, 64, 49, 39,  0,  0,  0,  0,
        0,  0,  0, 39, 46, 54, 59, 61, 59, 54, 46, 39,  0,  0,  0,  0,
        0,  0,  0, 29, 37, 41, 54, 59, 54, 41, 37, 29,  0,  0,  0,  0,
        0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 开中局、没有进攻机会的帅(将)和兵(卒)
const int kingPawnMidgameAttackless[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
        0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
        0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
        0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
        0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
        0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 残局、有进攻机会的帅(将)和兵(卒)
const int kingPawnEndgameAttacking[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 10, 10, 10, 15, 15, 15, 10, 10, 10,  0,  0,  0,  0,
        0,  0,  0, 50, 55, 60, 85,100, 85, 60, 55, 50,  0,  0,  0,  0,
        0,  0,  0, 65, 70, 70, 75, 75, 75, 70, 70, 65,  0,  0,  0,  0,
        0,  0,  0, 75, 80, 80, 80, 80, 80, 80, 80, 75,  0,  0,  0,  0,
        0,  0,  0, 70, 70, 65, 70, 70, 70, 65, 70, 70,  0,  0,  0,  0,
        0,  0,  0, 45,  0, 40, 45, 45, 45, 40,  0, 45,  0,  0,  0,  0,
        0,  0,  0, 40,  0, 35, 40, 40, 40, 35,  0, 40,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  5,  5, 15,  5,  5,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  3,  3, 13,  3,  3,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  1,  1, 11,  1,  1,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 残局、没有进攻机会的帅(将)和兵(卒)
const int kingPawnEndgameAttackless[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 10, 10, 10, 15, 15, 15, 10, 10, 10,  0,  0,  0,  0,
        0,  0,  0, 10, 15, 20, 45, 60, 45, 20, 15, 10,  0,  0,  0,  0,
        0,  0,  0, 25, 30, 30, 35, 35, 35, 30, 30, 25,  0,  0,  0,  0,
        0,  0,  0, 35, 40, 40, 45, 45, 45, 40, 40, 35,  0,  0,  0,  0,
        0,  0,  0, 25, 30, 30, 35, 35, 35, 30, 30, 25,  0,  0,  0,  0,
        0,  0,  0, 25,  0, 25, 25, 25, 25, 25,  0, 25,  0,  0,  0,  0,
        0,  0,  0, 20,  0, 20, 20, 20, 20, 20,  0, 20,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  5,  5, 13,  5,  5,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  3,  3, 12,  3,  3,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  1,  1, 11,  1,  1,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 没受威胁的仕(士)和相(象)
const int advisorBishopThreatless[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 18,  0,  0, 20, 23, 20,  0,  0, 18,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 20, 20,  0, 20, 20,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 受到威胁的仕(士)和相(象)，参照“象眼”
const int advisorBishopThreatened[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 40,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 38,  0,  0, 40, 43, 40,  0,  0, 38,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 43,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 40, 40,  0, 40, 40,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 开中局的马，参照“象眼”
const int knightMidgame[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
        0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
        0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
        0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
        0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
        0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
        0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
        0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
        0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
        0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 残局的马
const int knightEndgame[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 92, 94, 96, 96, 96, 96, 96, 94, 92,  0,  0,  0,  0,
        0,  0,  0, 94, 96, 98, 98, 98, 98, 98, 96, 94,  0,  0,  0,  0,
        0,  0,  0, 96, 98,100,100,100,100,100, 98, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 98,100,100,100,100,100, 98, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 98,100,100,100,100,100, 98, 96,  0,  0,  0,  0,
        0,  0,  0, 94, 96, 98, 98, 98, 98, 98, 96, 94,  0,  0,  0,  0,
        0,  0,  0, 94, 96, 98, 98, 98, 98, 98, 96, 94,  0,  0,  0,  0,
        0,  0,  0, 92, 94, 96, 96, 96, 96, 96, 94, 92,  0,  0,  0,  0,
        0,  0,  0, 90, 92, 94, 92, 92, 92, 94, 92, 90,  0,  0,  0,  0,
        0,  0,  0, 88, 90, 92, 90, 90, 90, 92, 90, 88,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 开中局的车，参照“象眼”
const int rookMidgame[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
        0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
        0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
        0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
        0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
        0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
        0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
        0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
        0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
        0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 残局的车
const int rookEndgame[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,182,182,182,184,186,184,182,182,182,  0,  0,  0,  0,
        0,  0,  0,184,184,184,186,190,186,184,184,184,  0,  0,  0,  0,
        0,  0,  0,182,182,182,184,186,184,182,182,182,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,180,180,180,182,184,182,180,180,180,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 开中局的炮，参照“象眼”
const int cannonMidgame[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
        0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
        0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
        0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
        0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
        0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
        0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
        0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 残局的炮
const int cannonEndgame[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,100,100,100,100,100,100,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,100,100,100,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,100,100,100,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,102,104,102,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,102,104,102,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,102,104,102,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,102,104,102,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,102,104,102,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,104,106,104,100,100,100,  0,  0,  0,  0,
        0,  0,  0,100,100,100,104,106,104,100,100,100,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 空头炮的威胁分值，指标是对红方来说的行号(即黑方要用15去减)，大体上空头炮位置越高威胁越大。进入残局时，该值要相应减少。
static const int hollowThreat[16] = {
        0,  0,  0,  0,  0,  0, 60, 65, 70, 75, 80, 80, 80,  0,  0,  0
};

// 炮镇窝心马的威胁分值，指标同上，大体上高度越低威胁越大，没有窝心马时可取四分之一。进入残局时，取值似乎不应变化。
static const int centralThreat[16] = {
        0,  0,  0,  0,  0,  0, 50, 45, 40, 35, 30, 30, 30,  0,  0,  0
};

// 沉底炮的威胁分值，指标是列号，大体上越靠近边线威胁越大。威胁减少时，该值要相应减少。
static const int bottomThreat[16] = {
        0,  0,  0, 40, 30,  0,  0,  0,  0,  0, 30, 40,  0,  0,  0,  0
};

// 计算游戏进行到的状态
static const int ROOK_MIDGAME_VALUE = 6;
static const int KNIGHT_CANNON_MIDGAME_VALUE = 3;
static const int OTHER_MIDGAME_VALUE = 1;
static const int TOTAL_MIDGAME_VALUE = ROOK_MIDGAME_VALUE * 4 + KNIGHT_CANNON_MIDGAME_VALUE * 8 + OTHER_MIDGAME_VALUE * 18;
static const int TOTAL_ADVANCED_VALUE = 4;
static const int TOTAL_ATTACK_VALUE = 8;
static const int ADVISOR_BISHOP_ATTACKLESS_VALUE = 90;
static const int TOTAL_ADVISOR_LEAKAGE = 80;

// 偷懒评价的边界
const int EVAL_MARGIN1 = 160;
const int EVAL_MARGIN2 = 80;
const int EVAL_MARGIN3 = 40;
const int EVAL_MARGIN4 = 20;

// 常数表"inKnightEdgeSquares"给定了不利于马的位置，处于棋盘边缘和两个花心位置的马都是坏马
static const int inKnightEdge[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 中间子和被牵制子的距离越近，牵制的价值就越大
static const int stringValueTab[512] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
        12, 16, 20, 24, 28, 32, 36,  0, 36, 32, 28, 24, 20, 16, 12,  0,
        0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0
};

enum repStatus{
    none_rep = 0,
    draw_rep = 1,
    kill_rep = 2,
    killed_rep = 3
};

class evaluate : public position{
public:
    explicit evaluate(const int anotherBoard[256] = initGameBoard, int initSide = red) : position(anotherBoard,initSide){
        clearEvaBoard();
    }
    void clearEvaBoard(){
        vlRed = 0;
        vlBlack = 0;
        vlFirstGo = 0;
        memset(vlRedBoard,0,sizeof(int) * 7 * 256);
        memset(vlBlackBoard,0,sizeof(int) * 7 * 256);
    }
    bool makeMove(int fromPos,int toPos) {
        const int fromPiece = position::board.getPieceByPos(fromPos);
        const int fromIndex = swapBasicBoard::pieceToAbsType(fromPiece) - 1;
        const int toPiece = position::board.getPieceByPos(toPos);
//        if(abs(toPiece) == 1){
//            cout<<endl;
//        }
        //检查将军
        position::makeMove(fromPos,toPos);
        const bool originSideCheck = genMove::CheckedBy(*this,-position::side);
        if(originSideCheck){
            position::unMakeMove(fromPos,toPos,fromPiece,toPiece);
            return false;
        }
        //步进
        if(fromPiece > 0){
            this->vlRed += vlRedBoard[fromIndex][toPos] - vlRedBoard[fromIndex][fromPos];
        }else if(fromPiece < 0){
            this->vlBlack += vlBlackBoard[fromIndex][toPos] - vlBlackBoard[fromIndex][fromPos];
        }
        if(toPiece){
            const int toIndex = swapBasicBoard::pieceToAbsType(toPiece) - 1;
            if(toPiece > 0){
                this->vlRed -= vlRedBoard[toIndex][toPos];
            }else{
                this->vlBlack -= vlBlackBoard[toIndex][toPos];
            }
        }
        //优先记录走法
        moveRoad.emplace_back(fromPos,toPos,fromPiece,toPiece);
        //判断对方是否被我方将军
        const bool chase = genMove::ChasedBy(*this,moveRoad.back());
        const bool check = genMove::CheckedBy(*this,position::side);
        const int lastDrawMoveSum = drawMoveStatus.empty() ? 0 : drawMoveStatus.back();
        if(check || (!checkMoveStatus.empty() && checkMoveStatus.back())){
           drawMoveStatus.push_back(lastDrawMoveSum);
        }else if(toPiece){
            drawMoveStatus.push_back(0);
        }else {
            drawMoveStatus.push_back(lastDrawMoveSum + 1);
        }
        //记录剩余棋规状态
        checkMoveStatus.push_back(check);
        chaseMoveStatus.push_back(chase);
        return true;
    }
    void unMakeMove() {
        const int fromPos = moveRoad.back().fromPos;
        const int toPos = moveRoad.back().toPos;
        const int fromPiece = moveRoad.back().fromPiece;
        const int toPiece = moveRoad.back().toPiece;
        const int fromIndex = swapBasicBoard::pieceToAbsType(fromPiece) - 1;
        if(fromPiece > 0){
            this->vlRed -= vlRedBoard[fromIndex][toPos] - vlRedBoard[fromIndex][fromPos];
        }else if(fromPiece < 0){
            this->vlBlack -= vlBlackBoard[fromIndex][toPos] - vlBlackBoard[fromIndex][fromPos];
        }
        if(toPiece){
            const int toIndex = swapBasicBoard::pieceToAbsType(toPiece) - 1;
            if(toPiece > 0){
                this->vlRed += vlRedBoard[toIndex][toPos];
            }else{
                this->vlBlack += vlBlackBoard[toIndex][toPos];
            }
        }
        position::unMakeMove(fromPos,toPos,fromPiece,toPiece);
        //撤销棋规记录
        assert(!moveRoad.empty());
        drawMoveStatus.pop_back();
        checkMoveStatus.pop_back();
        chaseMoveStatus.pop_back();
        moveRoad.pop_back();
    }
    int getEvaluate(int side,int vlAlpha,int vlBeta){
        // 四级偷懒评价(彻底偷懒评价)，只包括子力平衡；
        int vl = this->material(side);
        if (vl + EVAL_MARGIN1 <= vlAlpha) {
            return vl + EVAL_MARGIN1;
        } else if (vl - EVAL_MARGIN1 >= vlBeta) {
            return vl - EVAL_MARGIN1;
        }

        // 三级偷懒评价，包括特殊棋型；
        vl += this->advisorShape(side);
        if (vl + EVAL_MARGIN2 <= vlAlpha) {
            return vl + EVAL_MARGIN2;
        } else if (vl - EVAL_MARGIN2 >= vlBeta) {
            return vl - EVAL_MARGIN2;
        }

        // 二级偷懒评价，包括牵制；
        vl += this->stringHold(side);
        if (vl + EVAL_MARGIN3 <= vlAlpha) {
            return vl + EVAL_MARGIN3;
        } else if (vl - EVAL_MARGIN3 >= vlBeta) {
            return vl - EVAL_MARGIN3;
        }

        // 一级偷懒评价，包括车的灵活性；
        vl += this->rookMobility(side);
        if (vl + EVAL_MARGIN4 <= vlAlpha) {
            return vl + EVAL_MARGIN4;
        } else if (vl - EVAL_MARGIN4 >= vlBeta) {
            return vl - EVAL_MARGIN4;
        }

        // 零级偷懒评价(完全评价)，包括马的阻碍。
        return vl + this->knightTrap(side);
    }
protected:
    //判断是否自然和棋
    bool isDraw(){
        return !drawMoveStatus.empty() && drawMoveStatus.back() >= DRAW_MOVE_NUM;
    }
    //判断走法线路的重复类型
    bool isRep(){
        if(getNowDistance() >= 4){
            const bool mineFirstCheck = checkMoveStatus[checkMoveStatus.size() - 3];
            const bool mineSecondCheck = checkMoveStatus[checkMoveStatus.size() - 1];
            const bool otherFirstCheck = checkMoveStatus[checkMoveStatus.size() - 4];
            const bool otherSecondCheck = checkMoveStatus[checkMoveStatus.size() - 2];

            const bool mineFirstChase = chaseMoveStatus[chaseMoveStatus.size() - 3];
            const bool mineSecondChase = chaseMoveStatus[chaseMoveStatus.size() - 1];
            const bool otherFirstChase = chaseMoveStatus[chaseMoveStatus.size() - 4];
            const bool otherSecondChase = chaseMoveStatus[chaseMoveStatus.size() - 2];

            const int mineCheck = (mineFirstCheck && mineSecondCheck);
            const int mineChase = (mineFirstChase && mineSecondChase);
            const int otherCheck = (otherFirstCheck && otherSecondCheck);
            const int otherChase = (otherFirstChase && otherSecondChase);

            const int mineRepLevel = mineChase + mineCheck * 2;
            const int otherRepLevel = otherChase + otherCheck * 2;

            if(mineRepLevel && otherRepLevel){
                if(mineRepLevel == otherRepLevel){
                    return draw_rep;
                }else if(mineRepLevel > otherRepLevel){
                    return killed_rep;
                }else{
                    return kill_rep;
                }
            }else{
                return none_rep;
            }
        }
        return none_rep;
    }
    //获取路线长度
    int getNowDistance(){
        return (int)moveRoad.size();
    }
    //更新估值矩阵
    void resetEvaBoard(){
        int midgameValue = 0;
        int redAttackValue = 0;
        int blackAttackValue = 0;
        int pawnPieceAttacking[256] = {0};
        int pawnPieceAttackless[256] = {0};
        // 首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加
        // 然后判断各方是否处于进攻状态，方法是计算各种过河棋子的数量，按照车马2炮兵1相加
        getMidgameValue(midgameValue);
        getAttackValue(redAttackValue,blackAttackValue);
        //计算先行泉下分值
        vlFirstGo = (TOTAL_ADVANCED_VALUE * midgameValue + TOTAL_ADVANCED_VALUE / 2) / TOTAL_MIDGAME_VALUE;
        //初始化估值矩阵
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                //将兵
                const int vlKingOrPawn = (kingPawnMidgameAttacking[pos] * midgameValue + kingPawnEndgameAttacking[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(king)][pos] = vlBlackBoard[vlIndex(king)][xyMirrorPos(pos)] = vlKingOrPawn;
                pawnPieceAttacking[pos] = vlKingOrPawn;
                pawnPieceAttackless[pos] = (kingPawnMidgameAttackless[pos] * midgameValue + kingPawnEndgameAttackless[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                //马
                const int vlKnight = (knightMidgame[pos] * midgameValue + knightEndgame[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(knight)][pos] = vlBlackBoard[vlIndex(knight)][xyMirrorPos(pos)] = vlKnight;
                //炮
                const int vlCannon = (cannonMidgame[pos] * midgameValue + cannonEndgame[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(cannon)][pos] = vlBlackBoard[vlIndex(cannon)][xyMirrorPos(pos)] = vlCannon;
                //车
                const int vlRook = (rookMidgame[pos] * midgameValue + rookEndgame[pos] * (TOTAL_MIDGAME_VALUE - midgameValue)) / TOTAL_MIDGAME_VALUE;
                vlRedBoard[vlIndex(rook)][pos] = vlBlackBoard[vlIndex(rook)][xyMirrorPos(pos)] = vlRook;
            }
        }
        //空头炮(中炮)和窝心马的威胁向量
        for (int i = 0;i < 16;i++) {
            vlHollowThreat[i] = hollowThreat[i] * (midgameValue + TOTAL_MIDGAME_VALUE) / (TOTAL_MIDGAME_VALUE * 2);
            vlCentralThreat[i] = centralThreat[i];
        }
        //计算士的危险程度
        vlBlackAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * redAttackValue / TOTAL_ATTACK_VALUE;
        vlRedAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * blackAttackValue / TOTAL_ATTACK_VALUE;
        //完善士象兵的估值矩阵
        for(int pos = 51;pos < 205;pos++){
            if(inBoard[pos]){
                //士象
                const int vlAdvisorOrBishop = (advisorBishopThreatened[pos] * blackAttackValue + (advisorBishopThreatless[pos]) * (TOTAL_ATTACK_VALUE - blackAttackValue)) / TOTAL_ATTACK_VALUE;
                vlRedBoard[vlIndex(advisor)][pos] = vlBlackBoard[vlIndex(advisor)][xyMirrorPos(pos)] = vlAdvisorOrBishop;
                vlRedBoard[vlIndex(bishop)][pos] = vlBlackBoard[vlIndex(bishop)][xyMirrorPos(pos)] = vlAdvisorOrBishop;
                //兵
                const int vlRedPawn = (pawnPieceAttacking[pos] * redAttackValue + pawnPieceAttackless[pos] * (TOTAL_ATTACK_VALUE - redAttackValue)) / TOTAL_ATTACK_VALUE;
                const int vlBlackPawn = (pawnPieceAttacking[xyMirrorPos(pos)] * blackAttackValue + pawnPieceAttackless[xyMirrorPos(pos)] * (TOTAL_ATTACK_VALUE - blackAttackValue)) / TOTAL_ATTACK_VALUE;
                vlRedBoard[vlIndex(pawn)][pos] = vlRedPawn;
                vlBlackBoard[vlIndex(pawn)][pos] = vlBlackPawn;
            }
        }
        //计算沉底炮的威胁向量
        for (int i = 0; i < 16; i ++) {
            vlRedBottomThreat[i] = bottomThreat[i] * blackAttackValue / TOTAL_ATTACK_VALUE;
            vlBlackBottomThreat[i] = bottomThreat[i] * redAttackValue / TOTAL_ATTACK_VALUE;
        }
        // 调整不受威胁方少掉的仕(士)相(象)分值
        this->vlRed = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - blackAttackValue) / TOTAL_ATTACK_VALUE;
        this->vlBlack = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - redAttackValue) / TOTAL_ATTACK_VALUE;

        // 最后重新计算子力位置分
        for(int pos = 51;pos < 205;pos++){
            const int piece = board.getPieceByPos(pos);
            const int index = vlIndex(swapBasicBoard::pieceToAbsType(piece));
            if(piece > 0){
                this->vlRed += vlRedBoard[index][pos];
            }else if(piece < 0){
                this->vlBlack += vlBlackBoard[index][pos];
            }
        }
    }
private:
    //物质分
    int material(int side) const{
        if(side == red){
            return vlRed - vlBlack + vlFirstGo;
        }
        return vlBlack - vlRed + vlFirstGo;
    }
    //车的机动性
    int rookMobility(int side){
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        const int stepList[4] = {1,1,16,16};
        int vlRedMobility = 0;
        int vlBlackMobility = 0;
        for(int piece : redRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int i = 0;i < 4;i++){
                    const int targetPos = position::bitBoard.getRayTargetPos(pos,targetPool[i],0);
                    if(targetPos > -1){
                        vlRedMobility += abs(targetPos - pos) / stepList[i];
                        if(position::board.getPieceByPos(targetPos) * piece > 0){
                            vlRedMobility--;
                        }
                    }
                }
            }
        }
        for(int piece : blackRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int i = 0;i < 4;i++){
                    const int targetPos = position::bitBoard.getRayTargetPos(pos,targetPool[i],0);
                    if(targetPos > -1){
                        vlBlackMobility += abs(targetPos - pos) / stepList[i];
                        if(position::board.getPieceByPos(targetPos) * piece > 0){
                            vlBlackMobility--;
                        }
                    }
                }
            }
        }
        if(side == red){
            return (vlRedMobility - vlBlackMobility) / 2;
        }
        return (vlBlackMobility - vlRedMobility) / 2;
    }
    //劣马
    int knightTrap(int side){
        int vlRedKnightTrap = 0;
        int vlBlackKnightTrap = 0;
        for(int piece : redKnightPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            int redPenalty = 10;
            if(pos){
                for(int step : knightDelta){
                    const int toPos = pos + step;
                    const int legPos = getKnightLeg(pos,toPos);
                    if(!inKnightEdge[toPos] &&
                        !position::board.getPieceByPos(legPos) &&
                        inBoard[toPos]){
                        redPenalty -= 5;
                        if(!redPenalty){
                            break;
                        }
                    }
                }
            }
            vlRedKnightTrap -= redPenalty;
        }
        for(int piece : blackKnightPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            int blackPenalty = 10;
            if(pos){
                for(int step : knightDelta){
                    const int toPos = pos + step;
                    const int legPos = getKnightLeg(pos,toPos);
                    if(!inKnightEdge[toPos] &&
                        !position::board.getPieceByPos(legPos) &&
                        inBoard[toPos]){
                        blackPenalty -= 5;
                        if(!blackPenalty){
                            break;
                        }
                    }
                }
            }
            vlBlackKnightTrap -= blackPenalty;
        }
        if(side == red){
            return vlRedKnightTrap - vlBlackKnightTrap;
        }
        return vlBlackKnightTrap - vlRedKnightTrap;
    }
    //士形
    int advisorShape(int side){
        int vlRedAdvisorShape = 0;
        int vlBlackAdvisorShape = 0;
        vector<int> redAdvisorPos;
        vector<int> blackAdvisorPos;
        for(int piece : redAdvisorPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                redAdvisorPos.push_back(pos);
            }
        }
        for(int piece : blackAdvisorPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                blackAdvisorPos.push_back(pos);
            }
        }

        int redRookSum = 0;
        int blackRookSum = 0;
        for(int piece : redRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                redRookSum++;
            }
        }
        for(int piece : blackAdvisorPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                blackRookSum++;
            }
        }

        //缺士怕双车
        if(redAdvisorPos.size() < 2 && blackRookSum == 2){
            vlRedAdvisorShape -= vlRedAdvisorLeakage;
        }
        if(blackAdvisorPos.size() < 2 && redRookSum == 2){
            vlBlackAdvisorShape -= vlBlackAdvisorLeakage;
        }

        const int redKingPos = position::swapBoard.getPosByPiece(redKingPiece);
        const int blackKingPos = position::swapBoard.getPosByPiece(blackKingPiece);
        assert(redKingPos && blackKingPos);

        //有双士但是将占花心要扣分
        if(redAdvisorPos.size() == 2 && inFortCenter[redKingPos]){
            vlRedAdvisorShape -= 20;
        }
        if(blackAdvisorPos.size() == 2 && inFortCenter[blackKingPos]){
            vlBlackAdvisorShape -= 20;
        }

        //双十和将在底线原位置
        if(redAdvisorPos.size() == 2 && inKingFortBottom[redKingPos]){
            if(inAdvisorFortBottom[redAdvisorPos.front()] && inAdvisorFortBottom[redAdvisorPos.back()]){
                const int toPos = position::bitBoard.getRayTargetPos(redKingPos,upTarget,0);
                if(toPos > -1){
                    const int toPiece = position::board.getPieceByPos(toPos);
                    const int toType = swapBasicBoard::pieceToAbsType(toPiece);
                    //空头炮
                    if(toType == cannon && toPiece < 0){
                        vlRedAdvisorShape -= hollowThreat[getY(toPos)];
                    }
                    //窝心马
                    if(toType == knight && toPiece > 0){
                        const int toInvInvPos = position::bitBoard.getRayTargetPos(redKingPos,upTarget,2);
                        if(toInvInvPos > -1){
                            const int toInvInvPiece = position::board.getPieceByPos(toInvInvPos);
                            const int toInvInvType = swapBasicBoard::pieceToAbsType(toInvInvPiece);
                            if(toInvInvType == cannon && toInvInvPiece < 0){
                                vlRedAdvisorShape -= vlCentralThreat[getY(toPos)];
                            }
                        }
                    }
                }
            }
        }
        if(blackAdvisorPos.size() == 2 && inKingFortBottom[blackKingPos]){
            if(inAdvisorFortBottom[blackAdvisorPos.front()] && inAdvisorFortBottom[blackAdvisorPos.back()]){
                const int toPos = position::bitBoard.getRayTargetPos(blackKingPos,downTarget,0);
                if(toPos > -1){
                    const int toPiece = position::board.getPieceByPos(toPos);
                    const int toType = swapBasicBoard::pieceToAbsType(toPiece);
                    //空头炮
                    if(toType == cannon && toPiece > 0){
                        vlBlackAdvisorShape -= hollowThreat[yMirrorPos(getY(toPos))];
                    }
                    //炮镇窝心马
                    if(toType == knight && toPiece < 0){
                        const int toInvInvPos = position::bitBoard.getRayTargetPos(redKingPos,downTarget,2);
                        if(toInvInvPos > -1){
                            const int toInvInvPiece = position::board.getPieceByPos(toInvInvPos);
                            const int toInvInvType = swapBasicBoard::pieceToAbsType(toInvInvPiece);
                            if(toInvInvType == cannon && toInvInvPiece > 0){
                                vlBlackAdvisorShape -= vlCentralThreat[yMirrorPos(getY(toPos))];
                            }
                        }
                    }
                }
            }
        }

        //将在底线原位置，有双士并且上了正士
        if(redAdvisorPos.size() == 2 && inKingFortBottom[redKingPos]){
            if(inFortCenter[redAdvisorPos.front()] || inFortCenter[redAdvisorPos.back()]){
                if(inAdvisorFortBottom[redAdvisorPos.front()] || inAdvisorFortBottom[redAdvisorPos.back()]){
                    //底线检查
                    const int targetPool[2] = {leftTarget,rightTarget};
                    for(int target : targetPool){
                        const int reverseBottomToPos = position::bitBoard.getRayTargetPos(redKingPos,target,2);
                        if(reverseBottomToPos > -1){
                            const int reverseBottomToPiece = position::board.getPieceByPos(reverseBottomToPos);
                            const int reverseBottomToType = swapBasicBoard::pieceToAbsType(reverseBottomToPiece);
                            if(reverseBottomToType == cannon && reverseBottomToPiece < 0){
                                vlRedAdvisorShape -= vlRedBottomThreat[getX(reverseBottomToPos)];
                            }
                        }
                    }
                    //中路检查
                    const int toInvInvPos = position::bitBoard.getRayTargetPos(redKingPos,upTarget,2);
                    if(toInvInvPos > -1){
                        const int toInvInvPiece = position::board.getPieceByPos(toInvInvPos);
                        const int toInvInvType = swapBasicBoard::pieceToAbsType(toInvInvPiece);
                        if(toInvInvType == cannon && toInvInvPiece < 0){
                            vlRedAdvisorShape -= (vlHollowThreat[getY(toInvInvPos)] >> 2);
                            //检查将门是否被封锁
                            const int kingDoorPos = !position::board.getPieceByPos(redKingPos - 1) ? redKingPos - 1 : redKingPos + 1;
                            if(!position::board.getPieceByPos(redKingPos - 1)){
                                position::makeMove(redKingPos,kingDoorPos);
                                if(genMove::getRelation(*this,redKingPos - 1,beThreatened)){
                                    vlRedAdvisorShape -= 20;
                                    //检查车是否在底线防守将门
                                    const int bottomToPos = position::bitBoard.getRayTargetPos(redKingPos - 1,leftTarget,0);
                                    if(bottomToPos > -1){
                                        const int bottomToPiece = position::board.getPieceByPos(bottomToPos);
                                        const int bottomToType = swapBasicBoard::pieceToAbsType(bottomToPiece);
                                        if(bottomToType == rook && bottomToPiece > 0){
                                            vlRedAdvisorShape -= 80;
                                        }
                                    }
                                }
                                position::unMakeMove(redKingPos,kingDoorPos,redKingPiece,empty);
                            }else if(!position::board.getPieceByPos(redKingPos + 1)){
                                position::makeMove(redKingPos,kingDoorPos);
                                if(genMove::getRelation(*this,redKingPos + 1,beThreatened)){
                                    vlRedAdvisorShape -= 20;
                                    //检查车是否在底线防守将门
                                    const int bottomToPos = position::bitBoard.getRayTargetPos(redKingPos + 1,rightTarget,0);
                                    if(bottomToPos > -1){
                                        const int bottomToPiece = position::board.getPieceByPos(bottomToPos);
                                        const int bottomToType = swapBasicBoard::pieceToAbsType(bottomToPiece);
                                        if(bottomToType == rook && bottomToPiece > 0){
                                            vlRedAdvisorShape -= 80;
                                        }
                                    }
                                }
                                position::unMakeMove(redKingPos,kingDoorPos,redKingPiece,empty);
                            }
                        }
                    }
                }
            }
        }
        if(blackAdvisorPos.size() == 2 && inKingFortBottom[blackKingPos]){
            if(inFortCenter[blackAdvisorPos.front()] || inFortCenter[blackAdvisorPos.back()]){
                if(inAdvisorFortBottom[blackAdvisorPos.front()] || inAdvisorFortBottom[blackAdvisorPos.back()]){
                    //底线检查
                    const int targetPool[2] = {leftTarget,rightTarget};
                    for(int target : targetPool){
                        const int reverseBottomToPos = position::bitBoard.getRayTargetPos(blackKingPos,target,2);
                        if(reverseBottomToPos > -1){
                            const int reverseBottomToPiece = position::board.getPieceByPos(reverseBottomToPos);
                            const int reverseBottomToType = swapBasicBoard::pieceToAbsType(reverseBottomToPiece);
                            if(reverseBottomToType == cannon && reverseBottomToPiece < 0){
                                vlBlackAdvisorShape -= vlBlackBottomThreat[getX(reverseBottomToPos)];
                            }
                        }
                    }
                    //中路检查
                    const int toInvInvPos = position::bitBoard.getRayTargetPos(blackKingPos,upTarget,2);
                    if(toInvInvPos > -1){
                        const int toInvInvPiece = position::board.getPieceByPos(toInvInvPos);
                        const int toInvInvType = swapBasicBoard::pieceToAbsType(toInvInvPiece);
                        if(toInvInvType == cannon && toInvInvPiece > 0){
                            vlBlackAdvisorShape -= (vlHollowThreat[yMirrorPos(getY(toInvInvPos))] >> 2);
                            //检查将门是否被封锁
                            const int kingDoorPos = !position::board.getPieceByPos(blackKingPos - 1) ? blackKingPos - 1 : blackKingPos + 1;
                            if(!position::board.getPieceByPos(blackKingPos - 1)){
                                position::makeMove(blackKingPos,kingDoorPos);
                                if(genMove::getRelation(*this,blackKingPos - 1,beThreatened)){
                                    vlBlackAdvisorShape -= 20;
                                    //检查车是否在底线防守将门
                                    const int bottomToPos = position::bitBoard.getRayTargetPos(blackKingPos - 1,leftTarget,0);
                                    if(bottomToPos > -1){
                                        const int bottomToPiece = position::board.getPieceByPos(bottomToPos);
                                        const int bottomToType = swapBasicBoard::pieceToAbsType(bottomToPiece);
                                        if(bottomToType == rook && bottomToPiece < 0){
                                            vlBlackAdvisorShape -= 80;
                                        }
                                    }
                                }
                                position::unMakeMove(blackKingPos,kingDoorPos,redKingPiece,empty);
                            }else if(!position::board.getPieceByPos(blackKingPos + 1)){
                                position::makeMove(blackKingPos,kingDoorPos);
                                if(genMove::getRelation(*this,blackKingPos + 1,beThreatened)){
                                    vlBlackAdvisorShape -= 20;
                                    //检查车是否在底线防守将门
                                    const int bottomToPos = position::bitBoard.getRayTargetPos(blackKingPos + 1,rightTarget,0);
                                    if(bottomToPos > -1){
                                        const int bottomToPiece = position::board.getPieceByPos(bottomToPos);
                                        const int bottomToType = swapBasicBoard::pieceToAbsType(bottomToPiece);
                                        if(bottomToType == rook && bottomToPiece < 0){
                                            vlBlackAdvisorShape -= 80;
                                        }
                                    }
                                }
                                position::unMakeMove(blackKingPos,kingDoorPos,redKingPiece,empty);
                            }
                        }
                    }
                }
            }
        }
        if(side == red){
            return vlRedAdvisorShape - vlBlackAdvisorShape;
        }
        return vlBlackAdvisorShape - vlRedAdvisorShape;
    }
    //牵制
    int stringHold(int side){
        int vlRedStringHold = 0;
        int vlBlackStringHold = 0;
        const int targetPool[4] = {leftTarget,rightTarget,upTarget,downTarget};
        //车
        for(int piece : redRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int target : targetPool){
                    vlRedStringHold += singleStringHold(pos,target);
                }
            }
        }
        for(int piece : blackRookPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(pos){
                for(int target : targetPool){
                    vlBlackStringHold += singleStringHold(pos,target);
                }
            }
        }
        //将帅
        int pos = position::swapBoard.getPosByPiece(redKingPiece);
        if(pos){
            for(int target : targetPool){
                vlRedStringHold += singleStringHold(pos,target);
            }
        }
        pos = position::swapBoard.getPosByPiece(blackKingPiece);
        if(pos){
            for(int target : targetPool){
                vlBlackStringHold += singleStringHold(pos,target);
            }
        }
        if(side == red){
            return vlRedStringHold - vlBlackStringHold;
        }
        return vlBlackStringHold - vlRedStringHold;
    }
    int singleStringHold(int pos,int target){
        int vlSingleStringHold = 0;
        const int toPos = position::bitBoard.getRayTargetPos(pos,target,0);
        const int toInvPos = position::bitBoard.getRayTargetPos(pos,target,1);
        if(toPos > -1){
            const int toPiece = position::board.getPieceByPos(toPos);
            const int toType = swapBasicBoard::pieceToAbsType(toPiece);
            if(toType == cannon){
                if(toInvPos > -1){
                    const int toInvPiece = position::board.getPieceByPos(toInvPos);
                    const int toInvType = swapBasicBoard::pieceToAbsType(toInvPiece);
                    if(toInvType == rook){
                        if(!genMove::getRelation(*this,toPos,beProtected,pos)){
                            vlSingleStringHold -= stringValueTab[256 + toPos - pos];
                        }
                    }
                }
            }else if(toType == knight){
                if(toInvPos > -1){
                    //受到车的牵制
                    const int toInvPiece = position::board.getPieceByPos(toInvPos);
                    const int toInvType = swapBasicBoard::pieceToAbsType(toInvPiece);
                    if(toInvType == rook){
                        if(!genMove::getRelation(*this,toPos,beProtected,pos)){
                            vlSingleStringHold -= stringValueTab[256 + toPos - pos];
                        }
                    }
                    //受到炮的牵制
                    const int toInvInvPos = position::bitBoard.getRayTargetPos(pos,target,2);
                    const int toInvInvPiece = position::board.getPieceByPos(toInvInvPos);
                    const int toInvInvType = swapBasicBoard::pieceToAbsType(toInvInvPiece);
                    if(toInvInvType == cannon){
                        if(!genMove::getRelation(*this,toPos,beProtected,pos)){
                            vlSingleStringHold -= stringValueTab[256 + toPos - pos];
                        }
                    }
                }
            }
        }
        return vlSingleStringHold;
    }
private:
    static int vlIndex(int type){
        return type - 1;
    }
    void getAttackValue(int& alRed,int& alBlack){
        alRed = alBlack = 0;
        int vlRedSimple = 0;
        int vlBlackSimple = 0;
        //车马
        const int rookOrKnightPieceList[8] = {8,9,-8,-9,6,7,-6,-7};
        for(int piece : rookOrKnightPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(piece > 0){
                if(inSideBoard[pos] * piece < 0){
                    alRed += 2;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == rook){
                    vlRedSimple += 2;
                }else{
                    vlRedSimple ++;
                }
            }else{
                if(inSideBoard[pos] * piece < 0){
                    alBlack += 2;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == rook){
                    vlBlackSimple += 2;
                }else{
                    vlBlackSimple ++;
                }
            }
        }
        const int cannonOrPawnPieceList[14] = {
                10, 11, 12, 13, 14, 15, 16,
                -10,-11,-12,-13,-14,-15,-16
        };
        for(int piece : cannonOrPawnPieceList){
            const int pos = position::swapBoard.getPosByPiece(piece);
            if(piece > 0){
                if(inSideBoard[pos] * piece < 0){
                    alRed ++;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == cannon){
                    vlRedSimple ++;
                }
            }else{
                if(inSideBoard[pos] * piece < 0){
                    alBlack ++;
                }
                if(swapBasicBoard::pieceToAbsType(piece) == cannon){
                    vlBlackSimple ++;
                }
            }
        }
        // 如果本方轻子数比对方多，那么每多一个轻子(车算2个轻子)威胁值加2。威胁值最多不超过8
        if(vlRedSimple > vlBlackSimple){
            alRed += abs(vlRedSimple - vlBlackSimple) * 2;
        }else if(vlBlackSimple > vlRedSimple){
            alBlack += abs(vlRedSimple - vlBlackSimple) * 2;
        }
        alRed = min(alRed,TOTAL_ATTACK_VALUE);
        alBlack = min(alBlack,TOTAL_ATTACK_VALUE);
    }
    void getMidgameValue(int& vl){
        //车
        const int rookPieceList[4] = {8,9,-8,-9};
        for(int piece : rookPieceList){
            if(position::swapBoard.getPosByPiece(piece)){
                vl += ROOK_MIDGAME_VALUE;
            }
        }
        //马炮
        const int knightOrCannonPieceList[8] = {6,7,-6,-7,10,11,-10,-11};
        for(int piece : knightOrCannonPieceList){
            if(position::swapBoard.getPosByPiece(piece)){
                vl += KNIGHT_CANNON_MIDGAME_VALUE;
            }
        }
        //其他
        const int OtherPieceList[18] = {
                2, 3, 4, 5, 12, 13, 14, 15, 16,
                -2,-3,-4,-5,-12,-13,-14,-15,-16
        };
        for(int piece : OtherPieceList){
            if(position::swapBoard.getPosByPiece(piece)){
                vl += OTHER_MIDGAME_VALUE;
            }
        }
        vl = (2 * TOTAL_MIDGAME_VALUE - vl) * vl / TOTAL_MIDGAME_VALUE;
    }
protected:
    vector<int> drawMoveStatus;             //走法路线对应的和棋走法数
    vector<bool> checkMoveStatus;           //走法路线对应的将军状态
    vector<bool> chaseMoveStatus;           //走法瑞安对应的捉子状态
    vector<step> moveRoad;                  //走法路线
private:
    int vlRed{};
    int vlBlack{};
    int vlFirstGo{};                        //先行权分值
    int vlRedBoard[7][256]{};               //红方估值矩阵
    int vlBlackBoard[7][256]{};             //黑方估值矩阵
    int vlRedAdvisorLeakage{};              //红士的危险值
    int vlBlackAdvisorLeakage{};            //黑士的危险值
    int vlRedBottomThreat[16]{};            //黑底炮的威胁向量
    int vlBlackBottomThreat[16]{};          //红底炮的威胁向量
    int vlHollowThreat[16]{};               //空头炮的威胁向量
    int vlCentralThreat[16]{};              //窝心马的威胁向量
    friend class test;
    friend class searchGroup;
    friend class killerCache;
};