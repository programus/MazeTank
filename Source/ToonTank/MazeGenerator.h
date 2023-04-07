// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define MAZE_H_PATH 0x01
#define MAZE_V_PATH  0x02

/**
 * 
 */
class TOONTANK_API CMazeGenerator
{
public:
	CMazeGenerator(uint32 Width, uint32 Height);
	~CMazeGenerator();

	const uint32** Generate(uint32 Row = 0, uint32 Col = 0);

	inline const uint32** GetMaze() const;

	FString MazeString() const;

protected:
	void dfs(uint32 r, uint32 c);

	void clean();

	inline uint32 Clamp(uint32 v, uint32 min, uint32 max)
	{
		if (v < min)
		{
			return min;
		}
		else if (v >= max)
		{
			return max - 1;
		}
		return v;
	}

	uint32** data;
	uint32 m;	// height
	uint32 n;	// width
};

