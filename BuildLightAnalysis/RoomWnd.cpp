
#include "stdafx.h"

#include "RoomWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "BuildLightAnalysis.h"
#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"

#include "Serializer.h"

#include <list>
#include "MathUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CRoomWnd::CRoomWnd()
{
}

CRoomWnd::~CRoomWnd()
{
}

BEGIN_MESSAGE_MAP(CRoomWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()

	ON_COMMAND(IDC_ROOM_INSERT_BUTTON, OnNewRoom)
	ON_UPDATE_COMMAND_UI(IDC_ROOM_INSERT_BUTTON, OnUpdateProperties1)
	ON_COMMAND(IDC_ROOM_DELETE_BUTTON, OnDeleteRoom)
	ON_UPDATE_COMMAND_UI(IDC_ROOM_DELETE_BUTTON, OnUpdateProperties1)

	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_ROOM_CAL_GRID, &CRoomWnd::OnRoomCalGrid)
	ON_COMMAND(ID_ROOM_ADD_LUMINAIRE_SINGLE, &CRoomWnd::OnRoomAddLuminaireSingle)
	ON_COMMAND(ID_ROOM_ADD_LUMINAIRE_SET, &CRoomWnd::OnRoomAddLuminaireSet)
	ON_COMMAND(ID_ROOM_ADD_CONTROL_SET, &CRoomWnd::OnRoomAddControlSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CRoomWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectButton;
	GetClientRect(rectClient);

	int cyBut = 20;//rectButton.Size().cy;

	m_insertButton.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width()/2, cyBut, SWP_NOZORDER);
	m_deleteButton.SetWindowPos(NULL, rectClient.left + rectClient.Width()/2, rectClient.top, rectClient.Width()/2, cyBut, SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyBut, rectClient.Width(), rectClient.Height() -(cyBut), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CRoomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	if (!m_insertButton.Create(_T("新建房间"),dwViewStyle, rectDummy, this, IDC_ROOM_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("删除房间"),dwViewStyle, rectDummy, this, IDC_ROOM_DELETE_BUTTON))
	{
		return -1;      // fail to create
	}

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_ROOM_CTRL))
	{
		TRACE0("Failed to create Room Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();
	return 0;
}

void CRoomWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CRoomWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CRoomWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CRoomWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CRoomWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CRoomWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void CRoomWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CRoomWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CRoomWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_insertButton.SetFont(&m_fntPropList);
	m_deleteButton.SetFont(&m_fntPropList);
}
PropertyGridProperty* CRoomWnd::AddRoom(CString roomName)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	if (!pMain || !pDoc)
		return NULL;

	double h = pMain->GetOptionProperty().GetDataDouble(OPTION_LEVEL_HEIGHT);

	PropertyGridProperty* pRoom = new PropertyGridProperty(roomName, 0, FALSE);

	PropertyGridProperty* pType = new PropertyGridProperty(_T("类型"), _T("普通办公室"), _T("房间类型"));
	vector<RoomType>& roomTypes = pDoc->getRoomTypes();
	for (int i = 0; i < roomTypes.size(); i++)
	{
		pType->AddOption(CString(roomTypes[i].name));
	}
	pType->AllowEdit(FALSE);
	PropertyGridProperty* pHeight = new PropertyGridProperty(_T("高度(mm)"), (_variant_t)h, _T("房间类型"));
	PropertyGridProperty* pOutWall = new PropertyGridProperty(_T("外墙"), 0, TRUE);
	PropertyGridProperty* pInWall = new PropertyGridProperty(_T("内墙"), 0, TRUE);
	PropertyGridProperty* pWin = new PropertyGridProperty(_T("窗户"), ROOM_WINDOW, TRUE);
	PropertyGridProperty* pGrid = new PropertyGridProperty(_T("计算网格"), ROOM_GRID, FALSE);
	PropertyGridProperty* pSingleLuminaire = new PropertyGridProperty(_T("单个灯具"), ROOM_SINGLE_LUMINAIRE, FALSE);
	PropertyGridProperty* pSetLuminaire = new PropertyGridProperty(_T("灯具组"), ROOM_SET_LUMINAIRE, FALSE);
	PropertyGridProperty* pControlSet = new PropertyGridProperty(_T("控制分组"), ROOM_CONTROL_SET, FALSE);

	PropertyGridProperty* pOffset = new PropertyGridProperty(_T("内偏移(mm)"), (_variant_t)120.0, _T("内偏移"));
	PropertyGridProperty* pMeshLen = new PropertyGridProperty(_T("网格边长(mm)"), (_variant_t)120.0, _T("网格边长"));
	pGrid->AddSubItem(pOffset);
	pGrid->AddSubItem(pMeshLen);

	pRoom->AddSubItem(pType);
	pRoom->AddSubItem(pHeight);
	pRoom->AddSubItem(pOutWall);
	pRoom->AddSubItem(pInWall);
	pRoom->AddSubItem(pWin);
	pRoom->AddSubItem(pGrid);
	pRoom->AddSubItem(pSingleLuminaire);
	pRoom->AddSubItem(pSetLuminaire);
	pRoom->AddSubItem(pControlSet);

	m_wndPropList.AddProperty(pRoom);
	m_wndPropList.SetCurSel(pRoom);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
	return pRoom;
}
void CRoomWnd::OnNewRoom()
{
	int count = m_wndPropList.GetPropertyCount();
	CString strCount;
	strCount.Format(_T("房间%d"),count);
	AddRoom(strCount);
	m_wndPropList.AdjustLayout();
}

