#include "stdafx.h"
#include "OptimizeLine.h"

double calLenOfLines(sLine& srcLine, sLine& dstLine)
{
	Vec2d d1 = srcLine.e - srcLine.s;
	Vec2d d2 = dstLine.e - dstLine.s;
	Vec2d a1 = srcLine.s;
	Vec2d a2 = dstLine.s;

	double D = d2.y * d1.x - d2.x * d1.y;
	if (D == 0.0)//平行
	{
		return 1.0e8;
	}
	double t1 = ((a1.y - a2.y) * d2.x - (a1.x - a2.x) * d2.y)/D;
	double t2 = ((a1.y - a2.y) * d1.x - (a1.x - a2.x) * d1.y)/D;
	
	if (t2 > 1 || t2 < 0)//代价要增加line2的修改
	{
		//line1代价
		double w1 = 10000;
		if (t1 >= 0 && t1 <= 1)
		{
			w1 =  t1 > 0.5 ? d1.Length() * (1-t1) : d1.Length() * t1;
		}
		else if (t1 < 0)
		{
			w1 =  -t1 * d1.Length();
		}
		else
		{
			w1 =  (t1 - 1 ) * d1.Length();
		}
		//line2代价
		double w2 = 10000;
		if (t2 < 0)
		{
			w2 =  -t2 * d2.Length();
		}
		else
		{
			w2 =  (t2 - 1 ) * d2.Length();
		}
		return w1 + w2;
	}
	else//只需要计算line1的修改代价
	{
		if (t1 >= 0 && t1 <= 1)
		{
			return t1 > 0.5 ? d1.Length() * (1-t1) : d1.Length() * t1;
		}
		else if (t1 < 0)
		{
			return -t1 * d1.Length();
		}
		else
		{
			return (t1 - 1 ) * d1.Length();
		}
	}
	return 10000;
}
void OptimizeTwoLines(sLine& srcLine, sLine& dstLine)
{
	Vec2d d1 = srcLine.e - srcLine.s;
	Vec2d d2 = dstLine.e - dstLine.s;
	Vec2d a1 = srcLine.s;
	Vec2d a2 = dstLine.s;

	double D = d2.y * d1.x - d2.x * d1.y;
	if (D == 0.0)//平行
	{
		return;
	}
	double t1 = ((a1.y - a2.y) * d2.x - (a1.x - a2.x) * d2.y)/D;
	double t2 = ((a1.y - a2.y) * d1.x - (a1.x - a2.x) * d1.y)/D;

	if (t2 > 1 || t2 < 0)//代价要增加line2的修改
	{
		//line1代价
		if (t1 >= 0 && t1 <= 1)
		{
			if (t1 > 0.5)
			{
				srcLine.e = srcLine.s + d1 * t1;
			}
			else
			{
				srcLine.s = srcLine.s + d1 * t1;
			}
		}
		else if (t1 < 0)
		{
			srcLine.s = srcLine.s + d1 * t1;
		}
		else
		{
			srcLine.e = srcLine.s + d1 * t1;
		}
	}
	else//只需要计算line1的修改代价
	{
		if (t1 >= 0 && t1 <= 1)
		{
			if (t1 > 0.5)
			{
				srcLine.e = srcLine.s + d1 * t1;
			}
			else
			{
				srcLine.s = srcLine.s + d1 * t1;
			}
		}
		else if (t1 < 0)
		{
			srcLine.s = srcLine.s + d1 * t1;
		}
		else
		{
			srcLine.e = srcLine.s + d1 * t1;
		}
	}
	return;
}
const double thW = 40.0;
void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines)
{
	outSlines = slines;
	for (int i = 0; i < slines.size(); i++)
	{
		double minW = 1.0e8;
		sLine doLine;
		for (int j = 0; j < slines.size(); j++)
		{
			if (j != i)
			{
				double w = calLenOfLines(slines[i],slines[j]);
				if (w < minW)
				{
					minW = w;
					doLine = slines[j];
				}
			}
		}
		if (minW < thW)//有一个需要修改的
		{
			OptimizeTwoLines(outSlines[i], doLine);
		}
	}
}

