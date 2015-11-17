#include "stdafx.h"
#include "ProjectOutput.h"
#include "MainFrm.h"
#include "MathUtility.h"
#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"

#include <fstream>
#include <list>
#include <string>
#include <algorithm>



void geometryOutput(string filename, set<CString>& outMats, set<Material>& antiMaterials)
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
	vector<Room> rooms;
	pMain->GetRoomProperty().OutputToRooms(rooms);

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);

	vector<OutRoom> outRooms;
	RoomOutToVector(outRooms, outMats, antiMaterials);
	double angle = pMain->GetOptionProperty().GetDataDouble(OPTION_NORTH);
	double anglep = angle / 180 * PI;

	for (int i = 0; i < rooms.size(); i++)
	{
		//导出第i个房间

		for (int j = 0; j < outRooms[i].surfaces.size(); j++)
		{
			OutSurface& surf = outRooms[i].surfaces[j];
			out << surf.mat << " polygon ";//材料 类型
			out << surf.name << endl;//名字
			out << surf.args[0] << " " << surf.args[1] << " " << surf.args[2] << " ";

			for (int k = 0; k < surf.points.size(); k++)
			{
				Vec2d p(surf.points[k].x, surf.points[k].y);
				p = Rotate(p, anglep);

				out << "\t";
				out << p.x << " " << p.y << " " << surf.points[k].z << endl;
			}
			out << endl;
		}
	}
}

void materialOutput(string filename, MaterialSet& materials, set<CString>& mats, set<Material>& antiMaterials )
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
	//输出反物质材料

	for (set<Material>::iterator i = antiMaterials.begin(); i != antiMaterials.end(); i++)
	{
		out << "void " << (*i).type << " " << (*i).name;
		for (int j = 0; j < (*i).args.size(); j++)
		{
			out <<" " << (*i).args[j];
		}
		out << endl;
	}
}