void CRoomWnd::CalGrid(CMFCPropertyGridProperty* pGrid)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain || !pGrid)
		return;

	if (pGrid->GetSubItemsCount() == GRID_POINTS+1)
	{
		CMFCPropertyGridProperty* pPoints = pGrid->GetSubItem(GRID_POINTS);
		if (pPoints)
			pGrid->RemoveSubItem(pPoints);
	}


	double offset = pGrid->GetSubItem(GRID_OFFSET)->GetValue().dblVal;
	double meshLen = pGrid->GetSubItem(GRID_MESHLEN)->GetValue().dblVal;

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	//导出第i个房间
	CMFCPropertyGridProperty* pRoom = pGrid->GetParent();
	CMFCPropertyGridProperty* pOutWallIndexs = pRoom->GetSubItem(ROOM_OUT_WALL);
	CMFCPropertyGridProperty* pInWallIndexs = pRoom->GetSubItem(ROOM_IN_WALL);
	Wall wall;
	list<Wall> roomWalls;
	for (int j = 0; j < pOutWallIndexs->GetSubItemsCount(); j++)
	{
		int index = pOutWallIndexs->GetSubItem(j)->GetValue().intVal;
		wall.line.s.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		wall.line.s.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
		wall.line.e.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		wall.line.e.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
		wall.wallInfo.index = index;
		wall.wallInfo.type = sLine::OUT_WALL;
		wall.isOrder = true;
		roomWalls.push_back(wall);
	}
	for (int j = 0; j < pInWallIndexs->GetSubItemsCount(); j++)
	{
		int index = pInWallIndexs->GetSubItem(j)->GetValue().intVal;
		wall.line.s.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		wall.line.s.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
		wall.line.e.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		wall.line.e.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
		wall.wallInfo.index = index;
		wall.wallInfo.type = sLine::IN_WALL;
		wall.isOrder = true;
		roomWalls.push_back(wall);
	}
	vector<Vec2d> outPolygon;
	vector<Wall> outWalls;
	if (!CalClosedPolygon(roomWalls, outWalls, outPolygon))
	{
		return;
	}
	vector<Vec2d> gridPoints;
	CalGridFromPolygon(outPolygon,offset,meshLen,gridPoints);

	CMFCPropertyGridProperty* pGridList = new CMFCPropertyGridProperty(_T("计算点"),0,FALSE);
	for (int i = 0; i < gridPoints.size(); i++)
	{
		CMFCPropertyGridProperty* pGridPoint = new CMFCPropertyGridProperty(_T("点(mm)"),0,TRUE);
		CMFCPropertyGridProperty* pGridPointX = new CMFCPropertyGridProperty(_T("X"),(_variant_t)gridPoints[i].x,_T("X"));
		CMFCPropertyGridProperty* pGridPointY = new CMFCPropertyGridProperty(_T("Y"),(_variant_t)gridPoints[i].y,_T("Y"));
		CMFCPropertyGridProperty* pGridPointZ = new CMFCPropertyGridProperty(_T("Z"),(_variant_t)750.0,_T("Z"));
		pGridPoint->AddSubItem(pGridPointX);
		pGridPoint->AddSubItem(pGridPointY);
		pGridPoint->AddSubItem(pGridPointZ);

		pGridList->AddSubItem(pGridPoint);
	}
	pGrid->AddSubItem(pGridList);

	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pGrid));

	m_wndPropList.AdjustLayout();
}

