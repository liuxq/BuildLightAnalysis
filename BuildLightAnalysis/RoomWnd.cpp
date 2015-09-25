
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

	PropertyGridProperty* pRoom = new PropertyGridProperty(roomName, 0, FALSE);

	PropertyGridProperty* pType = new PropertyGridProperty(_T("类型"), _T("普通办公室"), _T("房间类型"));
	vector<RoomType>& roomTypes = pDoc->getRoomTypes();
	for (int i = 0; i < roomTypes.size(); i++)
	{
		pType->AddOption(CString(roomTypes[i].name));
	}
	PropertyGridProperty* pOutWall = new PropertyGridProperty(_T("外墙"), 0, TRUE);
	PropertyGridProperty* pInWall = new PropertyGridProperty(_T("内墙"), 0, TRUE);
	PropertyGridProperty* pWin = new PropertyGridProperty(_T("窗户"), ROOM_WINDOW, TRUE);
	PropertyGridProperty* pGrid = new PropertyGridProperty(_T("计算网格"), ROOM_GRID, FALSE);

	PropertyGridProperty* pOffset = new PropertyGridProperty(_T("内偏移"), (_variant_t)120.0, _T("内偏移"));
	PropertyGridProperty* pMeshLen = new PropertyGridProperty(_T("网格边长"), (_variant_t)120.0, _T("网格边长"));
	pGrid->AddSubItem(pOffset);
	pGrid->AddSubItem(pMeshLen);

	pRoom->AddSubItem(pType);
	pRoom->AddSubItem(pOutWall);
	pRoom->AddSubItem(pInWall);
	pRoom->AddSubItem(pWin);
	pRoom->AddSubItem(pGrid);

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
		CMFCPropertyGridProperty* pGridPoint = new CMFCPropertyGridProperty(_T("点"),0,TRUE);
		CMFCPropertyGridProperty* pGridPointX = new CMFCPropertyGridProperty(_T("X"),(_variant_t)gridPoints[i].x,_T("X"));
		CMFCPropertyGridProperty* pGridPointY = new CMFCPropertyGridProperty(_T("Y"),(_variant_t)gridPoints[i].y,_T("Y"));
		pGridPoint->AddSubItem(pGridPointX);
		pGridPoint->AddSubItem(pGridPointY);

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
		((PropertyGridProperty*)selItem)->RemoveAllSubItem();
		m_wndPropList.DeleteProperty(selItem);

		//重新设置一下坐标编号
		int count = m_wndPropList.GetPropertyCount();
		CString strName;
		for (int i = 0; i < count; i++)
		{
			strName.Format(_T("房间%d"),i);
			m_wndPropList.GetProperty(i)->SetName(strName);
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
		CMFCPropertyGridProperty* pOutWall = pRoom->GetSubItem(ROOM_OUT_WALL);
		CMFCPropertyGridProperty* pInWall = pRoom->GetSubItem(ROOM_IN_WALL);
		CMFCPropertyGridProperty* pWindow = pRoom->GetSubItem(ROOM_WINDOW);
		CString type = pType->GetValue().bstrVal;
		_tcscpy_s(room.type.name, type);
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
				if (_name == _T("关键点"))
					p.isKey = true;
				else
					p.isKey = false;
				p.p.x = pPoints->GetSubItem(j)->GetSubItem(0)->GetValue().dblVal;
				p.p.y = pPoints->GetSubItem(j)->GetSubItem(1)->GetValue().dblVal;
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
		CMFCPropertyGridProperty* pOutWall = pRoom->GetSubItem(ROOM_OUT_WALL);
		CMFCPropertyGridProperty* pInWall = pRoom->GetSubItem(ROOM_IN_WALL);
		CMFCPropertyGridProperty* pWindow = pRoom->GetSubItem(ROOM_WINDOW);
		CMFCPropertyGridProperty* pGrid = pRoom->GetSubItem(ROOM_GRID);
		
		pType->SetValue(rooms[i].type.name);
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
			CMFCPropertyGridProperty* point = new CMFCPropertyGridProperty(gp.isKey?_T("关键点"):_T("点"), 0, TRUE);
			CMFCPropertyGridProperty* pGridPointX = new CMFCPropertyGridProperty(_T("X"),(_variant_t)gp.p.x,_T("X"));
			CMFCPropertyGridProperty* pGridPointY = new CMFCPropertyGridProperty(_T("Y"),(_variant_t)gp.p.y,_T("Y"));
			point->AddSubItem(pGridPointX);
			point->AddSubItem(pGridPointY);
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
