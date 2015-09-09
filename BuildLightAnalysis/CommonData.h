#pragma once

#include <vector>
#include <string>
#include "vec2.h"
using namespace std;


struct sLine
{
	enum W_TYPE
	{
		OUT_WALL = 1,
		IN_WALL = 2
		
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

#pragma pack(1)
struct sOpWall
{
	sLine line;
	WCHAR mat[80];
};
#pragma pack()

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
	int type;// 1 2 3 : ÍâÇ½£¬ ÄÚÇ½£¬ ´°»§
	int index;
};

struct Wall
{
	sLine line;
	WallIndex wallInfo;
	bool isOrder;
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

struct GridPoint
{
	Vec2d p;
	bool isKey;
};
struct Grid
{
	int roomIndex;
	double offset;
	double meshLen;
	vector<GridPoint> points;
};