void CRoomWnd::OnUpdateProperties1(CCmdUI* pCmdUI)
{

}
void CRoomWnd::OnDeleteRoom()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem && !selItem->GetParent())
	{
		CString name = selItem->GetName();
		CString namePost = name.Right(name.GetLength() - 2);
		int index = _ttoi(namePost);

		((PropertyGridProperty*)selItem)->RemoveAllSubItem();
		m_wndPropList.DeleteProperty(selItem);

		//重新设置一下坐标编号
		int count = m_wndPropList.GetPropertyCount();
		CString strName;
		for (int i = 0; i < count; i++)
		{
			strName.Format(_T("房间%d"),i);
			m_wndPropList.GetProperty(i)->SetName(strName);

			if (i == index)
				m_wndPropList.SetCurSel(m_wndPropList.GetProperty(i));
		}
	}
	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}
void CRoomWnd::DeleteWindowFromRoom(int winIndex)
{
	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty* pWin = m_wndPropList.GetProperty(i)->GetSubItem(ROOM_WINDOW);
		for (int j = 0; j < pWin->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty* p = pWin->GetSubItem(j);
			int oriIndex = p->GetValue().lVal;
			if (oriIndex == winIndex)
			{
				pWin->RemoveSubItem(p);
				j--;
			}
			else if (oriIndex > winIndex)
			{
				p->SetValue((_variant_t)(oriIndex-1));
			}
		}
	}
	m_wndPropList.AdjustLayout();
	
}
bool CRoomWnd::AddToSelectedRoom(int type, int index)
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	while (selItem && selItem->GetParent())
	{
		selItem = selItem->GetParent();
	}
	if (selItem)
	{
		CMFCPropertyGridProperty* subItem = selItem->GetSubItem(type);
		for (int i = 0; i < subItem->GetSubItemsCount(); i++)
		{
			CMFCPropertyGridProperty* p = subItem->GetSubItem(i);
			if (p->GetValue().intVal == index)
			{
				AfxMessageBox(_T("该房间已经存在该墙或窗户"));
				return false;
			}
		}
		PropertyGridProperty* pItem = new PropertyGridProperty(_T("编号"), (_variant_t)index, _T("房间内的项目"));
		subItem->AddSubItem(pItem);
		m_wndPropList.UpdateProperty((PropertyGridProperty*)subItem);
		m_wndPropList.AdjustLayout();
		return true;
	}
	else
	{
		AfxMessageBox(_T("尚未选择任何房间，请选择要添加进的房间"));
		return false;
	}
}

