/**********************************************************************************************//**
 * \file	MainFrm.h
 *
 * \brief	主框架类
 **************************************************************************************************/

#pragma once
#include "OutWallWnd.h"
#include "InWallWnd.h"
#include "OptimizeWallWnd.h"
#include "OptionWnd.h"
#include "WindowWnd.h"
#include "RoomWnd.h"


///< An enum constant representing the enum option
enum
{
	MODE_OUTWALL = 0,
	MODE_INWALL,
	MODE_OPTIMIZE
};

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

	/**********************************************************************************************//**
	 * \fn	void CMainFrame::SetMode(int m)
	 *
	 * \brief	设置模式，包括：外墙模式、内墙模式、处理后模式
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	m	The int to process.
	 **************************************************************************************************/

	void SetMode(int m)
	{
		m_eMode = m;
	}
	int GetMode()
	{
		return m_eMode;
	}

	/**********************************************************************************************//**
	 * \fn	void CMainFrame::saveMode(ofstream& out);
	 *
	 * \brief	保存模式到工程文件中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out.
	 **************************************************************************************************/

	void saveMode(ofstream& out);
	void loadMode(ifstream& in);
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
	CWindowWnd& GetWindowProperty()
	{
		return m_wndWindowProperties;
	}
	CRoomWnd& GetRoomProperty()
	{
		return m_wndRoomProperties;
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

	/** \brief	外墙属性界面 */
	COutWallWnd		  m_wndOutWallProperties;

	/** \brief	内墙属性界面 */
	CInWallWnd		  m_wndInWallProperties;

	/** \brief	处理后墙属性界面 */
	COptimizeWallWnd  m_wndOptimizeWallProperties;

	/** \brief	选项界面 */
	COptionWnd		  m_wndOptionProperties;

	/** \brief	窗户属性界面 */
	CWindowWnd		  m_wndWindowProperties;

	/** \brief	房间属性界面，包含：计算网格、灯具、控制分组、人员相关 */
	CRoomWnd		  m_wndRoomProperties;

	/** \brief	模式 */
	int m_eMode;
	
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

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditOutwall();
	 *
	 * \brief	对应外墙菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditOutwall();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditInwall();
	 *
	 * \brief	对应内墙菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditInwall();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditOption();
	 *
	 * \brief	对应选项菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditOption();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditWindow();
	 *
	 * \brief	对应窗户菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditWindow();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditRoom();
	 *
	 * \brief	对应房间菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditRoom();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomCalGrid();
	 *
	 * \brief	对应计算计算网格菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomCalGrid();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnUpdateEditOutwall(CCmdUI *pCmdUI);
	 *
	 * \brief	对应外墙菜单是否选中状态的显示
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pCmdUI	If non-null, the command user interface.
	 **************************************************************************************************/

	afx_msg void OnUpdateEditOutwall(CCmdUI *pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnUpdateEditInwall(CCmdUI *pCmdUI);
	 *
	 * \brief	对应内墙菜单是否选中状态的显示
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pCmdUI	If non-null, the command user interface.
	 **************************************************************************************************/

	afx_msg void OnUpdateEditInwall(CCmdUI *pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnUpdateEditOptimize(CCmdUI *pCmdUI);
	 *
	 * \brief	对应处理后墙菜单是否选中状态的显示
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pCmdUI	If non-null, the command user interface.
	 **************************************************************************************************/

	afx_msg void OnUpdateEditOptimize(CCmdUI *pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddLuminaireSingle();
	 *
	 * \brief	对应添加单个灯具菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSingle();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddLuminaireSet();
	 *
	 * \brief	对应添加等剧组菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddControlSet();
	 *
	 * \brief	对应添加控制分组菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddControlSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddPerson();
	 *
	 * \brief	对应添加人员菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddPerson();
};


