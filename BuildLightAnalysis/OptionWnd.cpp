
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

	CMFCPropertyGridProperty* pLevelHigh = new CMFCPropertyGridProperty(_T("层高(mm)"),(_variant_t) 2800.0,_T("层高度，单位：mm"), 1);
	CMFCPropertyGridProperty* pOptimizeTh = new CMFCPropertyGridProperty(_T("处理阈值(mm)"),(_variant_t) 400.0,_T("处理内外墙时低于多少毫米会进行补足和删除"), 2);
	CMFCPropertyGridProperty* pOutWallMaterial = new PropertyGridProperty(_T("默认外墙材质"), _T("GenericInteriorWall_50PercentReflectance"), _T("默认外墙材质"));
	CMFCPropertyGridProperty* pInWallMaterial = new PropertyGridProperty(_T("默认内墙材质"), _T("GenericInteriorWall_50PercentReflectance"), _T("默认内墙材质"));
	CMFCPropertyGridProperty* pWindowMaterial = new PropertyGridProperty(_T("默认窗户材质"), _T("GenericDoubleGlazing60_60VisualTransmittance"), _T("默认窗户材质"));
	CMFCPropertyGridProperty* pFloorMaterial = new PropertyGridProperty(_T("默认地面材质"), _T("GenericFloor_20PercentReflectance"), _T("默认地面材质"));
	CMFCPropertyGridProperty* pRoofMaterial = new PropertyGridProperty(_T("默认棚顶材质"), _T("GenericCeiling_80PercentReflectance"), _T("默认棚顶材质"));
	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("像素毫米比"),(_variant_t) 0.1,_T("缩放系数：像素/毫米"));
	CMFCPropertyGridProperty* pCenterX = new CMFCPropertyGridProperty(_T("初始原点位置X"),(_variant_t) 100.0,_T("平移系数，X"));
	CMFCPropertyGridProperty* pCenterY = new CMFCPropertyGridProperty(_T("初始原点位置Y"),(_variant_t) 100.0,_T("平移系数，Y"));
	CMFCPropertyGridColorProperty* pOutWallColor = new CMFCPropertyGridColorProperty(_T("外墙颜色"),RGB(0, 111, 200));
	CMFCPropertyGridColorProperty* pInWallColor = new CMFCPropertyGridColorProperty(_T("内墙颜色"),RGB(0, 111, 200));
	CMFCPropertyGridColorProperty* pInKeyGridColor = new CMFCPropertyGridColorProperty(_T("关键点颜色"),RGB(255, 0, 0));
	CMFCPropertyGridProperty* pCity = new PropertyGridProperty(_T("城市"), _T("Beijing.Beijing"), _T("默认城市"));
	CMFCPropertyGridProperty* pNorth = new PropertyGridProperty(_T("指北针角度(角度)"), (_variant_t)0.0, _T("指北针角度， -180~180"));

	
	m_wndPropList.AddProperty(pLevelHigh);m_DataType[OPTION_LEVEL_HEIGHT] = OPTION_TYPE_DOUBLE;
	m_wndPropList.AddProperty(pOptimizeTh);m_DataType[OPTION_OPTIMIZE_TH] = OPTION_TYPE_DOUBLE;
	m_wndPropList.AddProperty(pOutWallMaterial);m_DataType[OPTION_OUTWALL_MAT] = OPTION_TYPE_STRING;
	m_wndPropList.AddProperty(pInWallMaterial);m_DataType[OPTION_INWALL_MAT] = OPTION_TYPE_STRING;
	m_wndPropList.AddProperty(pWindowMaterial);m_DataType[OPTION_WINDOW_MAT] = OPTION_TYPE_STRING;
	m_wndPropList.AddProperty(pFloorMaterial);m_DataType[OPTION_FLOOR_MAT] = OPTION_TYPE_STRING;
	m_wndPropList.AddProperty(pRoofMaterial);m_DataType[OPTION_ROOF_MAT] = OPTION_TYPE_STRING;
	m_wndPropList.AddProperty(pScale);m_DataType[OPTION_PIX_MM_SCALE] = OPTION_TYPE_DOUBLE;
	m_wndPropList.AddProperty(pCenterX);m_DataType[OPTION_ORIGIN_X] = OPTION_TYPE_DOUBLE;
	m_wndPropList.AddProperty(pCenterY);m_DataType[OPTION_ORIGIN_Y] = OPTION_TYPE_DOUBLE;
	m_wndPropList.AddProperty(pOutWallColor);m_DataType[OPTION_OUTWALL_COLOR] = OPTION_TYPE_COLOR;
	m_wndPropList.AddProperty(pInWallColor);m_DataType[OPTION_INWALL_COLOR] = OPTION_TYPE_COLOR;
	m_wndPropList.AddProperty(pInKeyGridColor);m_DataType[OPTION_KEYGRID_COLOR] = OPTION_TYPE_COLOR;
	m_wndPropList.AddProperty(pCity);m_DataType[OPTION_CITY] = OPTION_TYPE_STRING;
	m_wndPropList.AddProperty(pNorth);m_DataType[OPTION_NORTH] = OPTION_TYPE_DOUBLE;
}

