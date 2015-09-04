
#include "stdafx.h"

#include "OptionWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "BuildLightAnalysis.h"
#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"

#include "Serializer.h"
#include "MathUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

COptionWnd::COptionWnd()
{
}

COptionWnd::~COptionWnd()
{
}

BEGIN_MESSAGE_MAP(COptionWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void COptionWnd::AdjustLayout()
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

int COptionWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_OPTION_CTRL))
	{
		TRACE0("Failed to create Option Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();
	return 0;
}

void COptionWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void COptionWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void COptionWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void COptionWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void COptionWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void COptionWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pLevelHigh = new CMFCPropertyGridProperty(_T("层高"),(_variant_t) 2800.0,_T("层高度"), 1);
	CMFCPropertyGridProperty* pOptimizeTh = new CMFCPropertyGridProperty(_T("处理阈值"),(_variant_t) 400.0,_T("处理内外墙时低于多少毫米会进行补足和删除"), 2);
	CMFCPropertyGridProperty* pOutWallMaterial = new PropertyGridProperty(_T("默认外墙材质"), _T("GenericCeiling_80PercentReflectance"), _T("默认外墙材质"));
	CMFCPropertyGridProperty* pInWallMaterial = new PropertyGridProperty(_T("默认内墙材质"), _T("GenericCeiling_80PercentReflectance"), _T("默认内墙材质"));
	CMFCPropertyGridProperty* pWindowMaterial = new PropertyGridProperty(_T("默认窗户材质"), _T("GenericCeiling_80PercentReflectance"), _T("默认窗户材质"));
	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("像素毫米比"),(_variant_t) 0.1,_T("缩放系数：像素/毫米"));
	CMFCPropertyGridProperty* pCenterX = new CMFCPropertyGridProperty(_T("初始原点位置X"),(_variant_t) 100.0,_T("平移系数，X"));
	CMFCPropertyGridProperty* pCenterY = new CMFCPropertyGridProperty(_T("初始原点位置Y"),(_variant_t) 100.0,_T("平移系数，Y"));
	
	m_wndPropList.AddProperty(pLevelHigh);
	m_wndPropList.AddProperty(pOptimizeTh);
	m_wndPropList.AddProperty(pOutWallMaterial);
	m_wndPropList.AddProperty(pInWallMaterial);
	m_wndPropList.AddProperty(pWindowMaterial);
	m_wndPropList.AddProperty(pScale);
	m_wndPropList.AddProperty(pCenterX);
	m_wndPropList.AddProperty(pCenterY);
}

void COptionWnd::loadMaterialTemplate()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	CString strMat;
	vector<Material>& mats = pDoc->getMaterials();

	CMFCPropertyGridProperty* pOutWallMaterial = m_wndPropList.GetProperty(2);
	CMFCPropertyGridProperty* pInWallMaterial = m_wndPropList.GetProperty(3);
	CMFCPropertyGridProperty* pWindowMaterial = m_wndPropList.GetProperty(4);

	for (int i = 0; i < mats.size(); i++)
	{
		strMat.Format(_T("%s"), mats[i].name.c_str());
		pOutWallMaterial->AddOption(strMat);
		pInWallMaterial->AddOption(strMat);
		pWindowMaterial->AddOption(strMat);
	}

}
void COptionWnd::GetTransform(double s, double centerX, double centerY)
{
	m_wndPropList.GetProperty(5)->SetValue(s);
	m_wndPropList.GetProperty(6)->SetValue(centerX);
	m_wndPropList.GetProperty(7)->SetValue(centerY);
}
void COptionWnd::SetTransform()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisView* pView = (CBuildLightAnalysisView*)pMain->GetActiveView();
	double d5 = m_wndPropList.GetProperty(5)->GetValue().dblVal;
	double d6 = m_wndPropList.GetProperty(6)->GetValue().dblVal;
	double d7 = m_wndPropList.GetProperty(7)->GetValue().dblVal;
	
	pView->GetTransform().Set(d5,Vec2d(d6,d7));
	pView->Invalidate();
}
LRESULT COptionWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	SetTransform();
	return 0;
}
void COptionWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void COptionWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void COptionWnd::SetPropListFont()
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
void COptionWnd::save(ofstream& out)
{
	double d0 = m_wndPropList.GetProperty(0)->GetValue().dblVal;
	double d1 = m_wndPropList.GetProperty(1)->GetValue().dblVal;
	CString d2(m_wndPropList.GetProperty(2)->GetValue().bstrVal);
	CString d3(m_wndPropList.GetProperty(3)->GetValue().bstrVal);
	CString d4(m_wndPropList.GetProperty(4)->GetValue().bstrVal);
	double d5 = m_wndPropList.GetProperty(5)->GetValue().dblVal;
	double d6 = m_wndPropList.GetProperty(6)->GetValue().dblVal;
	double d7 = m_wndPropList.GetProperty(7)->GetValue().dblVal;

	string st2 = CStringToString(d2);
	string st3 = CStringToString(d3);
	string st4 = CStringToString(d4);
	
	serializer<double>::write(out,&d0);
	serializer<double>::write(out,&d1);
	serializer<string>::writeString(out,&st2);
	serializer<string>::writeString(out,&st3);
	serializer<string>::writeString(out,&st4);
	serializer<double>::write(out,&d5);
	serializer<double>::write(out,&d6);
	serializer<double>::write(out,&d7);
}
void COptionWnd::load(ifstream& in)
{
	double d0;
	double d1;
	string d2;
	string d3;
	string d4;
	double d5;
	double d6;
	double d7;
	serializer<double>::read(in,&d0);
	serializer<double>::read(in,&d1);
	serializer<string>::readString(in,&d2);
	serializer<string>::readString(in,&d3);
	serializer<string>::readString(in,&d4);
	serializer<double>::read(in,&d5);
	serializer<double>::read(in,&d6);
	serializer<double>::read(in,&d7);

	m_wndPropList.GetProperty(0)->SetValue(d0);
	m_wndPropList.GetProperty(1)->SetValue(d1);
	m_wndPropList.GetProperty(2)->SetValue(stringToCString(d2));
	m_wndPropList.GetProperty(3)->SetValue(stringToCString(d3));
	m_wndPropList.GetProperty(4)->SetValue(stringToCString(d4));
	m_wndPropList.GetProperty(5)->SetValue(d5);
	m_wndPropList.GetProperty(6)->SetValue(d6);
	m_wndPropList.GetProperty(7)->SetValue(d7);
}