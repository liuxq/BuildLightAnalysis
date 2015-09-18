
#include "stdafx.h"

#include "OutWallWnd.h"
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
	
	ON_COMMAND(IDC_OUT_INSERT_BUTTON, OnInsertPos)
	ON_UPDATE_COMMAND_UI(IDC_OUT_INSERT_BUTTON, OnUpdateProperties2)
	ON_COMMAND(IDC_OUT_DELETE_BUTTON, OnDeletePos)
	ON_UPDATE_COMMAND_UI(IDC_OUT_DELETE_BUTTON, OnUpdateProperties2)
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

	if (!m_insertButton.Create(_T("插入坐标"),dwViewStyle, rectDummy, this, IDC_OUT_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("删除坐标"),dwViewStyle, rectDummy, this, IDC_OUT_DELETE_BUTTON))
	{
		return -1;      // fail to create
	}

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_OUT_PROPERTY_CTRL))
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
void COutWallWnd::InsertPos(double x, double y)
{
	CMFCPropertyGridProperty* pGroup = getCoodGroup();
	int count = pGroup->GetSubItemsCount();
	CString strCount;
	strCount.Format(_T("%d"),count);

	PropertyGridProperty* pPos = new PropertyGridProperty(strCount, 0, TRUE);

	PropertyGridProperty* pProp = new PropertyGridProperty(_T("X"), (_variant_t) x, _T("内墙坐标X值"));
	pPos->AddSubItem(pProp);

	pProp = new PropertyGridProperty( _T("Y"), (_variant_t) y, _T("内墙坐标Y值"));
	pPos->AddSubItem(pProp);

	pGroup->AddSubItem(pPos);
	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pGroup));
	m_wndPropList.AdjustLayout();

	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}
void COutWallWnd::OnInsertPos()
{
	InsertPos(1000.0, 1000.0);
}

void COutWallWnd::inputFromLines(vector<sLine>& sLines)
{
	DeleteAllPos();

	if (sLines.size() >= 3)
	{
		for (int i = 0; i < sLines.size(); i++)
		{
			InsertPos(sLines[i].s.x, sLines[i].s.y);
		}
	}
	else if (sLines.size() > 0)
	{
		InsertPos(sLines[0].s.x, sLines[0].s.y);
		InsertPos(sLines[0].e.x, sLines[0].e.y);
	}
}
void COutWallWnd::DeleteAllPos()
{
	CMFCPropertyGridProperty* pGroup = getCoodGroup();
	((PropertyGridProperty*)pGroup)->RemoveAllSubItem();
}

void COutWallWnd::save(ofstream& out)
{
	vector<sLine> sLines;
	OutputToLines(sLines);
	serializer<sLine>::write(out, &sLines);
}
void COutWallWnd::load(ifstream& in)
{
	vector<sLine> sLines;
	serializer<sLine>::read(in, &sLines);
	inputFromLines(sLines);
}
void COutWallWnd::OutputToLines(vector<sLine>& sLines)
{
	CMFCPropertyGridProperty* outWallPos = getCoodGroup();
	Vec2d ps, pe, pFirst;
	//外墙
	for (int i = 0; i < outWallPos->GetSubItemsCount()-1; i++)
	{
		ps.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
		ps.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;

		pe.x = outWallPos->GetSubItem(i+1)->GetSubItem(0)->GetValue().dblVal;
		pe.y = outWallPos->GetSubItem(i+1)->GetSubItem(1)->GetValue().dblVal;

		if (i == 0)
		{
			pFirst = ps;
		}
		sLines.push_back(sLine(ps,pe, sLine::OUT_WALL));
	}
	if (outWallPos->GetSubItemsCount() > 2)
		sLines.push_back(sLine(pe,pFirst,sLine::OUT_WALL));
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
			strName.Format(_T("%d"),i);
			pGroup->GetSubItem(i)->SetName(strName);
		}
	}
	
	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}

void COutWallWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void COutWallWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("外墙坐标"),waiqiangID);
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
