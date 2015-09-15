#include "PropertyGridCtrl.h"
#pragma once


enum
{
	OPTION_START = 0,
	OPTION_LEVEL_HEIGHT = OPTION_START,  //层高
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
	OPTION_KEYGRID_COLOR,

	OPTION_NUM
};
enum
{
	OPTION_TYPE_INT = 0,
	OPTION_TYPE_DOUBLE,
	OPTION_TYPE_STRING,
	OPTION_TYPE_COLOR
};


class COptionWnd : public CDockablePane
{
public:
	
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

	int GetDataInt(int dataIndex)
	{
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			return data->GetValue().intVal;
		}
		return 0;
	}
	double GetDataDouble(int dataIndex)
	{
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			return data->GetValue().dblVal;
		}
		return 0.0;
	}
	CString GetDataCString(int dataIndex)
	{
		CString tmp;
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			tmp = data->GetValue().bstrVal;
		}
		return tmp;
	}
	void loadMaterialTemplate();
	void GetTransform(double s, double centerX, double centerY);
	void SetTransform();
	void save(ofstream& out);
	void load(ifstream& in);
	void ResetAllOption();
	

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;

	int m_DataType[OPTION_NUM];

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

