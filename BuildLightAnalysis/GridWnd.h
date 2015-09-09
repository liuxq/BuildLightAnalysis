#pragma once
#include "PropertyGridCtrl.h"
#include "CommonData.h"

class CGridWnd : public CDockablePane
{
// Construction
public:
	CGridWnd();

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

	CMFCPropertyGridProperty* InsertGrid(int roomIndex, double offset = 120.0, double meshLen = 120.0);
	void CalGrid(CMFCPropertyGridProperty* pGrid);
	void LoadGrid(CMFCPropertyGridProperty* pGrid, vector<GridPoint>& points);
	void DeleteAllGrid();

	void OutputToGrids(vector<Grid>& sLines);
	void inputFromGrids(vector<Grid>& sLines);
	void save(ofstream& out);
	void load(ifstream& in);

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;
	CButton m_insertButton;
	CButton m_deleteButton;

// Implementation
public:
	virtual ~CGridWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnNewGrid();
	afx_msg void OnDeleteGrid();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

