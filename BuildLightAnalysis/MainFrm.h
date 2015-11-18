/**********************************************************************************************//**
 * \file	MainFrm.h
 *
 * \brief	�������
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
	 * \brief	����ģʽ����������ǽģʽ����ǽģʽ�������ģʽ
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
	 * \brief	����ģʽ�������ļ���
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

	/** \brief	��ǽ���Խ��� */
	COutWallWnd		  m_wndOutWallProperties;

	/** \brief	��ǽ���Խ��� */
	CInWallWnd		  m_wndInWallProperties;

	/** \brief	�����ǽ���Խ��� */
	COptimizeWallWnd  m_wndOptimizeWallProperties;

	/** \brief	ѡ����� */
	COptionWnd		  m_wndOptionProperties;

	/** \brief	�������Խ��� */
	CWindowWnd		  m_wndWindowProperties;

	/** \brief	�������Խ��棬�������������񡢵ƾߡ����Ʒ��顢��Ա��� */
	CRoomWnd		  m_wndRoomProperties;

	/** \brief	ģʽ */
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
	 * \brief	��Ӧ��ǽ�˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditOutwall();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditInwall();
	 *
	 * \brief	��Ӧ��ǽ�˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditInwall();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditOption();
	 *
	 * \brief	��Ӧѡ��˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditOption();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditWindow();
	 *
	 * \brief	��Ӧ�����˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditWindow();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnEditRoom();
	 *
	 * \brief	��Ӧ����˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditRoom();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomCalGrid();
	 *
	 * \brief	��Ӧ�����������˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomCalGrid();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnUpdateEditOutwall(CCmdUI *pCmdUI);
	 *
	 * \brief	��Ӧ��ǽ�˵��Ƿ�ѡ��״̬����ʾ
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
	 * \brief	��Ӧ��ǽ�˵��Ƿ�ѡ��״̬����ʾ
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
	 * \brief	��Ӧ�����ǽ�˵��Ƿ�ѡ��״̬����ʾ
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
	 * \brief	��Ӧ��ӵ����ƾ߲˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSingle();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddLuminaireSet();
	 *
	 * \brief	��Ӧ��ӵȾ���˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddControlSet();
	 *
	 * \brief	��Ӧ��ӿ��Ʒ���˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddControlSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CMainFrame::OnRoomAddPerson();
	 *
	 * \brief	��Ӧ�����Ա�˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddPerson();
};


