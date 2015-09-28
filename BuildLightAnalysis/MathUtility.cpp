#include "stdafx.h"
#include "MathUtility.h"
#include <algorithm>

string CStringToString(CString& str)
{
	CStringA stra(str.GetBuffer(0));
	str.ReleaseBuffer();
	string strString = stra.GetBuffer(0);
	stra.ReleaseBuffer();
	return strString;
};

CString StringToCString(string& str)
{
	CStringA ss;
	ss.Format("%s",str.c_str());
	CString ws(ss.GetBuffer(0));
	ss.ReleaseBuffer();
	return ws;
};


double calIntersectPointsOfLines(sLine& srcLine, sLine& dstLine)
{
	Vec2d d1 = srcLine.e - srcLine.s;
	Vec2d d2 = dstLine.e - dstLine.s;
	Vec2d a1 = srcLine.s;
	Vec2d a2 = dstLine.s;

	double D = d2.y * d1.x - d2.x * d1.y;
	if (D == 0.0)//平行
	{
		return -1.0;
	}
	double t1 = ((a1.y - a2.y) * d2.x - (a1.x - a2.x) * d2.y)/D;
	double t2 = ((a1.y - a2.y) * d1.x - (a1.x - a2.x) * d1.y)/D;

	if (t1 > 0 && t1 < 1 && fabs(t1) > 1e-6 && fabs(t1-1) > 1e-6 && (t2 > 0 && t2 < 1 || fabs(t2) < 1e-6 || fabs(t2-1) < 1e-6))
	{
		return t1;
	}
	else
	{
		return -1.0;
	}
}

//分割线段，按照交点分割
void SegmentLine(vector<sLine>& slines, vector<sLine>& outSlines)
{	
	for (int i = 0; i < slines.size(); i++)
	{
		vector<double> ts;
		for (int j = 0; j < slines.size(); j++)
		{
			if (j != i)
			{
				double t;
				t = calIntersectPointsOfLines(slines[i],slines[j]);
				if (t > 0)
				{
					ts.push_back(t);
				}
			}
		}
		std::sort(ts.begin(), ts.end());
		sLine::W_TYPE type_ = slines[i].type;
		if (ts.empty())
		{
			outSlines.push_back(slines[i]);
		}
		else
		{
			sLine line = slines[i];
			Vec2d dir = line.e - line.s;
			Vec2d p1 = line.s;
			Vec2d p2;
			for (int k = 0; k < ts.size(); k++)
			{
				p2 = line.s + ts[k] * dir;
				outSlines.push_back(sLine(p1, p2 ,type_));
				p1 = p2;
			}
			outSlines.push_back(sLine(p1, line.e, type_));
		}
	}
}


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

Vec2d IntersectTwoLines(sLine& srcLine, sLine& dstLine)
{
	Vec2d d1 = srcLine.e - srcLine.s;
	Vec2d d2 = dstLine.e - dstLine.s;
	Vec2d a1 = srcLine.s;
	Vec2d a2 = dstLine.s;

	double D = d2.y * d1.x - d2.x * d1.y;
	if (D == 0.0)//平行
	{
		return Vec2d::ZERO;
	}
	double t1 = ((a1.y - a2.y) * d2.x - (a1.x - a2.x) * d2.y)/D;
	double t2 = ((a1.y - a2.y) * d1.x - (a1.x - a2.x) * d1.y)/D;

	return srcLine.s + d1 * t1;
}


