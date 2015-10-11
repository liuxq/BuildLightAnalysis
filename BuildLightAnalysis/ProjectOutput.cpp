#include "stdafx.h"
#include "ProjectOutput.h"
#include "MainFrm.h"
#include "MathUtility.h"

#include <fstream>
#include <list>



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

	double h = pMain->GetOptionProperty().GetDataDouble(OPTION_LEVEL_HEIGHT);
	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);

	vector<OutRoom> outRooms;
	RoomOutToVector(outRooms, outMats, antiMaterials);

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
				out << surf.points[k].x << " " << surf.points[k].y << " " << surf.points[k].z << endl;
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
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 
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
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 
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

				mats.insert(mat);

				surf.name = "room";
				surf.name += '0'+ i; 
				surf.name += ".Wall";
				surf.name += '0'+j;
				surf.type = "polygon";
				surf.mat = CStringToString(mat);
				surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 
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
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".Window";
				surf.name += '0' + j;
				surf.type = "polygon";
				surf.mat = CStringToString(CString(roomWindows[j].WinMaterial));
				surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 

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

				//记录反物质材质
				CString winOnWallMat = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				CString winOnWallMatAnti = _T("anti_mat_");
				winOnWallMatAnti += wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				Material mt;
				mt.type = "antimatter";
				mt.name = CStringToString(winOnWallMatAnti);
				mt.args.push_back("1");
				mt.args.push_back(CStringToString(winOnWallMat));
				mt.args.push_back("0");
				mt.args.push_back("0");
				antiMaterials.insert(mt);

				//写反物质,正方向
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".hole";
				surf.name += '0' + j; 
				surf.type = "polygon";
				surf.mat = CStringToString(winOnWallMatAnti);
				surf.args[0] = 0;
				surf.args[1] = 0;
				surf.args[2] = 12;
				surf.points = surfaces.back().points;//与窗户坐标相同
				surfaces.push_back(surf);

				//写反物质,反方向
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".hole_inv";
				surf.name += '0' + j; 
				surf.type = "polygon";
				surf.mat = CStringToString(winOnWallMatAnti);
				surf.args[0] = 0;
				surf.args[1] = 0;
				surf.args[2] = 12;
				surf.points = surfaces.back().points;//与窗户坐标相同
				int ssz = surf.points.size();
				for (int s = 0; s < ssz/2; s++)
				{
					swap(surf.points[s], surf.points[ssz-s-1]);
				}
				surfaces.push_back(surf);
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
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 
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
			surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 
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

				surf.name = "room";
				surf.name += '0'+ i; 
				surf.name += ".Wall";
				surf.name += '0'+j;
				surf.type = "polygon";
				surf.mat = CStringToString(mat);
				surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 
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
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".Window";
				surf.name += '0' + j;
				surf.type = "polygon";
				surf.mat = CStringToString(CString(roomWindows[j].WinMaterial));
				surf.args[0] = 0; surf.args[1] = 0; surf.args[2] = 9; 

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

				//记录反物质材质
				CString winOnWallMat = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				CString winOnWallMatAnti = _T("anti_mat_");
				winOnWallMatAnti += wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(2)->GetValue().bstrVal;
				Material mt;
				mt.type = "antimatter";
				mt.name = CStringToString(winOnWallMatAnti);
				mt.args.push_back("1");
				mt.args.push_back(CStringToString(winOnWallMat));
				mt.args.push_back("0");
				mt.args.push_back("0");
				antiMaterials.insert(mt);

				//写反物质,正方向
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".hole";
				surf.name += '0' + j; 
				surf.type = "polygon";
				surf.mat = CStringToString(winOnWallMatAnti);
				surf.args[0] = 0;
				surf.args[1] = 0;
				surf.args[2] = 12;
				surf.points = surfaces.back().points;//与窗户坐标相同
				surfaces.push_back(surf);

				//写反物质,反方向
				surf.name = "room";
				surf.name += '0' + i; 
				surf.name += ".hole_inv";
				surf.name += '0' + j; 
				surf.type = "polygon";
				surf.mat = CStringToString(winOnWallMatAnti);
				surf.args[0] = 0;
				surf.args[1] = 0;
				surf.args[2] = 12;
				surf.points = surfaces.back().points;//与窗户坐标相同
				int ssz = surf.points.size();
				for (int s = 0; s < ssz/2; s++)
				{
					swap(surf.points[s], surf.points[ssz-s-1]);
				}
				surfaces.push_back(surf);
			}
		}

		outRooms[i].surfaces = surfaces;
		outRooms[i].girth = CalGirth(outPolygon);
		outRooms[i].area = CalArea(outPolygon);
		outRooms[i].height = h;
	}
}

void LumOutput(string lumFile, string grid1File, string grid2File)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;


}
void RoomOutput(string roomFile, string grid1File, string grid2File)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	vector<Room> rooms;
	pMain->GetRoomProperty().OutputToRooms(rooms);

	ofstream out(roomFile);
	ofstream grid1Out(grid1File);
	ofstream grid2Out(grid2File);
	if (!out.is_open() || !grid1Out.is_open() || !grid2Out.is_open())
	{
		AfxMessageBox(_T("文件创建失败"));
		return;
	}

	vector<OutRoom> outRooms;
	set<CString> outMats;
	set<Material> antiMats;
	RoomOutToVector(outRooms, outMats, antiMats);

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	double h = pMain->GetOptionProperty().GetDataDouble(OPTION_LEVEL_HEIGHT);
	CString roofMat = pMain->GetOptionProperty().GetDataCString(OPTION_ROOF_MAT);
	CString floorMat = pMain->GetOptionProperty().GetDataCString(OPTION_FLOOR_MAT);
	int GridPointCount = 0;
	for (int i = 0; i < rooms.size(); i++)
	{
		//导出第i个房间

		out << "void Room_"  << i << endl;
		out << "room_Type " << CStringToString(CString(rooms[i].type.name)) << endl;
		out << "room_Girth " << outRooms[i].girth << endl;
		out << "room_Area " << outRooms[i].area / 1000000 << "平方米" << endl;
		out << "room_Height " << outRooms[i].height << endl << endl;
		out << "start geometry" << endl;

		for (int a = 0; a < outRooms[i].surfaces.size(); a++)
		{
			out << outRooms[i].surfaces[a].name <<endl;
		}

		out << "end geometry" << endl << endl;

		if (!rooms[i].grid.points.empty())
		{
			vector<GridPoint>& points = rooms[i].grid.points;
			out << "start pts" << endl;
			for (int a = 0; a < points.size(); a++)
			{
				out << GridPointCount << " ";
				if (points[a].isKey)
				{
					out <<"key";
				}
				out << endl;
				
				grid1Out << points[a].p.x << " " << points[a].p.y<<" "<< points[a].p.z << " "<<0<<" "<<0<<" "<<1<<endl;

				if (points[a].isKey)
				{
					grid2Out << points[a].p.x << " " << points[a].p.y<<" "<< points[a].p.z << " "<<0<<" "<<0<<" "<<1<<endl;
				}
				GridPointCount++;
			}
			out << "end pts" << endl << endl;
		}
		
	}
}