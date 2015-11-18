/**********************************************************************************************//**
 * \file	OptimizeWallWnd.h
 *
 * \brief	处理后墙属性界面
 **************************************************************************************************/

#pragma once
#include "PropertyGridCtrl.h"
#include "CommonData.h"

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

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::InsertPos(bool isOutWall, double,double,double,double,CString);
	 *
	 * \brief	按照给定的首尾节点插入墙
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	isOutWall 	true：外墙，false：内墙
	 * \param	parameter2	首点x
	 * \param	parameter3	首点y
	 * \param	parameter4	尾点x
	 * \param	parameter5	尾点y
	 * \param	parameter6	材料
	 **************************************************************************************************/

	void InsertPos(bool isOutWall, double,double,double,double,CString);

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::DeleteAllPos();
	 *
	 * \brief	删除所有墙
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllPos();

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::OutputToLines(vector<sOpWall>& sLines);
	 *
	 * \brief	将界面上的处理后墙属性导出到集合中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	sLines	The lines.
	 **************************************************************************************************/

	void OutputToLines(vector<sOpWall>& sLines);

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::inputFromLines(vector<sOpWall>& sLines);
	 *
	 * \brief	将集合中的处理后墙属性导入到界面上
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	sLines	The lines.
	 **************************************************************************************************/

	void inputFromLines(vector<sOpWall>& sLines);

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::save(ofstream& out);
	 *
	 * \brief	处理后墙保存在工程中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::load(ifstream& in);
	 *
	 * \brief	处理后墙从工程文件中读入
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

// Implementation
public:
	virtual ~COptimizeWallWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

