/**********************************************************************************************//**
 * \file	InWallWnd.h
 *
 * \brief	内墙界面类
 **************************************************************************************************/

#pragma once

#include "PropertyGridCtrl.h"
#include "CommonData.h"


class CInWallWnd : public CDockablePane
{
// Construction
public:
	CInWallWnd();
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	CMFCPropertyGridProperty* getCoodGroup()
	{
		return m_wndPropList.GetProperty(0);
	}
	void AdjustLayout();

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::InsertPos(double,double,double,double);
	 *
	 * \brief	按照指定的收尾坐标插入内墙
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	首坐标x
	 * \param	首坐标y
	 * \param	尾部坐标x
	 * \param	尾部坐标y
	 **************************************************************************************************/

	void InsertPos(double,double,double,double);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::DeleteAllPos();
	 *
	 * \brief	删除所有内墙
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllPos();

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::OutputToLines(vector<sLine>& sLines);
	 *
	 * \brief	将界面中的内墙导出到集合中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	内墙集合
	 **************************************************************************************************/

	void OutputToLines(vector<sLine>& sLines);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::inputFromLines(vector<sLine>& sLines);
	 *
	 * \brief	将内墙集合导出到界面中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	内墙集合
	 **************************************************************************************************/

	void inputFromLines(vector<sLine>& sLines);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::save(ofstream& out);
	 *
	 * \brief	保存到工程文件
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::load(ifstream& in);
	 *
	 * \brief	从工程文件中导入
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
	virtual ~CInWallWnd();

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

