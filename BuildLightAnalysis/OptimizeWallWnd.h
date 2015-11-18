/**********************************************************************************************//**
 * \file	OptimizeWallWnd.h
 *
 * \brief	�����ǽ���Խ���
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
	 * \brief	���ո�������β�ڵ����ǽ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	isOutWall 	true����ǽ��false����ǽ
	 * \param	parameter2	�׵�x
	 * \param	parameter3	�׵�y
	 * \param	parameter4	β��x
	 * \param	parameter5	β��y
	 * \param	parameter6	����
	 **************************************************************************************************/

	void InsertPos(bool isOutWall, double,double,double,double,CString);

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::DeleteAllPos();
	 *
	 * \brief	ɾ������ǽ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllPos();

	/**********************************************************************************************//**
	 * \fn	void COptimizeWallWnd::OutputToLines(vector<sOpWall>& sLines);
	 *
	 * \brief	�������ϵĴ����ǽ���Ե�����������
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
	 * \brief	�������еĴ����ǽ���Ե��뵽������
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
	 * \brief	�����ǽ�����ڹ�����
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
	 * \brief	�����ǽ�ӹ����ļ��ж���
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

