
#include "stdafx.h"

#include "WindowWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "BuildLightAnalysis.h"
#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"

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
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
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

	int cyBut = 0;//rectButton.Size().cy;
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyBut, rectClient.Width(), rectClient.Height() -(cyBut), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CWindowWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_WINDOW_CTRL))
	{
		TRACE0("Failed to create Option Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();
	return 0;
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
}
void CWindowWnd::InsertWindow(int outWallIndex, int inWallIndex)
{
	int count = m_wndPropList.GetPropertyCount();
	CString strCount;
	strCount.Format(_T("窗%d"),count+1);

	CString strWallIndex;
	if (outWallIndex >= 0)
	{
		strWallIndex.Format(_T("外墙%d"),outWallIndex);
	}
	if (inWallIndex >= 0)
	{
		strWallIndex.Format(_T("内墙%d"),inWallIndex);
	}

	PropertyGridProperty* pWindow = new PropertyGridProperty(strCount, 0, TRUE);

	PropertyGridProperty* pWallIndex = new PropertyGridProperty(_T("墙号"), (_variant_t) strWallIndex, _T("窗户所在的墙号"));
	PropertyGridProperty* pPos = new PropertyGridProperty(_T("位置"), (_variant_t) 0.5, _T("窗台中心与墙角的距离占墙的而距离比值"));
	PropertyGridProperty* pWinUpHeight = new PropertyGridProperty(_T("窗上高"), (_variant_t) 2.8, _T("内墙坐标X值"));
	PropertyGridProperty* pWinDownHeight = new PropertyGridProperty(_T("窗下高"), (_variant_t) 3.8, _T("内墙坐标X值"));
	PropertyGridProperty* pWinWidth = new PropertyGridProperty(_T("窗宽"), (_variant_t) 2, _T("内墙坐标X值"));
	
	pWindow->AddSubItem(pWallIndex);
	
	pWindow->AddSubItem(pPos);
	pWindow->AddSubItem(pWinUpHeight);
	pWindow->AddSubItem(pWinDownHeight);
	pWindow->AddSubItem(pWinWidth);

	m_wndPropList.AddProperty(pWindow);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
	m_wndPropList.AdjustLayout();

	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}
void CWindowWnd::save(ofstream& out)
{
	/*double d1 = m_wndPropList.GetProperty(0)->GetValue().dblVal;
	double d2 = m_wndPropList.GetProperty(1)->GetValue().dblVal;
	serializer<double>::write(out,&d1);
	serializer<double>::write(out,&d2);*/
}
void CWindowWnd::load(ifstream& in)
{
	/*double d1;
	double d2;
	serializer<double>::read(in,&d1);
	serializer<double>::read(in,&d2);
	m_wndPropList.GetProperty(0)->SetValue(d1);
	m_wndPropList.GetProperty(1)->SetValue(d2);*/
}