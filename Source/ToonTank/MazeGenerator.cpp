// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

#define RESULT_MASK 0x0f
#define MARK_MASK   0xf0
#define MARK_VALUE  0x10

CMazeGenerator::CMazeGenerator(uint32 Width, uint32 Height)
{
    n = Width;
    m = Height;

    data = new uint32 * [m];
    for (uint32 i = 0; i < m; i++)
    {
        data[i] = new uint32[n];
        memset(data[i], 0, sizeof(uint32) * n);
    }
}

CMazeGenerator::~CMazeGenerator()
{
    for (uint32 i = 0; i < m; i++)
    {
        delete data[i];
    }
    delete data;
    data = nullptr;
}

const uint32** CMazeGenerator::Generate(uint32 Row, uint32 Col)
{
    Row = Clamp(Row, 0, m);
    Col = Clamp(Col, 0, n);

    dfs(Row, Col);
    clean();

    return GetMaze();
}

const uint32** CMazeGenerator::GetMaze() const
{
    return (const uint32**) data;
}

void CMazeGenerator::dfs(uint32 r, uint32 c)
{
    int32 dir_r[] = { -1, 0, 1, 0 };
    int32 dir_c[] = { 0, -1, 0, 1 };
    uint32 d = rand() % 4, dd = rand() % 2 ? 1 : 3;
    data[r][c] |= MARK_VALUE;
    for (uint32 i = 0; i < 4; i++) {
        int32 rr = r + dir_r[d];
        int32 cc = c + dir_c[d];
        if ((unsigned)rr < m && (unsigned)cc < n && !(data[rr][cc] & MARK_MASK)) {
            if (d % 2)
            {
                data[r][c - (d == 1)] |= MAZE_H_PATH;
            }
            else
            {
                data[r - (d == 0)][c] |= MAZE_V_PATH;
            }

            dfs(rr, cc);
        }
        d = (d + dd) % 4;
    }
}

void CMazeGenerator::clean()
{
    for (uint32 r = 0; r < m; r++)
    {
        for (uint32 c = 0; c < n; c++)
        {
            data[r][c] &= RESULT_MASK;
        }
    }
}

FString CMazeGenerator::MazeString() const
{
    int32 Len = (m + 1) * (n + 1) * 2;
    char* str = new char[Len];
    uint32 index = 0;
    for (uint32 i = 0; i < n; i++)
    {
        str[index++] = '.';
        str[index++] = '_';
    }
    str[index++] = '.';
    str[index++] = '\n';
    for (uint32 r = 0; r < m; r++)
    {
        str[index++] = '|';
        for (uint32 c = 0; c < n; c++)
        {
            str[index++] = ((data[r][c] & MAZE_V_PATH) ? ' ' : '_');
            str[index++] = ((data[r][c] & MAZE_H_PATH) ? '.' : '|');
        }
        str[index++] = '\n';
    }

    FString ret(Len, str);
    delete str;
    return ret;
}

