#include "PropertyGridCtrl.h"
#pragma once

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class COptimizeWallWnd : public CDockablePane
{
// Construction
public:
	COptimizeWallWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	CMFCPropertyGridProperty* getCoodOutWallGroup()
	{
		return m_wndPropList.GetProperty(0);
	}
	CMFCPropertyGridProperty* getCoodInWallGroup()
	{
		return m_wndPropList.GetProperty(1);
	}
	void InsertPos(bool isOutWall, double,double,double,double);
	void DeletePos();

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;

// Implementation
public:
	virtual ~COptimizeWallWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnDeletePos();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

