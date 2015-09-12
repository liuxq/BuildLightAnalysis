#include "PropertyGridCtrl.h"
#pragma once


class COptionWnd : public CDockablePane
{
public:
	enum
	{
		OPTION_LEVEL_HEIGHT = 0,  //层高
		OPTION_OPTIMIZE_TH, //处理阈值，毫米
		OPTION_OUTWALL_MAT,	//外墙材质
		OPTION_INWALL_MAT,
		OPTION_WINDOW_MAT,
		OPTION_FLOOR_MAT,
		OPTION_ROOF_MAT,
		OPTION_PIX_MM_SCALE,
		OPTION_ORIGIN_X,
		OPTION_ORIGIN_Y,
		OPTION_OUTWALL_COLOR,
		OPTION_INWALL_COLOR,

		OPTION_NUM
	};
// Construction
public:
	COptionWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	PropertyGridCtrl* getPropList()
	{
		return &m_wndPropList;
	}

	double getProperty(int dataIndex)
	{
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			return data->GetValue().dblVal;
		}
		return 0.0;
	}
	void loadMaterialTemplate();
	void GetTransform(double s, double centerX, double centerY);
	void SetTransform();
	void save(ofstream& out);
	void load(ifstream& in);

	COLORREF GetOutWallColor()
	{
		return ((CMFCPropertyGridColorProperty*)m_wndPropList.GetProperty(OPTION_OUTWALL_COLOR))->GetColor();
	}
	COLORREF GetInWallColor()
	{
		return ((CMFCPropertyGridColorProperty*)m_wndPropList.GetProperty(OPTION_INWALL_COLOR))->GetColor();
	}
	CString GetOutWallMat()
	{
		return m_wndPropList.GetProperty(OPTION_OUTWALL_MAT)->GetValue().bstrVal;
	}
	CString GetInWallMat()
	{
		return m_wndPropList.GetProperty(OPTION_INWALL_MAT)->GetValue().bstrVal;
	}
	CString GetWindowMat()
	{
		return m_wndPropList.GetProperty(OPTION_WINDOW_MAT)->GetValue().bstrVal;
	}
	

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;

// Implementation
public:
	virtual ~COptionWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

