/**********************************************************************************************//**
 * \file	WindowWnd.h
 *
 * \brief	窗户界面类
 **************************************************************************************************/

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

	/**********************************************************************************************//**
	 * \fn	void CWindowWnd::InsertWindow(int outWallIndex, int inWallIndex, int winIndex, CString mat = _T("GenericDoubleGlazing60_60VisualTransmittance"), double pos = 0.5 ,double WinUpHeight = 2300.0,double WinDownHeight = 900.0, double WinWidth = 1200.0);
	 *
	 * \brief	插入窗户
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	outWallIndex 	Zero-based index of the out wall.
	 * \param	inWallIndex  	Zero-based index of the in wall.
	 * \param	winIndex	 	Zero-based index of the window.
	 * \param	mat			 	材质
	 * \param	pos			 	比例坐标
	 * \param	WinUpHeight  	Height of the window up.
	 * \param	WinDownHeight	Height of the window down.
	 * \param	WinWidth	 	Width of the window.
	 **************************************************************************************************/

	void InsertWindow(int outWallIndex, int inWallIndex, int winIndex, CString mat = _T("GenericDoubleGlazing60_60VisualTransmittance"), double pos = 0.5 ,double WinUpHeight = 2300.0,double WinDownHeight = 900.0,
		double WinWidth = 1200.0);

	/**********************************************************************************************//**
	 * \fn	void CWindowWnd::DeleteAllWindow();
	 *
	 * \brief	Deletes all window.
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllWindow();

	/**********************************************************************************************//**
	 * \fn	void CWindowWnd::DeleteWindowByIndex(int);
	 *
	 * \brief	Deletes the window by index described by parameter1.
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	The first parameter.
	 **************************************************************************************************/

	void DeleteWindowByIndex(int);

	/**********************************************************************************************//**
	 * \fn	void CWindowWnd::save(ofstream& out);
	 *
	 * \brief	保存窗户信息到工程文件
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);

	/**********************************************************************************************//**
	 * \fn	void CWindowWnd::load(ifstream& in);
	 *
	 * \brief	从工程文件中读取窗户信息
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	in	The in to load.
	 **************************************************************************************************/

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
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CWindowWnd::OnNewWindow();
	 *
	 * \brief	新建窗户按钮
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnNewWindow();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CWindowWnd::OnDeleteWindow();
	 *
	 * \brief	删除窗户按钮
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnDeleteWindow();
	afx_msg void OnUpdateButton(CCmdUI* pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg LRESULT CWindowWnd::OnPropertyChanged(WPARAM,LPARAM);
	 *
	 * \brief	当窗户界面发生修改时调用
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	The wParam field of the message.
	 * \param	parameter2	The lParam field of the message.
	 *
	 * \return	A LRESULT.
	 **************************************************************************************************/

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

