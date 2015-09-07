
#include "stdafx.h"

#include "WindowWnd.h"
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

CWindowWnd::CWindowWnd()
{
}

CWindowWnd::~CWindowWnd()
{
}

BEGIN_MESSAGE_MAP(CWindowWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)

	ON_COMMAND(IDC_WINDOW_INSERT_BUTTON, OnNewWindow)
	ON_UPDATE_COMMAND_UI(IDC_WINDOW_INSERT_BUTTON, OnUpdateProperties1)
	ON_COMMAND(IDC_WINDOW_DELETE_BUTTON, OnDeleteWindow)
	ON_UPDATE_COMMAND_UI(IDC_WINDOW_DELETE_BUTTON, OnUpdateProperties1)

	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CWindowWnd::AdjustLayout()
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

int CWindowWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	if (!m_insertButton.Create(_T("新建窗户"),dwViewStyle, rectDummy, this, IDC_WINDOW_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("删除窗户"),dwViewStyle, rectDummy, this, IDC_WINDOW_DELETE_BUTTON))
	{
		return -1;      // fail to create
	}


	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_WINDOW_CTRL))
	{
		TRACE0("Failed to create Option Grid \n");
		return -1;      // fail to create
	}



	InitPropList();

	AdjustLayout();
	return 0;
}

void CWindowWnd::OnNewWindow()
{
	InsertWindow(0, 0, -1);
}
void CWindowWnd::OnDeleteWindow()
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
			strName.Format(_T("窗%d"),i);
			m_wndPropList.GetProperty(i)->SetName(strName);
		}
	}
	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}

void CWindowWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CWindowWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CWindowWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CWindowWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CWindowWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CWindowWnd::OnUpdateProperties1(CCmdUI* pCmdUI)
{

}

void CWindowWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void CWindowWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CWindowWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CWindowWnd::SetPropListFont()
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
void CWindowWnd::InsertWindow(int outWallIndex, int inWallIndex, int windowIndex, double pos ,double WinUpHeight,double WinDownHeight,
double WinWidth, CString mat)
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	if (!pMain || !pDoc)
		return;

	int count = m_wndPropList.GetPropertyCount();
	CString strCount;
	strCount.Format(_T("窗%d"),count);

	if (windowIndex >= 0)
	{
		CString t = m_wndPropList.GetProperty(windowIndex)->GetSubItem(0)->GetValue().bstrVal;
		int _index = m_wndPropList.GetProperty(windowIndex)->GetSubItem(1)->GetValue().intVal;
		if (t == _T("外墙"))
		{
			outWallIndex = _index;
		}
		if (t == _T("内墙"))
		{
			inWallIndex = _index;
		}
	}

	CString strWallType;
	int wallIndex;
	if (outWallIndex >= 0)
	{
		strWallType = _T("外墙");
		wallIndex = outWallIndex;
		count = pMain->GetOptimizeWallProperty().getCoodOutWallGroup()->GetSubItemsCount();
	}
	if (inWallIndex >= 0)
	{
		strWallType = _T("内墙");
		wallIndex = inWallIndex;
		count = pMain->GetOptimizeWallProperty().getCoodInWallGroup()->GetSubItemsCount();
	}
	

	PropertyGridProperty* pWindow = new PropertyGridProperty(strCount, 0, TRUE);

	PropertyGridProperty* pWallType = new PropertyGridProperty(_T("墙类型"), strWallType, _T("窗户所在的墙的类型"),1000);
	pWallType->AddOption(_T("外墙"));
	pWallType->AddOption(_T("内墙"));
	pWallType->AllowEdit(FALSE);
	_variant_t var;
	var.vt = VT_INT;var = wallIndex;
	PropertyGridProperty* pWallIndex = new PropertyGridProperty(_T("墙号"), var, _T("窗户所在的墙的编号, -1表示该类型墙的数量为0"),1001);
	
	CString strItem;
	for (int i = 0; i < count; i++)
	{
		strItem.Format(_T("%d"), i);
		pWallIndex->AddOption(strItem);
	}
	pWallIndex->AddOption(_T("1"));
	pWallIndex->AddOption(_T("2"));
	pWallIndex->AllowEdit(FALSE);
	PropertyGridProperty* pPos = new PropertyGridProperty(_T("位置"), (_variant_t)pos, _T("窗台中心与墙角的距离占墙的而距离比值"));
	PropertyGridProperty* pWinUpHeight = new PropertyGridProperty(_T("窗上高"), (_variant_t)WinUpHeight, _T("窗户上高"));
	PropertyGridProperty* pWinDownHeight = new PropertyGridProperty(_T("窗下高"), (_variant_t)WinDownHeight, _T("窗户下高"));
	PropertyGridProperty* pWinWidth = new PropertyGridProperty(_T("窗宽"), (_variant_t)WinWidth, _T("窗户宽度"));

	PropertyGridProperty* pWinMaterial = new PropertyGridProperty(_T("窗材质"), mat, _T("窗户的材质"));
	vector<Material>& mats = pDoc->getMaterials();
	for (int i = 0; i < mats.size(); i++)
	{
		pWinMaterial->AddOption(stringToCString(mats[i].name));
	}
	
	pWindow->AddSubItem(pWallType);
	pWindow->AddSubItem(pWallIndex);
	pWindow->AddSubItem(pPos);
	pWindow->AddSubItem(pWinUpHeight);
	pWindow->AddSubItem(pWinDownHeight);
	pWindow->AddSubItem(pWinWidth);
	pWindow->AddSubItem(pWinMaterial);

	m_wndPropList.AddProperty(pWindow);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
	m_wndPropList.AdjustLayout();

	//更新视图     
	pMain->GetActiveView()->Invalidate(); 
}

void CWindowWnd::DeleteAllWindow()
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

LRESULT CWindowWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
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

void CWindowWnd::save(ofstream& out)
{
	vector<stWindow> windows;
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
	serializer<stWindow>::write(out,&windows);
}
void CWindowWnd::load(ifstream& in)
{
	OnDeleteWindow();
	vector<stWindow> windows;
	serializer<stWindow>::read(in, &windows);
	for (int i = 0; i < windows.size(); i++)
	{
		CString type = windows[i].wallType;
		if (type == _T("外墙"))
		{
			InsertWindow(windows[i].wallIndex, -1,-1 windows[i].pos, windows[i].WinUpHeight, windows[i].WinDownHeight,
				windows[i].WinWidth, CString(windows[i].WinMaterial));
		}
		else if (type == _T("内墙"))
		{
			InsertWindow(-1,windows[i].wallIndex,-1, windows[i].pos, windows[i].WinUpHeight, windows[i].WinDownHeight,
				windows[i].WinWidth, CString(windows[i].WinMaterial));
		}
		
	}
}