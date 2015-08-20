#include "stdafx.h"
#include "OptimizeLine.h"

void calLenOfLines(sLine& srcLine, sLine& dstLine)
{
	Vec2d d1 = srcLine.e - srcLine.s;
	Vec2d d2 = dstLine.e - dstLine.s;
	Vec2d a1 = srcLine.s;
	Vec2d a2 = dstLine.s;

	double D = d2.y * d1.x - d2.x * d1.y;
	if (D == 0.0)//Æ½ÐÐ
	{
		return;
	}
	double t1 = ((a1.y - a2.y) * d2.x - (a1.x - a2.x) * d2.y)/D;
	double t2 = ((a1.y - a2.y) * d1.x - (a1.x - a2.x) * d1.y)/D;
	
	if (t1 > 1)
	{
		srcLine.e = srcLine.s + d1 * t1;
	}
	if (t1 < 0)
	{
		srcLine.s = srcLine.s + d1 * t1;
	}
	if (t2 > 1)
	{
		dstLine.e = dstLine.s + d2 * t2;
	}
	if (t2 < 0)
	{
		dstLine.s = dstLine.s + d2 * t2;
	}
	return;
}

void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines)
{
	for (int i = 0; i < slines.size(); i++)
	{
		for (int j = i+1; j < slines.size(); j++)
		{
			calLenOfLines(slines[i],slines[j]);
		}
	}
}

