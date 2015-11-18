/**********************************************************************************************//**
 * \file	OutWallWnd.h
 *
 * \brief	��ǽ����
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
	 * \brief	�������������ǽ�ǵ�����
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	�ǵ�x
	 * \param	parameter2	�ǵ�y
	 **************************************************************************************************/

	void InsertPos(double, double);

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::DeleteAllPos();
	 *
	 * \brief	ɾ��������ǽ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllPos();

	/**********************************************************************************************//**
	 * \fn	void COutWallWnd::OutputToLines(vector<sLine>& sLines);
	 *
	 * \brief	��ǽ�ǵ㱣�浽������
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
	 * \brief	�Ӽ����ж�ȡ��ǽ�ǵ�
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
	 * \brief	��ǽ���ݱ��浽�����ļ���
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
	 * \brief	�ӹ����ļ��ж�ȡ��ǽ����
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

