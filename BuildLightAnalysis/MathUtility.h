#pragma once

#include <vector>
#include "CommonData.h"

using namespace std;



//���ս����Ż��߶Σ�ȥ������ģ�����ȱʧ�ģ�����ٰ��ս���ֶ�
void OptimizeLine(vector<sLine>& slines, vector<sLine>& outSlines, double wTh);
//�����߶εľ���
double lenOfLinePoint(sLine& line, Vec2d p);