void CRoomWnd::DeleteAllRoom()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.AdjustLayout();
}
LRESULT CRoomWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

	if (pProp->GetParent() && pProp->GetParent()->GetData() == ROOM_GRID)
	{
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			return 0;
		CMFCPropertyGridProperty* grid = pProp->GetParent();
		CalGrid(grid);
		//更新视图     
		pMain->GetActiveView()->Invalidate(); 
		return 1;
	}
	if (pProp->GetParent() && pProp->GetParent()->GetData() == ROOM_SET_LUMINAIRE)
	{
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			return 0;
		CMFCPropertyGridProperty* lumSet = pProp->GetParent();
		CalLumSet(lumSet);
		//更新视图     
		pMain->GetActiveView()->Invalidate(); 
		return 1;
	}
	return 0;
}
void CRoomWnd::OutputToRooms(vector<Room>& rooms)
{
	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		Room room;
		CMFCPropertyGridProperty* pRoom = m_wndPropList.GetProperty(i);
		CMFCPropertyGridProperty* pType = pRoom->GetSubItem(ROOM_TYPE);
		CMFCPropertyGridProperty* pHeight = pRoom->GetSubItem(ROOM_HEIGHT);
		CMFCPropertyGridProperty* pOutWall = pRoom->GetSubItem(ROOM_OUT_WALL);
		CMFCPropertyGridProperty* pInWall = pRoom->GetSubItem(ROOM_IN_WALL);
		CMFCPropertyGridProperty* pWindow = pRoom->GetSubItem(ROOM_WINDOW);
		CString type = pType->GetValue().bstrVal;
		_tcscpy_s(room.type.name, type);
		room.height = pHeight->GetValue().dblVal;
		for (int j = 0; j < pOutWall->GetSubItemsCount(); j++)
		{
			int index = pOutWall->GetSubItem(j)->GetValue().intVal;
			room.outWalls.push_back(index);
		}
		for (int j = 0; j < pInWall->GetSubItemsCount(); j++)
		{
			int index = pInWall->GetSubItem(j)->GetValue().intVal;
			room.inWalls.push_back(index);
		}
		for (int j = 0; j < pWindow->GetSubItemsCount(); j++)
		{
			int index = pWindow->GetSubItem(j)->GetValue().intVal;
			room.windows.push_back(index);
		}

		//计算点
		CMFCPropertyGridProperty* pGrid = pRoom->GetSubItem(ROOM_GRID);
		room.grid.offset = pGrid->GetSubItem(GRID_OFFSET)->GetValue().dblVal;
		room.grid.meshLen = pGrid->GetSubItem(GRID_MESHLEN)->GetValue().dblVal;
		if (pGrid->GetSubItemsCount() == GRID_POINTS+1)
		{
			CMFCPropertyGridProperty* pPoints = pGrid->GetSubItem(GRID_POINTS);
			for (int j = 0; j < pPoints->GetSubItemsCount(); j++)
			{
				GridPoint p;
				CString _name = pPoints->GetSubItem(j)->GetName();
				if (_name.Left(3) == _T("关键点"))
					p.isKey = true;
				else
					p.isKey = false;
				p.p.x = pPoints->GetSubItem(j)->GetSubItem(0)->GetValue().dblVal;
				p.p.y = pPoints->GetSubItem(j)->GetSubItem(1)->GetValue().dblVal;
				p.p.z = pPoints->GetSubItem(j)->GetSubItem(2)->GetValue().dblVal;
				room.grid.points.push_back(p);
			}
		}
		rooms.push_back(room);
	}
}
void CRoomWnd::save(ofstream& out)
{
	vector<Room> rooms;
	OutputToRooms(rooms);

	int size = rooms.size();
	out.write((char *)&size, sizeof(size));
	for (int i = 0; i < size; i++)
	{
		serializer<RoomType>::write(out, &rooms[i].type);
		serializer<double>::write(out, &rooms[i].height);
		serializer<int>::write(out, &rooms[i].outWalls);
		serializer<int>::write(out, &rooms[i].inWalls);
		serializer<int>::write(out, &rooms[i].windows);
		serializer<double>::write(out, &rooms[i].grid.offset);
		serializer<double>::write(out, &rooms[i].grid.meshLen);
		serializer<GridPoint>::write(out, &rooms[i].grid.points);
	}
	
}
void CRoomWnd::load(ifstream& in)
{
	DeleteAllRoom();

	int size = 0;
	in.read((char *)&size, sizeof(size));
	vector<Room> rooms(size);
	for (int i = 0; i < size; i++)
	{
		serializer<RoomType>::read(in, &rooms[i].type);
		serializer<double>::read(in, &rooms[i].height);
		serializer<int>::read(in, &rooms[i].outWalls);
		serializer<int>::read(in, &rooms[i].inWalls);
		serializer<int>::read(in, &rooms[i].windows);
		serializer<double>::read(in, &rooms[i].grid.offset);
		serializer<double>::read(in, &rooms[i].grid.meshLen);
		serializer<GridPoint>::read(in, &rooms[i].grid.points);
	}
	for (int i = 0; i < size; i++)
	{
		CString strCount;
		strCount.Format(_T("房间%d"),i);
		PropertyGridProperty* pRoom = AddRoom(strCount);
		CMFCPropertyGridProperty* pType = pRoom->GetSubItem(ROOM_TYPE);
		CMFCPropertyGridProperty* pHeight = pRoom->GetSubItem(ROOM_HEIGHT);
		CMFCPropertyGridProperty* pOutWall = pRoom->GetSubItem(ROOM_OUT_WALL);
		CMFCPropertyGridProperty* pInWall = pRoom->GetSubItem(ROOM_IN_WALL);
		CMFCPropertyGridProperty* pWindow = pRoom->GetSubItem(ROOM_WINDOW);
		CMFCPropertyGridProperty* pGrid = pRoom->GetSubItem(ROOM_GRID);
		
		pType->SetValue(rooms[i].type.name);
		pHeight->SetValue(rooms[i].height);
		for (int j = 0; j < rooms[i].outWalls.size(); j++)
		{
			CMFCPropertyGridProperty* outWallindex = new CMFCPropertyGridProperty(_T("编号"),(_variant_t)rooms[i].outWalls[j], _T("外墙编号"));
			pOutWall->AddSubItem(outWallindex);
		}
		for (int j = 0; j < rooms[i].inWalls.size(); j++)
		{
			CMFCPropertyGridProperty* inWallindex = new CMFCPropertyGridProperty(_T("编号"),(_variant_t)rooms[i].inWalls[j], _T("内墙编号"));
			pInWall->AddSubItem(inWallindex);
		}
		for (int j = 0; j < rooms[i].windows.size(); j++)
		{
			CMFCPropertyGridProperty* windowIndex = new CMFCPropertyGridProperty(_T("编号"),(_variant_t)rooms[i].windows[j], _T("窗户编号"));
			pWindow->AddSubItem(windowIndex);
		}
		pGrid->GetSubItem(GRID_OFFSET)->SetValue(rooms[i].grid.offset);
		pGrid->GetSubItem(GRID_MESHLEN)->SetValue(rooms[i].grid.meshLen);
		CMFCPropertyGridProperty* pGridList = new CMFCPropertyGridProperty(_T("计算点"),0,FALSE);
		pGrid->AddSubItem(pGridList);
		for (int j = 0; j < rooms[i].grid.points.size(); j++)
		{
			GridPoint& gp = rooms[i].grid.points[j];
			CMFCPropertyGridProperty* point = new CMFCPropertyGridProperty(gp.isKey?_T("关键点(mm)"):_T("点(mm)"), 0, TRUE);
			CMFCPropertyGridProperty* pGridPointX = new CMFCPropertyGridProperty(_T("X"),(_variant_t)gp.p.x,_T("X"));
			CMFCPropertyGridProperty* pGridPointY = new CMFCPropertyGridProperty(_T("Y"),(_variant_t)gp.p.y,_T("Y"));
			CMFCPropertyGridProperty* pGridPointZ = new CMFCPropertyGridProperty(_T("Z"),(_variant_t)gp.p.z,_T("Z"));
			point->AddSubItem(pGridPointX);
			point->AddSubItem(pGridPointY);
			point->AddSubItem(pGridPointZ);
			pGridList->AddSubItem(point);
		}

		m_wndPropList.UpdateProperty(pRoom);
	}
	m_wndPropList.AdjustLayout();
}