void wallToPoints(bool isAntiClock, double h, Wall& wall, vector<stWindow>& windows, vector<Vec3d>& outPoints)
{
	outPoints.clear();
	sort(windows.begin(),windows.end());
	bool isOrder = wall.isOrder;
	isOrder = isAntiClock? isOrder: !isOrder;
	if (isOrder)
	{
		outPoints.push_back(Vec3d(wall.line.s.x,wall.line.s.y, h));
		outPoints.push_back(Vec3d(wall.line.e.x,wall.line.e.y, h));
		outPoints.push_back(Vec3d(wall.line.e.x,wall.line.e.y, 0));
		for (int i = windows.size()-1; i >= 0 ; i--)
		{
			Vec2d dir = wall.line.e - wall.line.s;
			Vec2d pWinc = wall.line.s + dir * windows[i].pos;
			dir = dir / dir.Length();
			Vec2d pWins = pWinc - dir * windows[i].WinWidth * 0.5;
			Vec2d pWine = pWinc + dir * windows[i].WinWidth * 0.5;
			outPoints.push_back(Vec3d(pWine.x,pWine.y, 0));
			outPoints.push_back(Vec3d(pWine.x,pWine.y, windows[i].WinUpHeight));
			outPoints.push_back(Vec3d(pWins.x,pWins.y, windows[i].WinUpHeight));
			outPoints.push_back(Vec3d(pWins.x,pWins.y, windows[i].WinDownHeight));
			outPoints.push_back(Vec3d(pWine.x,pWine.y, windows[i].WinDownHeight));
			outPoints.push_back(Vec3d(pWine.x,pWine.y, 0));
		}
		outPoints.push_back(Vec3d(wall.line.s.x,wall.line.s.y, 0));
	}
	else
	{
		outPoints.push_back(Vec3d(wall.line.e.x,wall.line.e.y, h));
		outPoints.push_back(Vec3d(wall.line.s.x,wall.line.s.y, h));
		outPoints.push_back(Vec3d(wall.line.s.x,wall.line.s.y, 0));
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			Vec2d dir = wall.line.e - wall.line.s;
			Vec2d pWinc = wall.line.s + dir * windows[i].pos;
			dir = dir / dir.Length();
			Vec2d pWins = pWinc - dir * windows[i].WinWidth * 0.5;
			Vec2d pWine = pWinc + dir * windows[i].WinWidth * 0.5;
			outPoints.push_back(Vec3d(pWins.x,pWins.y, 0));
			outPoints.push_back(Vec3d(pWins.x,pWins.y, windows[i].WinUpHeight));
			outPoints.push_back(Vec3d(pWine.x,pWine.y, windows[i].WinUpHeight));
			outPoints.push_back(Vec3d(pWine.x,pWine.y, windows[i].WinDownHeight));
			outPoints.push_back(Vec3d(pWins.x,pWins.y, windows[i].WinDownHeight));
			outPoints.push_back(Vec3d(pWins.x,pWins.y, 0));
		}
		outPoints.push_back(Vec3d(wall.line.e.x,wall.line.e.y, 0));
	}
}
void RoomOutToVector(vector<OutRoom>& outRooms, set<CString>& mats, set<Material>& antiMaterials)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	vector<Room> rooms;
	pMain->GetRoomProperty().OutputToRooms(rooms);
	outRooms.resize(rooms.size());

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	
	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);
	mats.insert(roofMat);
	mats.insert(floorMat);

	int GridPointCount = 0;
	for (int i = 0; i < rooms.size(); i++)
	{
		double h = rooms[i].height;
		//导出第i个房间
		vector<OutSurface> surfaces;
		Wall wall;
		list<Wall> roomWalls;
		vector<stWindow> roomWindows;
		for (int j = 0; j < rooms[i].outWalls.size(); j++)
		{
			int index = rooms[i].outWalls[j];
			wall.line.s.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			wall.line.s.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			wall.line.e.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			wall.line.e.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
			wall.wallInfo.index = index;
			wall.wallInfo.type = sLine::OUT_WALL;
			wall.isOrder = true;
			roomWalls.push_back(wall);
		}
		for (int j = 0; j < rooms[i].inWalls.size(); j++)
		{
			int index = rooms[i].inWalls[j];
			wall.line.s.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			wall.line.s.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			wall.line.e.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			wall.line.e.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
			wall.wallInfo.index = index;
			wall.wallInfo.type = sLine::IN_WALL;
			wall.isOrder = true;
			roomWalls.push_back(wall);
		}
		for (int j = 0; j < rooms[i].windows.size(); j++)
		{
			int index = rooms[i].windows[j];
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

		vector<Vec2d> outPolygon;
		vector<Wall> outWalls;
		if (!CalClosedPolygon(roomWalls, outWalls, outPolygon))
		{
			CString msg;
			msg.Format(_T("房间%d没有封闭，不能导出"),i);
			AfxMessageBox(msg);
			return;
		}
		OutSurface surf;
		if (isAntiClock(outPolygon))//逆时针
		{
			//地面
			surf.name = "room";
			surf.name += '0'+ i; 
			surf.name += ".Floor";
			surf.type = "polygon";
			surf.mat = CStringToString(floorMat);
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = outPolygon.size() * 3; 
			surf.points.clear();
			for (int j = 0; j < outPolygon.size(); j++)
			{
				Vec3d p(outPolygon[j].x, outPolygon[j].y, 0.0);
				surf.points.push_back(p);
			}
			surfaces.push_back(surf);

			//棚顶
			surf.name = "room";
			surf.name += '0' + i; 
			surf.name += ".Roof";
			surf.type = "polygon";
			surf.mat = CStringToString(roofMat);
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = outPolygon.size() * 3; 
			surf.points.clear();
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				Vec3d p(outPolygon[j].x, outPolygon[j].y, h);
				surf.points.push_back(p);
			}
			surfaces.push_back(surf);

			//侧墙
			for (unsigned int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;

				mats.insert(mat);

				CString type = outWalls[j].wallInfo.type == sLine::OUT_WALL ? _T("外墙"): _T("内墙");
				vector<stWindow> roomWindowsInThisWall;
				for (unsigned int k = 0; k < roomWindows.size(); k++)
				{
					if (outWalls[j].wallInfo.index == roomWindows[k].wallIndex && 
						type == roomWindows[k].wallType)
					{
						roomWindowsInThisWall.push_back(roomWindows[k]);
					}
				}


				surf.name = "room";
				surf.name += '0'+ i; 
				surf.name += ".Wall";
				surf.name += '0'+j;
				surf.type = "polygon";
				surf.mat = CStringToString(mat);
				surf.args[0] = 0; surf.args[1] = 0;
				surf.points.clear();
				wallToPoints(true,h,outWalls[j],roomWindowsInThisWall,surf.points);
				surf.args[2] = surf.points.size()*3;
				/*if (outWalls[j].isOrder)
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
				}*/
				surfaces.push_back(surf);
			}
			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".Window";
				surf.name += '0' + j;
				surf.type = "polygon";
				surf.mat = CStringToString(CString(roomWindows[j].WinMaterial));
				surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 12; 

				mats.insert(CString(roomWindows[j].WinMaterial));

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

				////记录反物质材质
				//CString winOnWallMat = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				//CString winOnWallMatAnti = _T("anti_mat_");
				//winOnWallMatAnti += wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				//Material mt;
				//mt.type = "antimatter";
				//mt.name = CStringToString(winOnWallMatAnti);
				//mt.args.push_back("1");
				//mt.args.push_back(CStringToString(winOnWallMat));
				//mt.args.push_back("0");
				//mt.args.push_back("0");
				//antiMaterials.insert(mt);

				////计算窗户法线
				//vector<Vec3d>& wpoints = surfaces.back().points;
				//Vec3d crossW = (wpoints[1]-wpoints[0])/(wpoints[2]-wpoints[1]);
				//crossW.Normalize();
				////写反物质,正方向
				//surf.name = "room";
				//surf.name += '0' + i; 
				//surf.name += ".hole";
				//surf.name += '0' + j; 
				//surf.type = "polygon";
				//surf.mat = CStringToString(winOnWallMatAnti);
				//surf.args[0] = 0;
				//surf.args[1] = 0;
				//surf.args[2] = 12;
				//surf.points = wpoints;//与窗户坐标相同
				//for (unsigned int k = 0; k < surf.points.size(); k++)
				//{
				//	surf.points[k]+= crossW*2;
				//}
				//
				//surfaces.push_back(surf);

				////写反物质,反方向
				//surf.name = "room";
				//surf.name += '0' + i; 
				//surf.name += ".hole_inv";
				//surf.name += '0' + j; 
				//surf.type = "polygon";
				//surf.mat = CStringToString(winOnWallMatAnti);
				//surf.args[0] = 0;
				//surf.args[1] = 0;
				//surf.args[2] = 12;
				//surf.points = wpoints;//与窗户坐标相同
				//int ssz = surf.points.size();
				//for (unsigned int s = 0; s < ssz/2; s++)
				//{
				//	swap(surf.points[s], surf.points[ssz-s-1]);
				//}
				//for (unsigned int s = 0; s < ssz; s++)
				//{
				//	surf.points[s] -= crossW*2;
				//}
				//surfaces.push_back(surf);
			}
		}
		else
		{
			//地面
			surf.name = "room";
			surf.name += '0' + i; 
			surf.name += ".Floor";
			surf.type = "polygon";
			surf.mat = CStringToString(floorMat);
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = outPolygon.size() * 3; 
			surf.points.clear();
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				surf.points.push_back(Vec3d(outPolygon[j].x,outPolygon[j].y, 0.0));
			}
			surfaces.push_back(surf);
			//棚顶
			surf.name = "room";
			surf.name += '0'+ i; 
			surf.name += ".Roof";
			surf.type = "polygon";
			surf.mat = CStringToString(roofMat);
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = outPolygon.size() * 3; 
			surf.points.clear();
			for (int j = 0; j < outPolygon.size(); j++)
			{
				surf.points.push_back(Vec3d(outPolygon[j].x,outPolygon[j].y, h));
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
				mats.insert(mat);

				CString type = outWalls[j].wallInfo.type == sLine::OUT_WALL ? _T("外墙"): _T("内墙");
				vector<stWindow> roomWindowsInThisWall;
				for (unsigned int k = 0; k < roomWindows.size(); k++)
				{
					if (outWalls[j].wallInfo.index == roomWindows[k].wallIndex && 
						type == roomWindows[k].wallType)
					{
						roomWindowsInThisWall.push_back(roomWindows[k]);
					}
				}

				surf.name = "room";
				surf.name += '0'+ i; 
				surf.name += ".Wall";
				surf.name += '0'+j;
				surf.type = "polygon";
				surf.mat = CStringToString(mat);
				surf.args[0] = 0; surf.args[1] = 0;
				surf.points.clear();
				/*if (!outWalls[j].isOrder)
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
				}*/
				wallToPoints(false,h,outWalls[j],roomWindowsInThisWall,surf.points);
				surf.args[2] = surf.points.size()*3;
				surfaces.push_back(surf);
			}

			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".Window";
				surf.name += '0' + j;
				surf.type = "polygon";
				surf.mat = CStringToString(CString(roomWindows[j].WinMaterial));
				surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 12; 

				mats.insert(CString(roomWindows[j].WinMaterial));

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

				////记录反物质材质
				//CString winOnWallMat = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				//CString winOnWallMatAnti = _T("anti_mat_");
				//winOnWallMatAnti += wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				//Material mt;
				//mt.type = "antimatter";
				//mt.name = CStringToString(winOnWallMatAnti);
				//mt.args.push_back("1");
				//mt.args.push_back(CStringToString(winOnWallMat));
				//mt.args.push_back("0");
				//mt.args.push_back("0");
				//antiMaterials.insert(mt);

				////计算窗户法线
				//vector<Vec3d>& wpoints = surfaces.back().points;
				//Vec3d crossW = (wpoints[1]-wpoints[0])/(wpoints[2]-wpoints[1]);
				//crossW.Normalize();

				////写反物质,正方向
				//surf.name = "room";
				//surf.name += '0' + i; 
				//surf.name += ".hole";
				//surf.name += '0' + j; 
				//surf.type = "polygon";
				//surf.mat = CStringToString(winOnWallMatAnti);
				//surf.args[0] = 0;
				//surf.args[1] = 0;
				//surf.args[2] = 12;
				//surf.points = wpoints;//与窗户坐标相同
				//for (unsigned int k = 0; k < surf.points.size(); k++)
				//{
				//	surf.points[k] += crossW*2;
				//}
				//surfaces.push_back(surf);

				////写反物质,反方向
				//surf.name = "room";
				//surf.name += '0' + i; 
				//surf.name += ".hole_inv";
				//surf.name += '0' + j; 
				//surf.type = "polygon";
				//surf.mat = CStringToString(winOnWallMatAnti);
				//surf.args[0] = 0;
				//surf.args[1] = 0;
				//surf.args[2] = 12;
				//surf.points = wpoints;//与窗户坐标相同
				//unsigned int ssz = surf.points.size();
				//for (unsigned int s = 0; s < ssz/2; s++)
				//{
				//	swap(surf.points[s], surf.points[ssz-s-1]);
				//}
				//for (unsigned int s = 0; s < ssz; s++)
				//{
				//	surf.points[s] -= crossW*2;
				//}
				//surfaces.push_back(surf);
			}
		}

		outRooms[i].surfaces = surfaces;
		outRooms[i].girth = CalGirth(outPolygon);
		outRooms[i].area = CalArea(outPolygon);
		outRooms[i].height = h;
	}
}