struct MapInfo
{
	MapInfo()
	{
		isClose = false;
		isStart = false;
	}
	Vec2d iPoint;
	bool isClose;
	bool isStart;
};
//优化线段，去掉和补充
void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double thW)
{
	int sz = slines.size();
	//建立二维数组
	vector<vector<MapInfo> > mapInfos(sz);
	vector<bool> isChangedStart(sz,false);
	vector<bool> isChangedEnd(sz,false);
	for (int i = 0; i < mapInfos.size(); i++)
		mapInfos[i].resize(sz);

	vector<sLine> optimizeSlines = slines;
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
			mapInfos[i][minStartI].isClose = true;
			mapInfos[i][minStartI].isStart = true;
			mapInfos[i][minStartI].iPoint = IntersectTwoLines(slines[i],slines[minStartI]);
			//OptimizeTwoLines(optimizeSlines[i], slines[minStartI], true);
		}
		if (minEndW < thW)//有一个需要修改的
		{
			mapInfos[i][minEndI].isClose = true;
			mapInfos[i][minEndI].isStart = false;
			mapInfos[i][minEndI].iPoint = IntersectTwoLines(slines[i],slines[minEndI]);
			//OptimizeTwoLines(optimizeSlines[i], slines[minEndI], false);
		}
	}
	//先求出首尾相交的线段
	for (int i = 0; i < sz; i++)
	{
		for (int j = i+1; j < sz; j++)
		{
			if (mapInfos[i][j].isClose)
			{
				if (mapInfos[j][i].isClose)
				{
					if (mapInfos[i][j].isStart)
					{
						optimizeSlines[i].s = mapInfos[i][j].iPoint;
						isChangedStart[i] = true;
					}
					else
					{
						optimizeSlines[i].e = mapInfos[i][j].iPoint;
						isChangedEnd[i] = true;
					}

					if (mapInfos[j][i].isStart)
					{
						optimizeSlines[j].s = mapInfos[j][i].iPoint;
						isChangedStart[j] = true;
					}
					else
					{
						optimizeSlines[j].e = mapInfos[j][i].iPoint;
						isChangedEnd[j] = true;
					}
				}
			}
		}
	}
	for (int i = 0; i < sz; i++)
	{
		for (int j = 0; j < sz; j++)
		{
			if (mapInfos[i][j].isClose && !mapInfos[j][i].isClose)
			{
				if (isChangedStart[j] && (mapInfos[i][j].iPoint - optimizeSlines[j].s).Length() < thW)
				{
					if (mapInfos[i][j].isStart)
					{
						optimizeSlines[i].s = optimizeSlines[j].s;
						isChangedStart[i] = true;
					}
					else
					{
						optimizeSlines[i].e = optimizeSlines[j].s;
						isChangedEnd[i] = true;
					}
				}
				else if (isChangedEnd[j] && (mapInfos[i][j].iPoint - optimizeSlines[j].e).Length() < thW)
				{
					if (mapInfos[i][j].isStart)
					{
						optimizeSlines[i].s = optimizeSlines[j].e;
						isChangedStart[i] = true;
					}
					else
					{
						optimizeSlines[i].e = optimizeSlines[j].e;
						isChangedEnd[i] = true;
					}
				}
				else 
				{
					if (mapInfos[i][j].isStart)
					{
						optimizeSlines[i].s = mapInfos[i][j].iPoint;
					}
					else
					{
						optimizeSlines[i].e = mapInfos[i][j].iPoint;
					}
				}
			}
		}
	}
	
	SegmentLine(optimizeSlines, outSlines);
}

double lenOfLinePoint(sLine& line, Vec2d p)
{
	Vec2d d1 = line.e - line.s;
	Vec2d d2 = p - line.s;

	double l1 = d1.Length();
	double t = d1*d2 / l1;

	if (t >= 0 && t <= l1)
	{
		return fabs(d1/d2) / l1;
	}
	else if (t < 0)
	{
		return d2.Length();
	}
	else
	{
		return (p - line.e).Length();
	}

}
//相等的阈值，20mm
bool isEqual(Vec2d a, Vec2d b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;
	return x*x + y*y < 400; //20 * 20
}
//求线段集合形成的封闭多边形，如果不封闭则返回false
bool CalClosedPolygon(list<Wall>& walls, vector<Wall>& outWalls, vector<Vec2d>& outPoints)
{
	if (walls.size() < 3)
	{
		return false;
	}

	list<Wall>::iterator i;
	i = walls.begin();
	Wall firstWall = *i;
	Vec2d p = firstWall.line.s;
	firstWall.isOrder = false;
	walls.erase(i);
	outPoints.push_back(p);
	outWalls.push_back(firstWall);
	do
	{
		bool flag = false;
		for (list<Wall>::iterator j = walls.begin(); j != walls.end(); j++)
		{
			if (isEqual(p,(*j).line.s))
			{
				Wall w = (*j);
				p = w.line.e;
				walls.erase(j);
				flag = true;
				outPoints.push_back(p);
				w.isOrder = true;
				outWalls.push_back(w);
				break;
			}
			if (isEqual(p,(*j).line.e))
			{
				Wall w = (*j);
				p = w.line.s;
				walls.erase(j);
				flag = true;
				outPoints.push_back(p);
				w.isOrder = false;
				outWalls.push_back(w);
				break;
			}
		}

		if (!flag)
		{
			return false;
		}
	}
	while(!isEqual(p,firstWall.line.e));

	return true;
}

