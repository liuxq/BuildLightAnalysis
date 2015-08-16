
#include "stdafx.h"

#include "OutWallWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "BuildLightAnalysis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const int waiqiangID = 100;
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

COutWallWnd::COutWallWnd()
{
}

COutWallWnd::~COutWallWnd()
{
}

BEGIN_MESSAGE_MAP(COutWallWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(IDC_INSERT_BUTTON, OnInsertPos)
	ON_UPDATE_COMMAND_UI(IDC_INSERT_BUTTON, OnUpdateProperties2)
	ON_COMMAND(IDC_DELETE_BUTTON, OnDeletePos)
	ON_UPDATE_COMMAND_UI(IDC_DELETE_BUTTON, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void COutWallWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectButton;
	GetClientRect(rectClient);

	m_insertButton.GetWindowRect(&rectButton);

	int cyBut = 20;//rectButton.Size().cy;

	m_insertButton.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width()/2, cyBut, SWP_NOZORDER);
	m_deleteButton.SetWindowPos(NULL, rectClient.left + rectClient.Width()/2, rectClient.top, rectClient.Width()/2, cyBut, SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyBut, rectClient.Width(), rectClient.Height() -(cyBut), SWP_NOACTIVATE | SWP_NOZORDER);
}

int COutWallWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	if (!m_insertButton.Create(_T("插入坐标"),dwViewStyle, rectDummy, this, IDC_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("删除坐标"),dwViewStyle, rectDummy, this, IDC_DELETE_BUTTON))
	{
		return -1;      // fail to create
	}

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();
	return 0;
}

void COutWallWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void COutWallWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void COutWallWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void COutWallWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void COutWallWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void COutWallWnd::OnInsertPos()
{
	CMFCPropertyGridProperty* pGroup = getCoodGroup();
	int count = pGroup->GetSubItemsCount();
	CString strCount;
	strCount.Format(_T("%d"),count+1);

	CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(strCount, 0, TRUE);

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pPos->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Y"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pPos->AddSubItem(pProp);

	pGroup->AddSubItem(pPos);
	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pGroup));
	m_wndPropList.AdjustLayout();
}

void COutWallWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void COutWallWnd::OnDeletePos()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem && selItem->GetParent() && selItem->GetParent()->GetData() == waiqiangID)
	{
		m_wndPropList.DeleteProperty(selItem);

		//重新设置一下坐标编号
		CMFCPropertyGridProperty* pGroup = getCoodGroup();
		if (!pGroup)
			return;
		
		int count = pGroup->GetSubItemsCount();
		CString strName;
		for (int i = 0; i < count; i++)
		{
			strName.Format(_T("%d"),i+1);
			pGroup->GetSubItem(i)->SetName(strName);
		}
	}
	
}

void COutWallWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void COutWallWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	//m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("外墙坐标"),waiqiangID);

	CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(_T("1"), 0, TRUE);

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pPos->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Y"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pPos->AddSubItem(pProp);

	pGroup->AddSubItem(pPos);

	m_wndPropList.AddProperty(pGroup);

	

}

void COutWallWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void COutWallWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void COutWallWnd::SetPropListFont()
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
