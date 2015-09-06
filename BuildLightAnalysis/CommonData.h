#pragma once

#include <vector>
#include <string>
#include "vec2.h"
using namespace std;


struct sLine
{
	enum W_TYPE
	{
		IN_WALL = 0,
		OUT_WALL
	};
	Vec2d s;
	Vec2d e;
	W_TYPE type;
	sLine(Vec2d s, Vec2d e, W_TYPE itype = IN_WALL)
	{
		this->s = s;
		this->e = e;
		type = itype;
	}
	sLine()
	{
		this->s = Vec2d::ZERO;
		this->e = Vec2d::ZERO;
		type = IN_WALL;
	}
};

struct WallIndex
{
	WallIndex(int t, int i)
	{
		this->type = t;
		this->index = i;
	}
	WallIndex()
	{
		this->type = 1;
		this->index = -1;
	}
	int type;
	int index;
};

#pragma pack(1)
struct stWindow
{
	WCHAR wallType[80];
	int wallIndex;
	double pos;
	double WinUpHeight;
	double WinDownHeight;
	double WinWidth;
	WCHAR WinMaterial[80];
};
#pragma pack()