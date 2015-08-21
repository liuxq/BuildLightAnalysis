#include <vector>
#include "vec2.h"
using namespace std;


struct sLine
{
	Vec2d s;
	Vec2d e;
	sLine(Vec2d s, Vec2d e)
	{
		this->s = s;
		this->e = e;
	}
	sLine()
	{
		this->s = Vec2d::ZERO;
		this->e = Vec2d::ZERO;
	}
};

void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines);