void CRoomWnd::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_ROOM, point.x, point.y, this, TRUE);
#endif
	}

}

void CRoomWnd::OnRoomCalGrid()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	while (selItem && selItem->GetParent())
	{
		selItem = selItem->GetParent();
	}
	if (selItem)
	{
		CMFCPropertyGridProperty* pGrid = selItem->GetSubItem(ROOM_GRID);
		CalGrid(pGrid);
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			return;
		pMain->GetActiveView()->Invalidate(); 
	}
	else
	{
		AfxMessageBox(_T("没有选择任何房间！"));
	}
}
void CRoomWnd::AddSingleLuminaire(CMFCPropertyGridProperty* pLuminaire, double x, double y)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	if (!pMain || !pDoc)
		return;

	int count = pLuminaire->GetSubItemsCount();
	CString strCount;
	strCount.Format(_T("灯具%d"),count);
	
	PropertyGridProperty* pSingleLum = new PropertyGridProperty(strCount, 0, FALSE);

	PropertyGridProperty* pType = new PropertyGridProperty(_T("类型"), _T("FAC21280P-23W"), _T("灯具类型"));
	PropertyGridProperty* pLm = new PropertyGridProperty(_T("光通量(lm)"),(_variant_t)2100, _T("灯具的光通量"));
	PropertyGridProperty* pW = new PropertyGridProperty(_T("功率(w)"), (_variant_t)23, _T("灯具的功率"));
	pType->AllowEdit(FALSE);
	pLm->AllowEdit(FALSE);
	pW->AllowEdit(FALSE);
	vector<LuminaireTem>& lumTems = pDoc->getLuminaireTems();
	for (int i = 0; i < lumTems.size(); i++)
	{
		pType->AddOption(CString(lumTems[i].type));
	}
	PropertyGridProperty* pX = new PropertyGridProperty(_T("X(mm)"), (_variant_t)x, _T("X坐标"));
	PropertyGridProperty* pY = new PropertyGridProperty(_T("Y(mm)"), (_variant_t)y, _T("Y坐标"));
	PropertyGridProperty* pZ = new PropertyGridProperty(_T("Z(mm)"), (_variant_t)750.0, _T("Z坐标"));

	PropertyGridProperty* pNX = new PropertyGridProperty(_T("法矢X"), (_variant_t)0.0, _T("法矢X坐标"));
	PropertyGridProperty* pNY = new PropertyGridProperty(_T("法矢Y"), (_variant_t)0.0, _T("法矢Y坐标"));
	PropertyGridProperty* pNZ = new PropertyGridProperty(_T("法矢Z"), (_variant_t)-1.0, _T("法矢Z坐标"));

	pSingleLum->AddSubItem(pType);
	pSingleLum->AddSubItem(pLm);
	pSingleLum->AddSubItem(pW);
	pSingleLum->AddSubItem(pX);
	pSingleLum->AddSubItem(pY);
	pSingleLum->AddSubItem(pZ);
	pSingleLum->AddSubItem(pNX);
	pSingleLum->AddSubItem(pNY);
	pSingleLum->AddSubItem(pNZ);

	pLuminaire->AddSubItem(pSingleLum);

	m_wndPropList.UpdateProperty((PropertyGridProperty*)pLuminaire);
	m_wndPropList.AdjustLayout();
}

