#pragma once

#include "PropertyGridCtrl.h"
#include "CommonData.h"

class COutWallWnd : public CDockablePane
{
// Construction
public:
	COutWallWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	CMFCPropertyGridProperty* getCoodGroup()
	{
		return m_wndPropList.GetProperty(0);
	}
	void InsertPos(double, double);
	void DeleteAllPos();
	void OutputToLines(vector<sLine>& sLines);
	void inputFromLines(vector<sLine>& sLines);
	void save(ofstream& out);
	void load(ifstream& in);

protected:
	CFont m_fntPropList;
	CButton m_insertButton;
	CButton m_deleteButton;
	PropertyGridCtrl m_wndPropList;

// Implementation
public:
	virtual ~COutWallWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnInsertPos();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnDeletePos();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