void LumOutput(string lumFilePre, string controlFilePre, string personFilePre)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	vector<vector<OutLumSingle>> lumSingles;
	vector<vector<OutLumSet>> lumSets;
	vector<vector<OutControlSet>> controlSets;
	vector<vector<OutPerson>> persons;
	pMain->GetRoomProperty().OutputToLums(lumSingles, lumSets, controlSets, persons);

	//旋转角度
	double angle = pMain->GetOptionProperty().GetDataDouble(OPTION_NORTH);
	double anglep = angle / 180 * PI;
	for (unsigned int i = 0; i < controlSets.size(); i++)
	{
		string nameRoom = "_room";
		nameRoom += '0'+i;
		string lumFile = lumFilePre + nameRoom + "_lighting_system.gx";
		string controlFile = controlFilePre + nameRoom + "_control_system.ctl";
		string personFile = personFilePre + nameRoom + "_occupancy.occ";
		ofstream lumOut(lumFile);
		ofstream controlOut(controlFile);
		ofstream personOut(personFile);
		
		//第i个房间
		for (unsigned int j = 0; j < controlSets[i].size(); j++)
		{
			//第j个控制分组
			CMFCPropertyGridProperty* control = pMain->GetRoomProperty().getPropList()->GetProperty(i)->GetSubItem(ROOM_CONTROL_SET)->GetSubItem(j);
			int keyGrid = -1;
			vector<double> args;
			for (unsigned int k = 0; k < control->GetSubItemsCount(); k++)
			{
				if (control->GetSubItem(k)->GetData() == CONTROL_SET_ARGS)
				{
					args.push_back(control->GetSubItem(k)->GetValue().dblVal);
				}
				else if (control->GetSubItem(k)->GetData() == CONTROL_SET_ARGS_KEYGRID)
				{
					keyGrid = control->GetSubItem(k)->GetValue().intVal;
				}

			}
			vector<int> personIds;

			for (unsigned int k = 0; k < persons[i].size(); k++)
			{
				if (find(persons[i][k].controlIds.begin(),persons[i][k].controlIds.end(),j) != persons[i][k].controlIds.end())
				{
					personIds.push_back(k);
				}
			}
			//控制号j
			//单个灯具
			for (unsigned int k = 0; k < controlSets[i][j].lumSingles.size(); k++)
			{
				//导出灯具文件
				int lumIndex = controlSets[i][j].lumSingles[k];
				OutLumSingle& ols = lumSingles[i][lumIndex];
				Vec2d p(ols.p.x, ols.p.y);
				p = Rotate(p, anglep);
				string lumPath = "lum_data/";
				lumPath += CStringToString(CString(ols.type)) + ".IES";
				lumOut << "void " << "lum_room"<< i << "_Single"<<lumIndex << " " << lumPath << " " << i << " " << j << " " 
					<< p.x <<" "<< p.y << " " << ols.p.z << " " 
					<< ols.np.x <<" "<< ols.np.y << " " << ols.np.z << " " 
					<< CStringToString(CString(controlSets[i][j].type));
				for (unsigned int m = 0; m < personIds.size(); m++)
				{
					lumOut << " " << personIds[m];
				}
				lumOut << endl;

				//导出控制文件
				controlOut << "lum_room" << i << "_Single"<< lumIndex << " " << CStringToString(CString(controlSets[i][j].type));
				if (keyGrid >= 0)
					controlOut << " " << keyGrid;

				for (unsigned int m = 0; m < args.size(); m++)
				{
					controlOut << " " << args[m];
				}
				controlOut << endl;
			}
			//灯具组
			for (unsigned int k = 0; k < controlSets[i][j].lumSets.size(); k++)
			{
				int lumIndex = controlSets[i][j].lumSets[k];
				OutLumSet& ols = lumSets[i][lumIndex];
				vector<Vec2d> outLums;
				ols.outputLums(outLums);
				for (unsigned int m = 0; m < outLums.size(); m++)
				{
					//导出灯具文件
					Vec2d p(outLums[m].x, outLums[m].y);
					p = Rotate(p, anglep);
					string lumPath = "lum_data/";
					lumPath += CStringToString(CString(ols.type)) + ".IES";
					lumOut << "void " << "lum_room"<< i << "_Set"<<lumIndex <<"_"<< m << " " 
						<< lumPath << " " << i << " " << j << " " 
						<< p.x <<" "<< p.y << " " << ols.z << " " 
						<< ols.np.x <<" "<< ols.np.y << " " << ols.np.z << " " 
						<< CStringToString(CString(controlSets[i][j].type));
					for (unsigned int m = 0; m < personIds.size(); m++)
					{
						lumOut << " " << personIds[m];
					}
					lumOut << endl;

					//导出控制文件
					controlOut << "lum_room" << i << "_Set"<< lumIndex <<"_"<< m << " "
						<< CStringToString(CString(controlSets[i][j].type));
					if (keyGrid >= 0)
						controlOut << " " << keyGrid;

					for (unsigned int n = 0; n < args.size(); n++)
					{
						controlOut << " " << args[n];
					}
					controlOut << endl;
				}
			}
		}
		//导出人员信息
		vector<Room> rooms;
		pMain->GetRoomProperty().OutputToRooms(rooms);
		double room_E = rooms[i].type.e;
		for (unsigned int j = 0; j < persons[i].size(); j++)
		{
			//第j个人
			personOut << "void " << j << " " <<i << " " <<CStringToString(CString(persons[i][j].schedule_type)) << " "
				<< CStringToString(CString(persons[i][j].behavior_type));

			for (unsigned int k = 0; k < persons[i][j].controlIds.size(); k++)
			{
				personOut << " " << persons[i][j].controlIds[k];
			}
			personOut << endl;
			CString be_type = CString(persons[i][j].behavior_type);
			if (be_type == _T("Economic_type"))
			{
				//0.5*E_standard 0.3*E_standard 6
				personOut << "on_type " << room_E* 0.5 << " " << room_E * 0.3 << " " << 6 << endl;
				//1 1 1.5*E_standard 0.6*E_standard 6
				personOut << "off_type " << 1 << " " <<1 << " " << room_E* 1.5 << " " << room_E * 0.6 << " " << 6 << endl;
			}
			else if (be_type == _T("Common_type"))
			{
				//E_standard 0.5*E_standard 3 0.5
				personOut << "on_type " << room_E << " " << room_E * 0.5 << " " << 3 << " " << 0.5 << endl;
				//0.5 1 2*E_standard E_standard 3
				personOut << "off_type " << 0.5 << " " <<1 << " " << room_E * 2 << " " << room_E << " " << 3 << endl;
			}
			else if (be_type == _T("Comfortable_type"))
			{
				//1
				personOut << "on_type " << 1 << endl;
				//1
				personOut << "off_type " << 1 << endl;
			}
		}
		
		if ((size_t)lumOut.tellp() == 0)
		{
			lumOut.close();
			DeleteFile(StringToCString(lumFile));
		}
		if ((size_t)controlOut.tellp() == 0)
		{
			controlOut.close();
			DeleteFile(StringToCString(controlFile));
		}
		if ((size_t)personOut.tellp() == 0)
		{
			personOut.close();
			DeleteFile(StringToCString(personFile));
		}
		lumOut.close();
		controlOut.close();
		personOut.close();
	}
}
void RoomOutput(string roomFile, string gridDir)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	if (!pMain || !pDoc)
		return;

	vector<Room> rooms;
	pMain->GetRoomProperty().OutputToRooms(rooms);
	vector<vector<OutLumSingle>> lumSingles;
	vector<vector<OutLumSet>> lumSets;
	vector<vector<OutControlSet>> controlSets;
	vector<vector<OutPerson>> persons;
	pMain->GetRoomProperty().OutputToLums(lumSingles, lumSets, controlSets, persons);

	ofstream out(roomFile);

	if (!out.is_open())
	{
		AfxMessageBox(_T("文件创建失败"));
		return;
	}
	//到处城市信息
	CString strCity = pMain->GetOptionProperty().GetDataCString(OPTION_CITY);
	out << "City_name "  << CStringToString(strCity) << endl;
	string cityPath = "wea_data/";
	out << "City_path "  << cityPath + CStringToString(strCity) + "_5min.wea" << endl << endl;

	vector<OutRoom> outRooms;
	set<CString> outMats;
	set<Material> antiMats;
	RoomOutToVector(outRooms, outMats, antiMats);

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);

	double angle = pMain->GetOptionProperty().GetDataDouble(OPTION_NORTH);
	double anglep = angle / 180 * PI;
	for (int i = 0; i < rooms.size(); i++)
	{
		//导出第i个房间

		out << "void Room_"  << i << endl;
		out << "room_Type " << pDoc->getTranslate()[CStringToString(CString(rooms[i].type.name))] << endl;
		out << "room_Girth " << outRooms[i].girth << "mm" << endl;
		out << "room_Area " << outRooms[i].area / 1000000 << "m*m" << endl;
		out << "room_Height " << outRooms[i].height << "mm" << endl << endl;

		out << "start geometry" << endl;
		for (int a = 0; a < outRooms[i].surfaces.size(); a++)
		{
			out << outRooms[i].surfaces[a].name <<endl;
		}
		out << "end geometry" << endl << endl;

		out << "start luminaire" << endl;
		for (unsigned int a = 0; a < lumSingles[i].size(); a++)
		{
			out << "lum_room"<< i << "_Single"<< a << endl;
		}
		for (unsigned int a = 0; a < lumSets[i].size(); a++)
		{
			OutLumSet& ols = lumSets[i][a];
			vector<Vec2d> outLums;
			ols.outputLums(outLums);
			for (unsigned int n = 0; n < outLums.size(); n++)
			{
				out << "lum_room"<< i << "_Set"<< a <<"_"<< n << endl;
			}
		}
		out << "end luminaire" << endl << endl;

		if (!rooms[i].grid.points.empty())
		{
			string gridFile = gridDir + "_room";
			gridFile += '0'+i;
			ofstream grid1Out(gridFile +"_grid1.pts");
			ofstream grid2Out(gridFile +"_grid2.pts");
			if (!grid1Out.is_open() || !grid2Out.is_open())
			{
				AfxMessageBox(_T("文件创建失败"));
				return;
			}

			vector<GridPoint>& points = rooms[i].grid.points;
			out << "start pts" << endl;
			for (int a = 0; a < points.size(); a++)
			{
				out << a << " ";
				if (points[a].isKey)
				{
					out <<"key";
				}
				out << endl;

				Vec2d p(points[a].p.x, points[a].p.y);
				p = Rotate(p, anglep);

				grid1Out << p.x << " " << p.y<<" "<< points[a].p.z << " "<<0<<" "<<0<<" "<<1<<endl;

				if (points[a].isKey)
				{
					grid2Out << p.x << " " << p.y<<" "<< points[a].p.z << " "<<0<<" "<<0<<" "<<1<<endl;
				}
			}
			out << "end pts" << endl << endl;

			grid1Out.close();
			grid2Out.close();
		}

		out << "start user" << endl;
		for (int a = 0; a < persons[i].size(); a++)
		{
			out << a <<endl;
		}
		out << "end user" << endl << endl;

	}
}
