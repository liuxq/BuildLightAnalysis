
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
	

	PropertyGridProperty* pGrid = new PropertyGridProperty(strCount, 0, TRUE);

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

	m_wndPropList.AddProperty(pGrid);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
	m_wndPropList.AdjustLayout();

	//更新视图     
	pMain->GetActiveView()->Invalidate(); 
}

void CGridWnd::DeleteAllWindow()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.AdjustLayout();
	/*for (int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty* subItem = m_wndPropList.GetProperty(i);
		m_wndPropList.DeleteProperty(subItem);
		i--;
	}*/
}

LRESULT CGridWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

	if (pProp->GetData() == 1000)
	{
		CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
		
		CString d;
		d = pProp->GetValue().bstrVal;
		if (d == _T("外墙"))
		{
			CMFCPropertyGridProperty* pIndex = pProp->GetParent()->GetSubItem(1);
			pIndex->RemoveAllOptions();

			int count = pMain->GetOptimizeWallProperty().getCoodOutWallGroup()->GetSubItemsCount();
			if (count == 0)
			{
				pIndex->SetValue((_variant_t)(-1));
			}
			CString strItem;
			for (int i = 0; i < count; i++)
			{
				strItem.Format(_T("%d"), i);
				pIndex->AddOption(strItem);
			}
			
		}
		else if (d == _T("内墙"))
		{
			CMFCPropertyGridProperty* pIndex = pProp->GetParent()->GetSubItem(1);
			pIndex->RemoveAllOptions();

			int count = pMain->GetOptimizeWallProperty().getCoodInWallGroup()->GetSubItemsCount();
			if (count == 0)
			{
				pIndex->SetValue((_variant_t)(-1));
			}
			CString strItem;
			for (int i = 0; i < count; i++)
			{
				strItem.Format(_T("%d"), i);
				pIndex->AddOption(strItem);
			}
		}
	}
	return 0;
}

void CGridWnd::save(ofstream& out)
{
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