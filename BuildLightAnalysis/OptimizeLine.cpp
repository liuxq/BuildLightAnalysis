#include "stdafx.h"
#include "OptimizeLine.h"

void calLenOfLines(sLine& srcLine, sLine& dstLine, double& startW, double& endW)
{
	Vec2d d1 = srcLine.e - srcLine.s;
	Vec2d d2 = dstLine.e - dstLine.s;
	Vec2d a1 = srcLine.s;
	Vec2d a2 = dstLine.s;

	double D = d2.y * d1.x - d2.x * d1.y;
	if (D == 0.0)//平行
	{
		startW = endW = 1.0e8;
		return;
	}
	double t1 = ((a1.y - a2.y) * d2.x - (a1.x - a2.x) * d2.y)/D;
	double t2 = ((a1.y - a2.y) * d1.x - (a1.x - a2.x) * d1.y)/D;
	
	if (t2 > 1 || t2 < 0)//代价要增加line2的修改
	{
		//line1代价
		double d1Length = d1.Length();
		if (t1 >= 0 && t1 <= 1)
		{
			startW = d1Length * t1;
			endW = d1Length * (1-t1);
		}
		else if (t1 < 0)
		{
			startW = -t1 * d1Length;
			endW = startW + d1Length;
		}
		else
		{
			endW = (t1 - 1 ) * d1Length;
			startW = endW + d1Length;
		}
		//line2代价
		double w2 = 1.0e8;
		if (t2 < 0)
		{
			w2 =  -t2 * d2.Length();
		}
		else
		{
			w2 =  (t2 - 1 ) * d2.Length();
		}
		startW += w2;
		endW += w2;
		return;
	}
	else//只需要计算line1的修改代价
	{
		double d1Length = d1.Length();
		if (t1 >= 0 && t1 <= 1)
		{
			startW = d1Length * t1;
			endW = d1Length * (1-t1);
			return;
		}
		else if (t1 < 0)
		{
			startW = -t1 * d1Length;
			endW = startW + d1Length;
			return;
		}
		else
		{
			endW = (t1 - 1 ) * d1Length;
			startW = endW + d1Length;
			return;
		}
	}
	return;
}
void OptimizeTwoLines(sLine& srcLine, sLine& dstLine, bool isStartP)
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

	Vec2d intersectP = srcLine.s + d1 * t1;

	if (isStartP)
	{
		srcLine.s = intersectP;
	}
	else
	{
		srcLine.e = intersectP;
	}
	return;
}

void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double thW)
{
	outSlines = slines;
	for (int i = 0; i < slines.size(); i++)
	{
		double minStartW = 1.0e8, minEndW = 1.0e8;
		int minStartI = 0, minEndI = 0;
		for (int j = 0; j < slines.size(); j++)
		{
			if (j != i)
			{
				double startW, endW;
				calLenOfLines(slines[i],slines[j], startW, endW);
				if (startW < minStartW)
				{
					minStartW = startW;
					minStartI = j;
				}
				if (endW < minEndW)
				{
					minEndW = endW;
					minEndI = j;
				}
			}
		}
		if (minStartW < thW)//有一个需要修改的
		{
			OptimizeTwoLines(outSlines[i], slines[minStartI], true);
		}
		if (minEndW < thW)//有一个需要修改的
		{
			OptimizeTwoLines(outSlines[i], slines[minEndI], false);
		}
	}
}

