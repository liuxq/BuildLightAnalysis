#include <vector>
#include "vec2.h"
using namespace std;


struct sLine
{
	enum
	{
		IN_WALL = 0,
		OUT_WALL
	};
	Vec2d s;
	Vec2d e;
	int type;
	sLine(Vec2d s, Vec2d e, int itype = IN_WALL)
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

void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double wTh);
