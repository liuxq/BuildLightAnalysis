#include "stdafx.h"
#include "ProjectOutput.h"
#include "MainFrm.h"
#include "MathUtility.h"

#include <fstream>
#include <list>



void geometryOutput(string filename, set<CString>& outMats)
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

	PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	double h = pMain->GetOptionProperty().GetDataDouble(OPTION_LEVEL_HEIGHT);
	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);

	outMats.insert(roofMat);
	outMats.insert(floorMat);

	for (int i = 0; i < pRoomlist->GetPropertyCount(); i++)
	{
		//导出第i个房间
		CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(i);
		Wall wall;
		list<Wall> roomWalls;
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
				_tcscpy_s(win.wallType, wallType);
				win.wallIndex = pWin->GetSubItem(1)->GetValue().intVal;
				win.pos = pWin->GetSubItem(2)->GetValue().dblVal;
				win.WinUpHeight = pWin->GetSubItem(3)->GetValue().dblVal;
				win.WinDownHeight = pWin->GetSubItem(4)->GetValue().dblVal;
				win.WinWidth = pWin->GetSubItem(5)->GetValue().dblVal;
				CString mat = pWin->GetSubItem(6)->GetValue().bstrVal;
				_tcscpy_s(win.WinMaterial, mat);

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
			out << " polygon " << CStringToString(floorMat) << endl; 
			out << "0 0 9 ";
			for (int j = 0; j < outPolygon.size(); j++)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< 0.0 << endl;
			}
			out << endl;
			//棚顶
			out << "room" << i << ".Roof";
			out << " polygon " << CStringToString(roofMat) << endl; 
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

				outMats.insert(mat);

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

				outMats.insert(CString(roomWindows[j].WinMaterial));

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
			out << " polygon " << CStringToString(floorMat) << endl; 
			out << "0 0 9 ";
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< 0.0 << endl;
			}
			out << endl;
			//棚顶
			out << "room" << i << ".Roof";
			out << " polygon " << CStringToString(roofMat) << endl; 
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

				outMats.insert(mat);

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

				outMats.insert(CString(roomWindows[j].WinMaterial));

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

void materialOutput(string filename, MaterialSet& materials, set<CString>& mats )
{
	ofstream out(filename);
	if (!out.is_open())
	{
		return;
	}

	for (int i = 0; i < materials.m_materials.size(); i++)
	{
		if (mats.find(StringToCString(materials.m_materials[i].name)) == mats.end())
			continue;
		out << "void " << materials.m_materials[i].type << " " << materials.m_materials[i].name;
		for (int j = 0; j < materials.m_materials[i].args.size(); j++)
		{
			out <<" " << materials.m_materials[i].args[j];
		}
		out << endl;
	}
}

