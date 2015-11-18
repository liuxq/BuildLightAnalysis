/**********************************************************************************************//**
 * \file	OutWallWnd.h
 *
 * \brief	外墙界面
 **************************************************************************************************/

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

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::InsertPos(double, double);
	 *
	 * \brief	按照坐标插入外墙角点坐标
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	角点x
	 * \param	parameter2	角点y
	 **************************************************************************************************/

	void InsertPos(double, double);

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::DeleteAllPos();
	 *
	 * \brief	删除所有外墙
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllPos();

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::OutputToLines(vector<sLine>& sLines);
	 *
	 * \brief	外墙角点保存到集合中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	sLines	The lines.
	 **************************************************************************************************/

	void OutputToLines(vector<sLine>& sLines);

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::inputFromLines(vector<sLine>& sLines);
	 *
	 * \brief	从集合中读取外墙角点
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	sLines	The lines.
	 **************************************************************************************************/

	void inputFromLines(vector<sLine>& sLines);

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::save(ofstream& out);
	 *
	 * \brief	外墙数据保存到工程文件中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::load(ifstream& in);
	 *
	 * \brief	从工程文件中读取外墙数据
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	in	The in to load.
	 **************************************************************************************************/

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
	afx_msg void OnInsertPos();
	afx_msg void OnDeletePos();
	afx_msg void OnUpdateButton(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

