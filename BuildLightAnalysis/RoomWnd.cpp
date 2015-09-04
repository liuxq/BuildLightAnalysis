
#include "stdafx.h"

#include "RoomWnd.h"
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

CRoomWnd::CRoomWnd()
{
}

CRoomWnd::~CRoomWnd()
{
}

BEGIN_MESSAGE_MAP(CRoomWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
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

	PropertyGridProperty* pRoom = new PropertyGridProperty(strCount, 0, TRUE);
	m_wndPropList.AddProperty(pRoom);
	//m_wndPropList.UpdateProperty((PropertyGridProperty*)(pWindow));
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

bool CRoomWnd::AddToSelectedRoom(CString name, int index)
{
	CMFCPropertyGridProperty* selItem = m_wndPropList.GetCurSel();
	if (selItem && !selItem->GetParent())
	{
		for (int i = 0; i < selItem->GetSubItemsCount(); i++)
		{
			CMFCPropertyGridProperty* p = selItem->GetSubItem(i);
			if (p->GetName() == name && p->GetValue().intVal == index)
			{
				AfxMessageBox(_T("该房间已经存在该墙或窗户"));
				return false;
			}
		}
		PropertyGridProperty* pItem = new PropertyGridProperty(name, (_variant_t)index, _T("房间内的项目"));
		selItem->AddSubItem(pItem);
		m_wndPropList.AdjustLayout();
		return true;
	}
	else
	{
		AfxMessageBox(_T("尚未选择任何房间，请选择要添加进的房间"));
		return false;
	}
}
LRESULT CRoomWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	return 0;
}

void CRoomWnd::save(ofstream& out)
{
	/*double d1 = m_wndPropList.GetProperty(0)->GetValue().dblVal;
	double d2 = m_wndPropList.GetProperty(1)->GetValue().dblVal;
	serializer<double>::write(out,&d1);
	serializer<double>::write(out,&d2);*/
}
void CRoomWnd::load(ifstream& in)
{
	/*double d1;
	double d2;
	serializer<double>::read(in,&d1);
	serializer<double>::read(in,&d2);
	m_wndPropList.GetProperty(0)->SetValue(d1);
	m_wndPropList.GetProperty(1)->SetValue(d2);*/
}