void COptionWnd::loadMaterialTemplateAndCity()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisDoc* pDoc = (CBuildLightAnalysisDoc*)pMain->GetActiveDocument();
	CString strMat;
	vector<Material>& mats = pDoc->getMaterials();

	CMFCPropertyGridProperty* pOutWallMaterial = m_wndPropList.GetProperty(OPTION_OUTWALL_MAT);pOutWallMaterial->AllowEdit(FALSE);
	CMFCPropertyGridProperty* pInWallMaterial = m_wndPropList.GetProperty(OPTION_INWALL_MAT);pInWallMaterial->AllowEdit(FALSE);
	CMFCPropertyGridProperty* pWindowMaterial = m_wndPropList.GetProperty(OPTION_WINDOW_MAT);pWindowMaterial->AllowEdit(FALSE);
	CMFCPropertyGridProperty* pFloorMaterial = m_wndPropList.GetProperty(OPTION_FLOOR_MAT);pFloorMaterial->AllowEdit(FALSE);
	CMFCPropertyGridProperty* pRoofMaterial = m_wndPropList.GetProperty(OPTION_ROOF_MAT);pRoofMaterial->AllowEdit(FALSE);

	for (int i = 0; i < mats.size(); i++)
	{
		strMat = StringToCString(mats[i].name);
		pOutWallMaterial->AddOption(strMat);
		pInWallMaterial->AddOption(strMat);
		pWindowMaterial->AddOption(strMat);
		pFloorMaterial->AddOption(strMat);
		pRoofMaterial->AddOption(strMat);
	}

	CMFCPropertyGridProperty* pCitys = m_wndPropList.GetProperty(OPTION_CITY);
	vector<CString>& citys = pDoc->getCitys();
	for (int i = 0; i < citys.size(); i++)
	{
		pCitys->AddOption(citys[i]);
	}
	pCitys->AllowEdit(FALSE);

}
void COptionWnd::GetTransform(double s, double centerX, double centerY)
{
	m_wndPropList.GetProperty(OPTION_PIX_MM_SCALE)->SetValue(s);
	m_wndPropList.GetProperty(OPTION_ORIGIN_X)->SetValue(centerX);
	m_wndPropList.GetProperty(OPTION_ORIGIN_Y)->SetValue(centerY);
}
void COptionWnd::SetTransform()
{
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
	CBuildLightAnalysisView* pView = (CBuildLightAnalysisView*)pMain->GetActiveView();
	double s = m_wndPropList.GetProperty(OPTION_PIX_MM_SCALE)->GetValue().dblVal;
	double X = m_wndPropList.GetProperty(OPTION_ORIGIN_X)->GetValue().dblVal;
	double Y = m_wndPropList.GetProperty(OPTION_ORIGIN_Y)->GetValue().dblVal;
	
	pView->GetTransform().Set(s,Vec2d(X,Y));
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
	double dbl;
	long longv;
	string str;
	CString cstr;

	for (int i = 0; i < OPTION_NUM; i++)
	{
		switch (m_DataType[OPTION_START + i])
		{
		case OPTION_TYPE_DOUBLE:
			dbl = m_wndPropList.GetProperty(OPTION_START + i)->GetValue().dblVal;
			serializer<double>::write(out,&dbl);
			break;
		case OPTION_TYPE_STRING:
			cstr = m_wndPropList.GetProperty(OPTION_START + i)->GetValue().bstrVal;
			serializer<string>::writeString(out,&CStringToString(cstr));
			break;
		case OPTION_TYPE_COLOR:
			longv = m_wndPropList.GetProperty(OPTION_START + i)->GetValue().lVal;
			serializer<long>::write(out,&longv);
			break;

		}
	}


}
void COptionWnd::load(ifstream& in)
{
	double dbl;
	long longv;
	string str;
	CString cstr;

	for (int i = 0; i < OPTION_NUM; i++)
	{
		switch (m_DataType[OPTION_START + i])
		{
		case OPTION_TYPE_DOUBLE:
			serializer<double>::read(in,&dbl);
			m_wndPropList.GetProperty(OPTION_START + i)->SetValue(dbl);
			break;
		case OPTION_TYPE_STRING:
			serializer<string>::readString(in,&str);
			m_wndPropList.GetProperty(OPTION_START + i)->SetValue(StringToCString(str));
			break;
		case OPTION_TYPE_COLOR:
			serializer<long>::read(in,&longv);
			((CMFCPropertyGridColorProperty*)m_wndPropList.GetProperty(OPTION_START + i))->SetColor(longv);
			break;

		}
	}

}

void COptionWnd::ResetAllOption()
{
	m_wndPropList.RemoveAll();
	InitPropList();

}