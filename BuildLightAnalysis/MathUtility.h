#pragma once

#include <vector>
#include "CommonData.h"

using namespace std;

string CStringToString(CString& str);
CString stringToCString(string& str);


//按照交点优化线段，去掉多余的，补充缺失的，最后再按照交点分段
void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double wTh);
//求点和线段的距离
double lenOfLinePoint(sLine& line, Vec2d p);