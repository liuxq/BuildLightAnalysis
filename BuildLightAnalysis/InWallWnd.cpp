
#include "stdafx.h"

#include "InWallWnd.h"
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

CInWallWnd::CInWallWnd()
{
}

CInWallWnd::~CInWallWnd()
{
}

BEGIN_MESSAGE_MAP(CInWallWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	
	ON_COMMAND(IDC_IN_INSERT_BUTTON, OnInsertPos)
	ON_UPDATE_COMMAND_UI(IDC_IN_INSERT_BUTTON, OnUpdateProperties2)
	ON_COMMAND(IDC_IN_DELETE_BUTTON, OnDeletePos)
	ON_UPDATE_COMMAND_UI(IDC_IN_DELETE_BUTTON, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CInWallWnd::AdjustLayout()
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

int CInWallWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	if (!m_insertButton.Create(_T("插入内墙"),dwViewStyle, rectDummy, this, IDC_IN_INSERT_BUTTON))
	{
		return -1;      // fail to create
	}
	if (!m_deleteButton.Create(_T("删除内墙"),dwViewStyle, rectDummy, this, IDC_IN_DELETE_BUTTON))
	{
		return -1;      // fail to create
	}

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_IN_PROPERTY_CTRL))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();
	return 0;
}

void CInWallWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CInWallWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CInWallWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CInWallWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CInWallWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}
void CInWallWnd::InsertPos(double x, double y, double x1, double y1)
{
	CMFCPropertyGridProperty* pGroup = getCoodGroup();
	int count = pGroup->GetSubItemsCount();
	CString strCount;
	strCount.Format(_T("%d"),count);

	PropertyGridProperty* pPos = new PropertyGridProperty(strCount, 0, TRUE);

	PropertyGridProperty* pStart = new PropertyGridProperty(_T("Start"), 0, TRUE);
	PropertyGridProperty* pProp = new PropertyGridProperty(_T("X"), (_variant_t) x, _T("内墙坐标X值"));
	pStart->AddSubItem(pProp);
	pProp = new PropertyGridProperty( _T("Y"), (_variant_t) y, _T("内墙坐标Y值"));
	pStart->AddSubItem(pProp);

	PropertyGridProperty* pEnd = new PropertyGridProperty(_T("End"), 0, TRUE);
	pProp = new PropertyGridProperty(_T("X"), (_variant_t) x1, _T("内墙坐标X值"));
	pEnd->AddSubItem(pProp);
	pProp = new PropertyGridProperty( _T("Y"), (_variant_t) y1, _T("内墙坐标Y值"));
	pEnd->AddSubItem(pProp);

	pPos->AddSubItem(pStart);
	pPos->AddSubItem(pEnd);


	pGroup->AddSubItem(pPos);

	m_wndPropList.UpdateProperty((PropertyGridProperty*)(pGroup));
	m_wndPropList.AdjustLayout();

	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}
void CInWallWnd::OnInsertPos()
{
	InsertPos(0,0,2000,2000);
}
void CInWallWnd::DeleteAllPos()
{
	CMFCPropertyGridProperty* pGroup = getCoodGroup();
	((PropertyGridProperty*)pGroup)->RemoveAllSubItem();
}
void CInWallWnd::inputFromLines(vector<sLine>& sLines)
{
	DeleteAllPos();	
	for (int i = 0; i < sLines.size(); i++)
	{
		InsertPos(sLines[i].s.x, sLines[i].s.y, sLines[i].e.x, sLines[i].e.y);
	}
	
}
void CInWallWnd::OutputToLines(vector<sLine>& sLines)
{
	CMFCPropertyGridProperty* inWallPos = getCoodGroup();
	//内墙
	Vec2d ps, pe;
	for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
	{
		ps.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		ps.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

		pe.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		pe.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

		sLines.push_back(sLine(ps,pe,sLine::IN_WALL));
	}
}
void CInWallWnd::save(ofstream& out)
{
	vector<sLine> sLines;
	OutputToLines(sLines);
	serializer<sLine>::write(out, &sLines);
}
void CInWallWnd::load(ifstream& in)
{
	vector<sLine> sLines;
	serializer<sLine>::read(in, &sLines);
	inputFromLines(sLines);
}
void CInWallWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CInWallWnd::OnDeletePos()
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem && selItem->GetParent() && selItem->GetParent()->GetData() == waiqiangID)
	{
		CString name = selItem->GetName();
		int index = _ttoi(name);
		
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

			if (i == index)
				m_wndPropList.SetCurSel(pGroup->GetSubItem(i));
		}
	}
	
	//更新视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	pMain->GetActiveView()->Invalidate(); 
}

void CInWallWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CInWallWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("内墙线段"),waiqiangID);

	m_wndPropList.AddProperty(pGroup);

	

}

void CInWallWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CInWallWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CInWallWnd::SetPropListFont()
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
