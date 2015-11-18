/**********************************************************************************************//**
 * \file	InWallWnd.h
 *
 * \brief	��ǽ������
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
	 * \brief	����ָ������β���������ǽ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	������x
	 * \param	������y
	 * \param	β������x
	 * \param	β������y
	 **************************************************************************************************/

	void InsertPos(double,double,double,double);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::DeleteAllPos();
	 *
	 * \brief	ɾ��������ǽ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllPos();

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::OutputToLines(vector<sLine>& sLines);
	 *
	 * \brief	�������е���ǽ������������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	��ǽ����
	 **************************************************************************************************/

	void OutputToLines(vector<sLine>& sLines);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::inputFromLines(vector<sLine>& sLines);
	 *
	 * \brief	����ǽ���ϵ�����������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	��ǽ����
	 **************************************************************************************************/

	void inputFromLines(vector<sLine>& sLines);

	/**********************************************************************************************//**
	 * \fn	void CInWallWnd::save(ofstream& out);
	 *
	 * \brief	���浽�����ļ�
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
	 * \brief	�ӹ����ļ��е���
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

