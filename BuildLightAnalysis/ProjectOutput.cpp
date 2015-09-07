#include "stdafx.h"
#include "ProjectOutput.h"
#include "MainFrm.h"
#include "MathUtility.h"

#include <fstream>
#include <list>


void geometryOutput(string filename)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	ofstream out(filename);
	if (!out.is_open())
	{
		AfxMessageBox(_T("文件创建失败"));
		return;
	}

	PropertyGridCtrl* pOption = pMain->GetOptionProperty().getPropList();
	PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	double h = pOption->GetProperty(0)->GetValue().dblVal;
	for (int i = 0; i < pRoomlist->GetPropertyCount(); i++)
	{
		//导出第i个房间
		CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(i);
		Wall wall;
		list<Wall> roomWalls;
		stWindow window;
		vector<stWindow> roomWindows;
		for (int j = 0; j < pRoom->GetSubItemsCount(); j++)
		{
			CString wallType = pRoom->GetSubItem(j)->GetName();
			if (wallType == _T("外墙号"))
			{
				int index = pRoom->GetSubItem(j)->GetValue().intVal;
				wall.line.s.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				wall.line.s.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				wall.line.e.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				wall.line.e.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				wall.wallInfo.index = index;
				wall.wallInfo.type = sLine::OUT_WALL;
				wall.isOrder = true;
				roomWalls.push_back(wall);
			}
			else if (wallType == _T("内墙号"))
			{
				int index = pRoom->GetSubItem(j)->GetValue().intVal;
				wall.line.s.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				wall.line.s.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				wall.line.e.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				wall.line.e.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				wall.wallInfo.index = index;
				wall.wallInfo.type = sLine::IN_WALL;
				wall.isOrder = true;
				roomWalls.push_back(wall);
			}
			else if (wallType == _T("窗户号"))
			{
				int index = pRoom->GetSubItem(j)->GetValue().intVal;
				stWindow win;
				CMFCPropertyGridProperty* pWin = pWindowlist->GetProperty(index);
				CString wallType = pWin->GetSubItem(0)->GetValue().bstrVal;
				_tcscpy(win.wallType, wallType);
				win.wallIndex = pWin->GetSubItem(1)->GetValue().intVal;
				win.pos = pWin->GetSubItem(2)->GetValue().dblVal;
				win.WinUpHeight = pWin->GetSubItem(3)->GetValue().dblVal;
				win.WinDownHeight = pWin->GetSubItem(4)->GetValue().dblVal;
				win.WinWidth = pWin->GetSubItem(5)->GetValue().dblVal;
				CString mat = pWin->GetSubItem(6)->GetValue().bstrVal;
				_tcscpy(win.WinMaterial, mat);

				roomWindows.push_back(win);
			}

		}
		vector<Vec2d> outPolygon;
		vector<Wall> outWalls;
		if (!CalClosedPolygon(roomWalls, outWalls, outPolygon))
		{
			CString msg;
			msg.Format(_T("房间%d没有封闭，不能导出"),i);
			AfxMessageBox(msg);
		}
		if (isAntiClock(outPolygon))//逆时针
		{
			//地面
			out << "room" << i << ".Floor";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(5)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = 0; j < outPolygon.size(); j++)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< 0.0 << endl;
			}
			out << endl;
			//棚顶
			out << "room" << i << ".Roof";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(6)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< h << endl;
			}
			out << endl;
			//侧墙
			for (int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;

				out << "room" << i << ".Flank" << j;
				out << " polygon " << CStringToString(mat) << endl; 
				out << "0 0 9 ";
				if (outWalls[j].isOrder)
				{
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
				}
				else
				{
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
				}
				out << endl;
			}
			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				out << "room" << i << ".Window" << j;
				out << " polygon " << CStringToString(CString(roomWindows[j].WinMaterial)) << endl; 
				out << "0 0 9 ";

				CMFCPropertyGridProperty* wallProperty = NULL;
				sLine::W_TYPE type;
				if (CString(roomWindows[j].wallType) == _T("外墙"))
				{
					wallProperty = optimizeOutWallPos;
					type = sLine::OUT_WALL;
				}
				else if (CString(roomWindows[j].wallType) = _T("内墙"))
				{
					wallProperty = optimizeInWallPos;
					type = sLine::IN_WALL;
				}
				bool isOrder = true;
				for (int k = 0; k < outWalls.size(); k++)
				{
					if (outWalls[k].wallInfo.type == type && outWalls[k].wallInfo.index == roomWindows[j].wallIndex)
					{
						isOrder = outWalls[k].isOrder;
						break;
					}
				}

				p.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				p.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				p1.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				p1.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				Vec2d dir = p1 - p;
				Vec2d pWinc = p + dir * roomWindows[j].pos;
				dir = dir / dir.Length();
				Vec2d pWins = pWinc - dir * roomWindows[j].WinWidth * 0.5;
				Vec2d pWine = pWinc + dir * roomWindows[j].WinWidth * 0.5;

				double hUp = roomWindows[j].WinUpHeight;


				if (isOrder)
				{
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				else
				{
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				out << endl;
			}
		}
		else
		{
			//地面
			out << "room" << i << ".Floor";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(5)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< 0.0 << endl;
			}
			out << endl;
			//棚顶
			out << "room" << i << ".Roof";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(6)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = 0; j < outPolygon.size(); j++)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< h << endl;
			}
			out << endl;
			//侧墙
			for (int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;

				out << "room" << i << ".Flank" << j;
				out << " polygon " << CStringToString(mat) << endl; 
				out << "0 0 9 ";
				if (!outWalls[j].isOrder)
				{
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
				}
				else
				{
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
				}
				out << endl;

			}

			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				out << "room" << i << ".Window" << j;
				out << " polygon " << CStringToString(CString(roomWindows[j].WinMaterial)) << endl; 
				out << "0 0 9 ";

				CMFCPropertyGridProperty* wallProperty = NULL;
				sLine::W_TYPE type;
				if (CString(roomWindows[j].wallType) == _T("外墙"))
				{
					wallProperty = optimizeOutWallPos;
					type = sLine::OUT_WALL;
				}
				else if (CString(roomWindows[j].wallType) = _T("内墙"))
				{
					wallProperty = optimizeInWallPos;
					type = sLine::IN_WALL;
				}
				bool isOrder = true;
				for (int k = 0; k < outWalls.size(); k++)
				{
					if (outWalls[k].wallInfo.type == type && outWalls[k].wallInfo.index == roomWindows[j].wallIndex)
					{
						isOrder = outWalls[k].isOrder;
						break;
					}
				}

				p.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				p.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				p1.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				p1.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				Vec2d dir = p1 - p;
				Vec2d pWinc = p + dir * roomWindows[j].pos;
				dir = dir / dir.Length();
				Vec2d pWins = pWinc - dir * roomWindows[j].WinWidth * 0.5;
				Vec2d pWine = pWinc + dir * roomWindows[j].WinWidth * 0.5;

				double hUp = roomWindows[j].WinUpHeight;


				if (!isOrder)
				{
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				else
				{
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				out << endl;
			}
		}
	}
}

void materialOutput(string filename, MaterialSet& materials)
{
	ofstream out(filename);
	if (!out.is_open())
	{
		return;
	}

	for (int i = 0; i < materials.m_materials.size(); i++)
	{
		out << "void " << materials.m_materials[i].type << " " << materials.m_materials[i].name;
		for (int j = 0; j < materials.m_materials[i].args.size(); j++)
		{
			out <<" " << materials.m_materials[i].args[j];
		}
		out << endl;
	}
}