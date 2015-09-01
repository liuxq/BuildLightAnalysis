
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutWallWnd.h"
#include "InWallWnd.h"
#include "OptimizeWallWnd.h"
#include "OptionWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	COutWallWnd& GetOutWallProperty()
	{
		return m_wndOutWallProperties;
	}
	CInWallWnd& GetInWallProperty()
	{
		return m_wndInWallProperties;
	}
	COptimizeWallWnd& GetOptimizeWallProperty()
	{
		return m_wndOptimizeWallProperties;
	}
	COptionWnd& GetOptionProperty()
	{
		return m_wndOptionProperties;
	}
	CMFCStatusBar& GetStatusBar()
	{
		return m_wndStatusBar;
	}
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	COutWallWnd		  m_wndOutWallProperties;
	CInWallWnd		  m_wndInWallProperties;
	COptimizeWallWnd  m_wndOptimizeWallProperties;
	COptionWnd		  m_wndOptionProperties;

public:
	

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

private:

public:
	afx_msg void OnEditOutwall();
	afx_msg void OnEditInwall();
	afx_msg void OnEditOption();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditWindow();
};