void RoomOutput(string filename)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	vector<Grid> grids;
	pMain->GetGridProperty().OutputToGrids(grids);

	ofstream out(filename);
	if (!out.is_open())
	{
		AfxMessageBox(_T("文件创建失败"));
		return;
	}

	PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	double h = pMain->GetOptionProperty().GetDataDouble(OPTION_LEVEL_HEIGHT);
	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);
	for (int i = 0; i < pRoomlist->GetPropertyCount(); i++)
	{
		//导出第i个房间
		vector<Surface> surfaces;
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
				_tcscpy_s(win.wallType, wallType);
				win.wallIndex = pWin->GetSubItem(1)->GetValue().intVal;
				win.pos = pWin->GetSubItem(2)->GetValue().dblVal;
				win.WinUpHeight = pWin->GetSubItem(3)->GetValue().dblVal;
				win.WinDownHeight = pWin->GetSubItem(4)->GetValue().dblVal;
				win.WinWidth = pWin->GetSubItem(5)->GetValue().dblVal;
				CString mat = pWin->GetSubItem(6)->GetValue().bstrVal;
				_tcscpy_s(win.WinMaterial, mat);

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
		Surface surf;
		if (isAntiClock(outPolygon))//逆时针
		{
			//地面
			surf.name = "room" + i; 
			surf.name += ".Floor";
			surf.type = "polygon";
			surf.mat = CStringToString(floorMat);
			surf.points.clear();
			for (int j = 0; j < outPolygon.size(); j++)
			{
				Vec3d p(outPolygon[j].x, outPolygon[j].y, 0.0);
				surf.points.push_back(p);
			}
			surfaces.push_back(surf);

			//棚顶
			surf.name = "room" + i; 
			surf.name += ".Roof";
			surf.type = "polygon";
			surf.mat = CStringToString(roofMat);
			surf.points.clear();
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				Vec3d p(outPolygon[j].x, outPolygon[j].y, h);
				surf.points.push_back(p);
			}
			surfaces.push_back(surf);

			//侧墙
			for (int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;

				surf.name = "room" + i; 
				surf.name += ".Flank";
				surf.type = "polygon";
				surf.mat = CStringToString(mat);
				surf.points.clear();
				if (outWalls[j].isOrder)
				{
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, 0.0));
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, 0.0));
				}
				else
				{
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, 0.0));
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, 0.0));
				}
				surfaces.push_back(surf);
			}
			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				surf.name = "room" + i; 
				surf.name += ".Window";
				surf.type = "polygon";
				surf.mat = CStringToString(CString(roomWindows[j].WinMaterial));

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
				surf.points.clear();
				if (isOrder)
				{
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinDownHeight));
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinDownHeight));
				}
				else
				{
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinDownHeight));
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinDownHeight));
				}
				surfaces.push_back(surf);
			}
		}
		else
		{
			//地面
			surf.name = "room" + i; 
			surf.name += ".Floor";
			surf.type = "polygon";
			surf.mat = CStringToString(floorMat);
			surf.points.clear();
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				surf.points.push_back(Vec3d(outPolygon[i].x,outPolygon[i].y, 0.0));
			}
			surfaces.push_back(surf);
			//棚顶
			surf.name = "room" + i; 
			surf.name += ".Roof";
			surf.type = "polygon";
			surf.mat = CStringToString(roofMat);
			surf.points.clear();
			for (int j = 0; j < outPolygon.size(); j++)
			{
				surf.points.push_back(Vec3d(outPolygon[i].x,outPolygon[i].y, h));
			}
			surfaces.push_back(surf);
			//侧墙
			for (int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;

				surf.name = "room" + i; 
				surf.name += ".Flank";
				surf.type = "polygon";
				surf.mat = CStringToString(mat);
				surf.points.clear();
				if (!outWalls[j].isOrder)
				{
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, 0.0));
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, 0.0));
				}
				else
				{
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, h));
					surf.points.push_back(Vec3d(outWalls[j].line.s.x,outWalls[j].line.s.y, 0.0));
					surf.points.push_back(Vec3d(outWalls[j].line.e.x,outWalls[j].line.e.y, 0.0));
				}
				surfaces.push_back(surf);
			}

			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				surf.name = "room" + i; 
				surf.name += ".Window";
				surf.type = "polygon";
				surf.mat = CStringToString(CString(roomWindows[j].WinMaterial));
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
				surf.points.clear();
				if (!isOrder)
				{
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinDownHeight));
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinDownHeight));
				}
				else
				{
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinUpHeight));
					surf.points.push_back(Vec3d(pWins.x,pWins.y, roomWindows[j].WinDownHeight));
					surf.points.push_back(Vec3d(pWine.x,pWine.y, roomWindows[j].WinDownHeight));
				}
				surfaces.push_back(surf);
			}
		}

		//写入当前的房间信息
		Vec3d boxMin(1.0e9,1.0e9,1.0e9), boxMax(-1.0e9,-1.0e9,-1.0e9);
		for (int a = 0; a < surfaces.size(); a++)
		{
			for(int b = 0; b < surfaces[a].points.size(); b++)
			{
				surfaces[a].points[b].UpdateMinMax(boxMin,boxMax);
			}
		}
		double xLen = boxMax.x - boxMin.x;
		double yLen = boxMax.y - boxMin.y;
		double zLen = boxMax.z - boxMin.z;

		out << "void Room_"  << i << endl;
		out << "room_l " << xLen << endl;
		out << "room_w " << yLen << endl;
		out << "room_h " << zLen << endl << endl;
		out << "start geometry" << endl;

		for (int a = 0; a < surfaces.size(); a++)
		{
			out << "Surface" << a <<": " << surfaces[a].type << " " << surfaces[a].mat <<endl;
			for(int b = 0; b < surfaces[a].points.size(); b++)
			{
				out << surfaces[a].points[b].x << " "<<surfaces[a].points[b].y << " "<<surfaces[a].points[b].z << endl;
			}
		}

		out << "end geometry" << endl << endl;

		for (int k = 0; k < grids.size(); k++)
		{
			if (grids[k].roomIndex == i)
			{
				out << "start pts" << endl;
				for (int a = 0; a < grids[k].points.size(); a++)
				{
					out << grids[k].points[a].isKey << " " << grids[k].points[a].p.x << " " << grids[k].points[a].p.y << endl;
				}
				out << "end pts" << endl << endl;
				break;
			}
		}
	}
}