//判断多边形是逆时针还是顺时针, true：逆时针
bool isAntiClock(vector<Vec2d>& polygon)
{
	int sz = polygon.size();
	int greaterC = 0, lessC = 0;
	for (int i = 0; i < polygon.size(); i++)
	{
		Vec2d a = polygon[i];
		Vec2d b = polygon[(i+1)%sz];
		Vec2d c = polygon[(i+2)%sz];
		if ((b-a)/(c-b) > 0)
			greaterC ++;
		else
			lessC ++;
	}
	if (greaterC > lessC)
	{
		return true;
	}
	else
		return false;
}

//判断点是否在线的内侧
bool IsPointInsideLine(Vec2d p,sLine& line)
{
	Vec2d d1 = line.e - line.s;
	Vec2d d2 = p - line.s;

	if (d1/d2 < 0)
		return false;
	if (d1*d2 < 0)
		return false;
	if (d1*(line.e-p) < 0)
		return false;

	return true;
}

//x线段向内侧平移offset
void OffsetLine(sLine& line, double offset)
{
	Vec2d d1 = line.e - line.s;
	Vec2d dv(-d1.y,d1.x);
	dv.Normalize();

	line.s += dv * offset;
	line.e += dv * offset;

	return;
}

bool PtInPolygon (Vec2d p, vector<Vec2d>& ptPolygon) 
{ 
	int nCross = 0;
	int nCount = ptPolygon.size();
	for (int i = 0; i < ptPolygon.size(); i++) 
	{ 
		Vec2d p1 = ptPolygon[i]; 
		Vec2d p2 = ptPolygon[(i + 1) % nCount];
			// 求解 y=p.y 与 p1p2 的交点
		if ( p1.y == p2.y ) // p1p2 与 y=p0.y平行 
			continue;
		if ( p.y < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
			continue; 
		if ( p.y >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
			continue;
		// 求交点的 X 坐标 -------------------------------------------------------------- 
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
		if ( x > p.x ) 
			nCross++; // 只统计单边交点 
	}

	// 单边交点为偶数，点在多边形之外 --- 
	return (nCross % 2 == 1); 
}


void CalGridFromPolygon(vector<Vec2d>& polygon, double offset,double meshLen, vector<Vec2d>& outPoints)
{
	vector<sLine> lines;
	sLine line;
	int sz = polygon.size();
	
	Vec2d boxMin(1.0e9,1.0e9), boxMax(-1.0e9,-1.0e9);
	vector<sLine> polygonLines;
	for (int i = 0; i < sz; i++)
	{
		polygon[i].UpdateMinMax(boxMin, boxMax);
		line.s = polygon[i];
		line.e = polygon[(i+1)%sz];
		polygonLines.push_back(line);
	}

	for (double x = boxMin.x; x < boxMax.x; x += meshLen)
	{
		for (double y = boxMin.y; y < boxMax.y; y += meshLen)
		{
			Vec2d p(x,y);
			if (PtInPolygon(p,polygon))
			{
				bool flag = false;
				for (int i = 0; i < polygonLines.size(); i++)
				{
					if (lenOfLinePoint(polygonLines[i], p) < offset)
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					outPoints.push_back(p);
			}	
		}
	}
}


//求多边形面积
double CalArea(vector<Vec2d>& polygon)
{
	double l = 0;
	int sz = polygon.size();
	Vec2d p1, p2;

	for (int i = 0; i < sz; i++)
	{
		p1 = polygon[i];
		p2 = polygon[(i+1)%sz];
		l += p1.x*p2.y - p2.x*p1.y;
	}
	return fabs(l*0.5);
}

//求多边形周长
double CalGirth(vector<Vec2d>& polygon)
{
	double l = 0;
	int sz = polygon.size();
	for (int i = 0; i < sz; i++)
	{
		l += (polygon[i] - polygon[(i+1)%sz]).Length();
	}
	return l;
}