void CRoomWnd::AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, double x, double y)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	if (!pMain || !pDoc)
		return;

	int count = pLuminaire->GetSubItemsCount();
	CString strCount;
	strCount.Format(_T("灯具组%d"),count);

	PropertyGridProperty* pSetLum = new PropertyGridProperty(strCount, ROOM_SET_LUMINAIRE, FALSE);

	PropertyGridProperty* pType = new PropertyGridProperty(_T("类型"), _T("FAC21280P-23W"), _T("灯具类型"));
	PropertyGridProperty* pLm = new PropertyGridProperty(_T("光通量(lm)"),(_variant_t)2100, _T("灯具的光通量"));
	PropertyGridProperty* pW = new PropertyGridProperty(_T("功率(w)"), (_variant_t)23, _T("灯具的功率"));
	pType->AllowEdit(FALSE);
	pLm->AllowEdit(FALSE);
	pW->AllowEdit(FALSE);
	vector<LuminaireTem>& lumTems = pDoc->getLuminaireTems();
	for (int i = 0; i < lumTems.size(); i++)
	{
		pType->AddOption(CString(lumTems[i].type));
	}
	PropertyGridProperty* pZ = new PropertyGridProperty(_T("Z(mm)"), (_variant_t)750.0, _T("Z坐标"));
	PropertyGridProperty* pNX = new PropertyGridProperty(_T("法矢X"), (_variant_t)0.0, _T("法矢X坐标"));
	PropertyGridProperty* pNY = new PropertyGridProperty(_T("法矢Y"), (_variant_t)0.0, _T("法矢Y坐标"));
	PropertyGridProperty* pNZ = new PropertyGridProperty(_T("法矢Z"), (_variant_t)-1.0, _T("法矢Z坐标"));

	PropertyGridProperty* pOriginX = new PropertyGridProperty(_T("初始点X"), (_variant_t)x, _T("初始点X"));
	PropertyGridProperty* pOriginY = new PropertyGridProperty(_T("初始点Y"), (_variant_t)y, _T("初始点Y"));
	PropertyGridProperty* pRowNum = new PropertyGridProperty(_T("行数"), (_variant_t)5, _T("行数"));
	PropertyGridProperty* pColumNum = new PropertyGridProperty(_T("列数"), (_variant_t)5, _T("列数"));
	PropertyGridProperty* pRowLen = new PropertyGridProperty(_T("行宽"), (_variant_t)200.0, _T("行宽"));
	PropertyGridProperty* pColumLen = new PropertyGridProperty(_T("列宽"), (_variant_t)200.0, _T("列宽"));

	PropertyGridProperty* pLumSet = new PropertyGridProperty(_T("坐标"), 0, FALSE);

	pSetLum->AddSubItem(pType);
	pSetLum->AddSubItem(pLm);
	pSetLum->AddSubItem(pW);

	pSetLum->AddSubItem(pZ);
	pSetLum->AddSubItem(pNX);
	pSetLum->AddSubItem(pNY);
	pSetLum->AddSubItem(pNZ);

	pSetLum->AddSubItem(pOriginX);
	pSetLum->AddSubItem(pOriginY);
	pSetLum->AddSubItem(pRowNum);
	pSetLum->AddSubItem(pColumNum);
	pSetLum->AddSubItem(pRowLen);
	pSetLum->AddSubItem(pColumLen);

	pSetLum->AddSubItem(pLumSet);

	pLuminaire->AddSubItem(pSetLum);

	CalLumSet(pSetLum);//计算灯具位置
	m_wndPropList.UpdateProperty((PropertyGridProperty*)pLuminaire);
}
void CRoomWnd::CalLumSet(CMFCPropertyGridProperty* pSetLum)
{
	double originX = pSetLum->GetSubItem(LUM_SET_ORIGIN_X)->GetValue().dblVal;
	double originY = pSetLum->GetSubItem(LUM_SET_ORIGIN_Y)->GetValue().dblVal;
	int rowN = pSetLum->GetSubItem(LUM_SET_ROW_N)->GetValue().intVal;
	int colN = pSetLum->GetSubItem(LUM_SET_COL_N)->GetValue().intVal;
	double rowL = pSetLum->GetSubItem(LUM_SET_ROW_L)->GetValue().dblVal;
	double colL = pSetLum->GetSubItem(LUM_SET_COL_L)->GetValue().dblVal;

	if (pSetLum->GetSubItemsCount() == LUM_SET_POINTS+1)
	{
		CMFCPropertyGridProperty* pLumSet = pSetLum->GetSubItem(LUM_SET_POINTS);
		if (pLumSet)
			pSetLum->RemoveSubItem(pLumSet);
	}

	PropertyGridProperty* pLumSet = new PropertyGridProperty(_T("坐标"), 0, FALSE);
	for (int i = 0; i < rowN; i++)
	{
		for (int j = 0; j < colN; j++)
		{
			CMFCPropertyGridProperty* pPoint = new CMFCPropertyGridProperty(_T("点(mm)"), 0, TRUE);
			CMFCPropertyGridProperty* pPointX = new CMFCPropertyGridProperty(_T("X"), i*colL + originX, _T("X坐标"));
			CMFCPropertyGridProperty* pPointY = new CMFCPropertyGridProperty(_T("Y"), j*rowL + originY, _T("Y坐标"));
			pPoint->AddSubItem(pPointX);
			pPoint->AddSubItem(pPointY);
			pLumSet->AddSubItem(pPoint);
		}
	}
	pSetLum->AddSubItem(pLumSet);

	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pSetLum));
	m_wndPropList.AdjustLayout();
}
void CRoomWnd::CalMidXY(CMFCPropertyGridProperty* pRoom, Vec2d& midP)
{
	if (!pRoom)
		return;
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	CMFCPropertyGridProperty* pOutWallIndexs = pRoom->GetSubItem(ROOM_OUT_WALL);
	CMFCPropertyGridProperty* pInWallIndexs = pRoom->GetSubItem(ROOM_IN_WALL);
	Vec2d s, e, midp(0, 0);
	int count = 0;
	for (int j = 0; j < pOutWallIndexs->GetSubItemsCount(); j++)
	{
		int index = pOutWallIndexs->GetSubItem(j)->GetValue().intVal;
		s.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		s.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
		e.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		e.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
		midp += s;
		midp += e;
		count+=2;
	}
	for (int j = 0; j < pInWallIndexs->GetSubItemsCount(); j++)
	{
		int index = pInWallIndexs->GetSubItem(j)->GetValue().intVal;
		s.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		s.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
		e.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		e.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
		midp += s;
		midp += e;
		count+=2;
	}
	midP = midp / count;
}
void CRoomWnd::CalMinXY(CMFCPropertyGridProperty* pRoom, Vec2d& minP)
{
	if (!pRoom)
	return;
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	CMFCPropertyGridProperty* pOutWallIndexs = pRoom->GetSubItem(ROOM_OUT_WALL);
	CMFCPropertyGridProperty* pInWallIndexs = pRoom->GetSubItem(ROOM_IN_WALL);
	Vec2d s, e, minp(1.0e8, 1.0e8), maxp(-1.0e8, -1.0e8);
	vector<Vec2d> points;
	for (int j = 0; j < pOutWallIndexs->GetSubItemsCount(); j++)
	{
		int index = pOutWallIndexs->GetSubItem(j)->GetValue().intVal;
		s.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		s.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
		e.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		e.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
		s.UpdateMinMax(minp,maxp);
		e.UpdateMinMax(minp,maxp);
		points.push_back(s);
		points.push_back(e);
	}
	for (int j = 0; j < pInWallIndexs->GetSubItemsCount(); j++)
	{
		int index = pInWallIndexs->GetSubItem(j)->GetValue().intVal;
		s.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		s.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
		e.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		e.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
		s.UpdateMinMax(minp,maxp);
		e.UpdateMinMax(minp,maxp);
		points.push_back(s);
		points.push_back(e);
	}
	double minLen = 1.0e9;
	int minI = -1;
	for (int i = 0; i < points.size(); i++)
	{
		if ((points[i]-minp).Length() < minLen)
		{
			minI = i;
			minLen = (points[i]-minp).Length();
		}
	}
	if (minI >= 0)
	{
		minP = points[minI];
	}
}
void CRoomWnd::OnRoomAddLuminaireSingle()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	while (selItem && selItem->GetParent())
	{
		selItem = selItem->GetParent();
	}
	if (selItem)
	{
		Vec2d midP;
		CalMidXY(selItem,midP);
		CMFCPropertyGridProperty* pLuminaire = selItem->GetSubItem(ROOM_SINGLE_LUMINAIRE);
		AddSingleLuminaire(pLuminaire, midP.x, midP.y);
		

		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			return;
		pMain->GetActiveView()->Invalidate(); 
	}
	else
	{
		AfxMessageBox(_T("没有选择任何房间！"));
	}
}


