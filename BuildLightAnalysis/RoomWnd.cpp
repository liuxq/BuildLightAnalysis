
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

	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)

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

void CRoomWnd::OnNewRoom()
{
	int count = m_wndPropList.GetPropertyCount();
	CString strCount;
	strCount.Format(_T("房间%d"),count);

	PropertyGridProperty* pRoom = new PropertyGridProperty(strCount, 0, FALSE);
	PropertyGridProperty* pOutWall = new PropertyGridProperty(_T("外墙"), 0, TRUE);
	PropertyGridProperty* pInWall = new PropertyGridProperty(_T("内墙"), 0, TRUE);
	PropertyGridProperty* pWin = new PropertyGridProperty(_T("窗户"), ROOM_WINDOW, TRUE);
	PropertyGridProperty* pGrid = new PropertyGridProperty(_T("计算点"), ROOM_GRID, TRUE);
	
	PropertyGridProperty* pOffset = new PropertyGridProperty(_T("内偏移"), (_variant_t)120.0, _T("内偏移"));
	PropertyGridProperty* pMeshLen = new PropertyGridProperty(_T("网格边长"), (_variant_t)120.0, _T("网格边长"));
	pGrid->AddSubItem(pOffset);
	pGrid->AddSubItem(pMeshLen);

	pRoom->AddSubItem(pOutWall);
	pRoom->AddSubItem(pInWall);
	pRoom->AddSubItem(pWin);
	pRoom->AddSubItem(pGrid);

	m_wndPropList.AddProperty(pRoom);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
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

	CMFCPropertyGridProperty* pGridList = new CMFCPropertyGridProperty(_T("计算点"),0,TRUE);

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

void CRoomWnd::save(ofstream& out)
{
	vector<vector<WallIndex> > rooms;
	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		vector<WallIndex> room;
		CMFCPropertyGridProperty* pRoom = m_wndPropList.GetProperty(i);
		for (int j = 0; j < pRoom->GetSubItemsCount(); j++)
		{
			CString name = pRoom->GetSubItem(j)->GetName();
			if (name == _T("外墙号"))
			{
				room.push_back(WallIndex(1,pRoom->GetSubItem(j)->GetValue().intVal));
			}
			if (name == _T("内墙号"))
			{
				room.push_back(WallIndex(2,pRoom->GetSubItem(j)->GetValue().intVal));
			}
			if (name == _T("窗户号"))
			{
				room.push_back(WallIndex(3,pRoom->GetSubItem(j)->GetValue().intVal));
			}
		}
		rooms.push_back(room);
	}

	int size = rooms.size();
	out.write((char *)&size, sizeof(size));
	for (int i = 0; i < size; i++)
	{
		serializer<WallIndex>::write(out, &rooms[i]);
	}
	
}
void CRoomWnd::load(ifstream& in)
{
	DeleteAllRoom();

	int size = 0;
	in.read((char *)&size, sizeof(size));
	vector<vector<WallIndex> > rooms(size);
	for (int i = 0; i < size; i++)
	{
		serializer<WallIndex>::read(in, &rooms[i]);
	}
	for (int i = 0; i < size; i++)
	{
		CString strCount;
		strCount.Format(_T("房间%d"),i);
		PropertyGridProperty* pRoom = new PropertyGridProperty(strCount, 0, TRUE);
		
		
		
		for (int j = 0; j < rooms[i].size(); j++)
		{
			if(rooms[i][j].type == 1)
			{
				PropertyGridProperty* pItem = new PropertyGridProperty(_T("外墙号"), (_variant_t)rooms[i][j].index, _T("房间内的项目"));
				pRoom->AddSubItem(pItem);
			}
			else if(rooms[i][j].type == 2)
			{
				PropertyGridProperty* pItem = new PropertyGridProperty(_T("内墙号"), (_variant_t)rooms[i][j].index, _T("房间内的项目"));
				pRoom->AddSubItem(pItem);
			}
			else if(rooms[i][j].type == 3)
			{
				PropertyGridProperty* pItem = new PropertyGridProperty(_T("窗户号"), (_variant_t)rooms[i][j].index, _T("房间内的项目"));
				pRoom->AddSubItem(pItem);
			}
		}

		m_wndPropList.AddProperty(pRoom);

		m_wndPropList.AdjustLayout();
		
	}

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
	while (selItem->GetParent())
	{
		selItem = selItem->GetParent();
	}
	if (selItem)
	{
		CMFCPropertyGridProperty* pGrid = selItem->GetSubItem(ROOM_GRID);
		CalGrid(pGrid);
	}
}
