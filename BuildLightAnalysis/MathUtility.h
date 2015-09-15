#pragma once

#include <vector>
#include <list>
#include "CommonData.h"

using namespace std;

string CStringToString(CString& str);
CString StringToCString(string& str);


//���ս����Ż��߶Σ�ȥ������ģ�����ȱʧ�ģ�����ٰ��ս���ֶ�
void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double wTh);
//�����߶εľ���
double lenOfLinePoint(sLine& line, Vec2d p);

//���߶μ����γɵķ�ն���Σ����������򷵻�false
bool CalClosedPolygon(list<Wall>& lines, vector<Wall>& outWalls, vector<Vec2d>& outPoints);

//�ж϶��������ʱ�뻹��˳ʱ��
bool isAntiClock(vector<Vec2d>& polygon);

//�������ڵļ����
void CalGridFromPolygon(vector<Vec2d>& polygon, double offset, double meshLen, vector<Vec2d>& outPoints);