void CRoomWnd::OnRoomAddLuminaireSet()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	while (selItem && selItem->GetParent())
	{
		selItem = selItem->GetParent();
	}
	if (selItem)
	{
		Vec2d minP;
		CalMinXY(selItem,minP);
		CMFCPropertyGridProperty* pLuminaire = selItem->GetSubItem(ROOM_SET_LUMINAIRE);
		AddSetLuminaire(pLuminaire, minP.x, minP.y);
		m_wndPropList.UpdateProperty((PropertyGridProperty*)pLuminaire);

		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			return;
		pMain->GetActiveView()->Invalidate(); 
	}
	else
	{
		AfxMessageBox(_T("没有选择任何房间！"));
	}
}


void CRoomWnd::OnRoomAddControlSet()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	while (selItem && selItem->GetParent())
	{
		selItem = selItem->GetParent();
	}
	if (selItem)
	{
		Vec2d minP;
		CalMinXY(selItem,minP);
		CMFCPropertyGridProperty* pControlSet = selItem->GetSubItem(ROOM_CONTROL_SET);
		AddSetLuminaire(pLuminaire, minP.x, minP.y);
		m_wndPropList.UpdateProperty((PropertyGridProperty*)pLuminaire);

		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			return;
		pMain->GetActiveView()->Invalidate(); 
	}
	else
	{
		AfxMessageBox(_T("没有选择任何房间！"));
	}
}
