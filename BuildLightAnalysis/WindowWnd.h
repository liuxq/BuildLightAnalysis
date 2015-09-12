#include "PropertyGridCtrl.h"
#pragma once


class CWindowWnd : public CDockablePane
{
// Construction
public:
	CWindowWnd();

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

	void InsertWindow(int outWallIndex, int inWallIndex, int winIndex, CString mat = _T("GenericDoubleGlazing60_60VisualTransmittance"), double pos = 0.5 ,double WinUpHeight = 2300.0,double WinDownHeight = 900.0,
		double WinWidth = 1200.0);
	void DeleteAllWindow();

	void save(ofstream& out);
	void load(ifstream& in);

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;
	CButton m_insertButton;
	CButton m_deleteButton;

// Implementation
public:
	virtual ~CWindowWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnNewWindow();
	afx_msg void OnDeleteWindow();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

