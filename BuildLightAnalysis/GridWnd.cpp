
#include "stdafx.h"

#include "GridWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "BuildLightAnalysis.h"
#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"

#include "MathUtility.h"
#include "Serializer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CGridWnd::CGridWnd()
{
}

CGridWnd::~CGridWnd()
{
}

BEGIN_MESSAGE_MAP(CGridWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)

	ON_COMMAND(IDC_GRID_INSERT_BUTTON, OnNewGrid)
	ON_UPDATE_COMMAND_UI(IDC_GRID_INSERT_BUTTON, OnUpdateProperties1)
	ON_COMMAND(IDC_GRID_DELETE_BUTTON, OnDeleteGrid)
	ON_UPDATE_COMMAND_UI(IDC_GRID_DELETE_BUTTON, OnUpdateProperties1)

	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CGridWnd::AdjustLayout()
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

int CGridWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	if (!m_insertButton.Create(_T("新建计算点"),dwViewStyle, rectDummy, this, IDC_GRID_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("删除计算点"),dwViewStyle, rectDummy, this, IDC_GRID_DELETE_BUTTON))
	{
		return -1;      // fail to create
	}


	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_GRID_CTRL))
	{
		TRACE0("Failed to create Grid \n");
		return -1;      // fail to create
	}



	InitPropList();

	AdjustLayout();
	return 0;
}

void CGridWnd::OnNewGrid()
{
	InsertGrid(-1);
}
void CGridWnd::OnDeleteGrid()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem && !selItem->GetParent())
	{
		m_wndPropList.DeleteProperty(selItem);

		//重新设置一下坐标编号
		int count = m_wndPropList.GetPropertyCount();
		CString strName;
		for (int i = 0; i < count; i++)
		{
			strName.Format(_T("计算点%d"),i);
			m_wndPropList.GetProperty(i)->SetName(strName);
		}
	}
	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}

void CGridWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CGridWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CGridWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CGridWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CGridWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CGridWnd::OnUpdateProperties1(CCmdUI* pCmdUI)
{

}

void CGridWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void CGridWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CGridWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CGridWnd::SetPropListFont()
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
void CGridWnd::InsertGrid(int roomIndex, double offset, double meshLen)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if (!pMain)
		return;

	int count = m_wndPropList.GetPropertyCount();
	CString strCount;
	strCount.Format(_T("计算点%d"),count);
	

	PropertyGridProperty* pGrid = new PropertyGridProperty(strCount, 1000, TRUE);

	_variant_t var;
	var.vt = VT_INT;var = roomIndex;
	PropertyGridProperty* pRoomIndex = new PropertyGridProperty(_T("房间号"), var, _T("计算点的房间号"));
	
	PropertyGridCtrl* roomList = pMain->GetRoomProperty().getPropList();
	CString strRoomIndex;
	for (int i = 0; i < roomList->GetPropertyCount(); i++)
	{
		strRoomIndex.Format(_T("%d"),i);
		pRoomIndex->AddOption(strRoomIndex);
	}
	pRoomIndex->AllowEdit(FALSE);
	
	PropertyGridProperty* pOffset = new PropertyGridProperty(_T("内偏移"), (_variant_t)offset, _T("内偏移"));
	PropertyGridProperty* pMeshLen = new PropertyGridProperty(_T("网格边长"), (_variant_t)meshLen, _T("网格边长"));
	
	pGrid->AddSubItem(pRoomIndex);
	pGrid->AddSubItem(pOffset);
	pGrid->AddSubItem(pMeshLen);

	CalGrid(pGrid);

	m_wndPropList.AddProperty(pGrid);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
	m_wndPropList.AdjustLayout();

	//更新视图     
	pMain->GetActiveView()->Invalidate(); 
}

