
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
	if (!m_insertButton.Create(_T("�½������"),dwViewStyle, rectDummy, this, IDC_GRID_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("ɾ�������"),dwViewStyle, rectDummy, this, IDC_GRID_DELETE_BUTTON))
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
	CMFCPropertyGridProperty* pgrid = InsertGrid(-1);
	CalGrid(pgrid);
}
void CGridWnd::OnDeleteGrid()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem && !selItem->GetParent())
	{
		m_wndPropList.DeleteProperty(selItem);

		//��������һ��������
		int count = m_wndPropList.GetPropertyCount();
		CString strName;
		for (int i = 0; i < count; i++)
		{
			strName.Format(_T("�����%d"),i);
			m_wndPropList.GetProperty(i)->SetName(strName);
		}
	}
	//������ͼ
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
CMFCPropertyGridProperty* CGridWnd::InsertGrid(int roomIndex, double offset, double meshLen)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if (!pMain)
		return NULL;

	int count = m_wndPropList.GetPropertyCount();
	CString strCount;
	strCount.Format(_T("�����%d"),count);
	

	PropertyGridProperty* pGrid = new PropertyGridProperty(strCount, 1000, TRUE);

	_variant_t var;
	var.vt = VT_INT;var = roomIndex;
	PropertyGridProperty* pRoomIndex = new PropertyGridProperty(_T("�����"), var, _T("�����ķ����"));
	
	PropertyGridCtrl* roomList = pMain->GetRoomProperty().getPropList();
	CString strRoomIndex;
	for (int i = 0; i < roomList->GetPropertyCount(); i++)
	{
		strRoomIndex.Format(_T("%d"),i);
		pRoomIndex->AddOption(strRoomIndex);
	}
	pRoomIndex->AllowEdit(FALSE);
	
	PropertyGridProperty* pOffset = new PropertyGridProperty(_T("��ƫ��"), (_variant_t)offset, _T("��ƫ��"));
	PropertyGridProperty* pMeshLen = new PropertyGridProperty(_T("����߳�"), (_variant_t)meshLen, _T("����߳�"));
	
	pGrid->AddSubItem(pRoomIndex);
	pGrid->AddSubItem(pOffset);
	pGrid->AddSubItem(pMeshLen);

	m_wndPropList.AddProperty(pGrid);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
	m_wndPropList.AdjustLayout();

	return pGrid;
}
void CGridWnd::LoadGrid(CMFCPropertyGridProperty* pGrid, vector<GridPoint>& points)
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

	CMFCPropertyGridProperty* pGridList = new CMFCPropertyGridProperty(_T("�����"),0,TRUE);

	CString strp;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].isKey)
			strp = _T("�ؼ���");
		else
			strp = _T("��");
		CMFCPropertyGridProperty* pGridPoint = new CMFCPropertyGridProperty(strp,0,TRUE);
		CMFCPropertyGridProperty* pGridPointX = new CMFCPropertyGridProperty(_T("X"),(_variant_t)points[i].p.x,_T("X"));
		CMFCPropertyGridProperty* pGridPointY = new CMFCPropertyGridProperty(_T("Y"),(_variant_t)points[i].p.y,_T("Y"));
		pGridPoint->AddSubItem(pGridPointX);
		pGridPoint->AddSubItem(pGridPointY);

		pGridList->AddSubItem(pGridPoint);
	}
	pGrid->AddSubItem(pGridList);

	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pGrid));

}
void CGridWnd::CalGrid(CMFCPropertyGridProperty* pGrid)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain || !pGrid)
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

	CMFCPropertyGridProperty* pGridList = new CMFCPropertyGridProperty(_T("�����"),0,TRUE);

	PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	//������i������
	CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(roomIndex);
	Wall wall;
	list<Wall> roomWalls;
	for (int j = 0; j < pRoom->GetSubItemsCount(); j++)
	{
		CString wallType = pRoom->GetSubItem(j)->GetName();
		if (wallType == _T("��ǽ��"))
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
		else if (wallType == _T("��ǽ��"))
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

	for (int i = 0; i < gridPoints.size(); i++)
	{
		CMFCPropertyGridProperty* pGridPoint = new CMFCPropertyGridProperty(_T("��"),0,TRUE);
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

		//������ͼ     
		pMain->GetActiveView()->Invalidate(); 
	}
	return 0;
}
void CGridWnd::OutputToGrids(vector<Grid>& grids)
{
	for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		Grid g;
		CMFCPropertyGridProperty* pG = m_wndPropList.GetProperty(i);
		g.roomIndex = pG->GetSubItem(0)->GetValue().intVal;
		g.offset = pG->GetSubItem(1)->GetValue().dblVal;
		g.meshLen = pG->GetSubItem(2)->GetValue().dblVal;
		if (pG->GetSubItemsCount() < 4)
			continue;

		CMFCPropertyGridProperty* pList = pG->GetSubItem(3);
		for (int j = 0; j < pList->GetSubItemsCount(); j++)
		{
			GridPoint p;
			CString _name = pList->GetSubItem(j)->GetName();
			if (_name == _T("�ؼ���"))
				p.isKey = true;
			else
				p.isKey = false;
			p.p.x = pList->GetSubItem(j)->GetSubItem(0)->GetValue().dblVal;
			p.p.y = pList->GetSubItem(j)->GetSubItem(1)->GetValue().dblVal;
			g.points.push_back(p);
		}
		grids.push_back(g);
	}
}
void CGridWnd::inputFromGrids(vector<Grid>& sGrids)
{

}
void CGridWnd::save(ofstream& out)
{
	vector<Grid> grids;
	OutputToGrids(grids);
	int sz = grids.size();
	serializer<int>::write(out, &sz);
	for (int i = 0; i < sz; i++)
	{
		serializer<int>::write(out, &grids[i].roomIndex);
		serializer<double>::write(out, &grids[i].offset);
		serializer<double>::write(out, &grids[i].meshLen);

		int gsz = grids[i].points.size();
		serializer<GridPoint>::write(out, &grids[i].points);
	}
}
void CGridWnd::load(ifstream& in)
{
	DeleteAllGrid();
	int sz = 0;
	serializer<int>::read(in, &sz);
	for (int i = 0; i < sz; i++)
	{
		int roomIndex = -1;
		double offset = 0;
		double meshLen = 0;
		serializer<int>::read(in, &roomIndex);
		serializer<double>::read(in, &offset);
		serializer<double>::read(in, &meshLen);
		vector<GridPoint> points;
		serializer<GridPoint>::read(in, &points);
		
		LoadGrid(InsertGrid(roomIndex, offset, meshLen), points);
	}
}