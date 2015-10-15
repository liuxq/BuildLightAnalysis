#pragma once

#include <vector>
#include <list>
#include "CommonData.h"


#define PI 3.1415926

using namespace std;

string CStringToString(CString& str);
CString StringToCString(string& str);


//按照交点优化线段，去掉多余的，补充缺失的，最后再按照交点分段
void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double wTh);
//求点和线段的距离
double lenOfLinePoint(sLine& line, Vec2d p);

//求线段集合形成的封闭多边形，如果不封闭则返回false
bool CalClosedPolygon(list<Wall>& lines, vector<Wall>& outWalls, vector<Vec2d>& outPoints);

//判断多边形是逆时针还是顺时针
bool isAntiClock(vector<Vec2d>& polygon);

//求多边形内的计算点
void CalGridFromPolygon(vector<Vec2d>& polygon, double offset, double meshLen, vector<Vec2d>& outPoints);

//求多边形面积
double CalArea(vector<Vec2d>& polygon);

//求多边形周长
double CalGirth(vector<Vec2d>& polygon);

//旋转坐标
Vec2d Rotate(Vec2d& p, double angle);

int NamePost(CString name);