void CGridWnd::CalGrid(CMFCPropertyGridProperty* pGrid)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	if (pGrid->GetSubItemsCount() >= 4)
	{
		CMFCPropertyGridProperty* pl = pGrid->GetSubItem(3);
		if (pl)
			pGrid->RemoveSubItem(pl);
	}
	

	int roomIndex = pGrid->GetSubItem(0)->GetValue().intVal;
	int offset = pGrid->GetSubItem(1)->GetValue().dblVal;
	int meshLen = pGrid->GetSubItem(2)->GetValue().dblVal;

	if (roomIndex == -1)
		return;

	CMFCPropertyGridProperty* pGridList = new CMFCPropertyGridProperty(_T("计算点"),0,TRUE);

	PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	//导出第i个房间
	CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(roomIndex);
	Wall wall;
	list<Wall> roomWalls;
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
	}
	vector<Vec2d> outPolygon;
	vector<Wall> outWalls;
	if (!CalClosedPolygon(roomWalls, outWalls, outPolygon))
	{
		return;
	}
	vector<Vec2d> gridPoints;
	CalGridFromPolygon(outPolygon,offset,meshLen,gridPoints);

	CString strGridPointIndex;
	for (int i = 0; i < gridPoints.size(); i++)
	{
		strGridPointIndex.Format(_T("%d"),i);
		CMFCPropertyGridProperty* pGridPoint = new CMFCPropertyGridProperty(_T("点"),0,TRUE);
		CMFCPropertyGridProperty* pGridPointX = new CMFCPropertyGridProperty(_T("X"),(_variant_t)gridPoints[i].x,_T("X"));
		CMFCPropertyGridProperty* pGridPointY = new CMFCPropertyGridProperty(_T("Y"),(_variant_t)gridPoints[i].y,_T("Y"));
		pGridPoint->AddSubItem(pGridPointX);
		pGridPoint->AddSubItem(pGridPointY);

		pGridList->AddSubItem(pGridPoint);
	}
	pGrid->AddSubItem(pGridList);

	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pGrid));
}

void CGridWnd::DeleteAllGrid()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.AdjustLayout();
}

LRESULT CGridWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

	if (pProp->GetParent() && pProp->GetParent()->GetData() == 1000)
	{
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (!pMain)
			0;
		CMFCPropertyGridProperty* grid = pProp->GetParent();
		CalGrid(grid);
		m_wndPropList.AdjustLayout();

		//更新视图     
		pMain->GetActiveView()->Invalidate(); 
	}
	return 0;
}

void CGridWnd::save(ofstream& out)
{
	vector<Grid> grids;

	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		Grid g;
		CMFCPropertyGridProperty* pG = m_wndPropList.GetProperty(i);
		g.roomIndex = pG->GetSubItem(0)->GetValue().intVal;
		g.offset = pG->GetSubItem(1)->GetValue().dblVal;
		g.meshLen = pG->GetSubItem(2)->GetValue().dblVal;
		if (pG->GetSubItem())
		{
		}
	}
	/*vector<stWindow> windows;
	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		stWindow win;
		CMFCPropertyGridProperty* pWin = m_wndPropList.GetProperty(i);
		CString wallType = pWin->GetSubItem(0)->GetValue().bstrVal;
		 _tcscpy(win.wallType, wallType);
		win.wallIndex = pWin->GetSubItem(1)->GetValue().intVal;
		win.pos = pWin->GetSubItem(2)->GetValue().dblVal;
		win.WinUpHeight = pWin->GetSubItem(3)->GetValue().dblVal;
		win.WinDownHeight = pWin->GetSubItem(4)->GetValue().dblVal;
		win.WinWidth = pWin->GetSubItem(5)->GetValue().dblVal;
		CString mat = pWin->GetSubItem(6)->GetValue().bstrVal;
		_tcscpy(win.WinMaterial, mat);

		windows.push_back(win);
	}
	serializer<stWindow>::write(out,&windows);*/
}
void CGridWnd::load(ifstream& in)
{
	/*OnDeleteGrid();
	vector<stWindow> windows;
	serializer<stWindow>::read(in, &windows);
	for (int i = 0; i < windows.size(); i++)
	{
		CString type = windows[i].wallType;
		if (type == _T("外墙"))
		{
			InsertGrid(windows[i].wallIndex, -1,-1, windows[i].pos, windows[i].WinUpHeight, windows[i].WinDownHeight,
				windows[i].WinWidth, CString(windows[i].WinMaterial));
		}
		else if (type == _T("内墙"))
		{
			InsertGrid(-1,windows[i].wallIndex,-1, windows[i].pos, windows[i].WinUpHeight, windows[i].WinDownHeight,
				windows[i].WinWidth, CString(windows[i].WinMaterial));
		}
		
	}*/
}