#pragma once

#include <vector>
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