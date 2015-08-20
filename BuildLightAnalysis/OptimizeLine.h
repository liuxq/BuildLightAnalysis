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
};

void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines);
