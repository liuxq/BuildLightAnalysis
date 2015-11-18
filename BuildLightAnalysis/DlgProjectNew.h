/**********************************************************************************************//**
 * \file	DlgProjectNew.h
 *
 * \brief	�½���Ŀ�Ի���
 **************************************************************************************************/

#pragma once
// DlgProjectNew dialog

class DlgProjectNew : public CDialogEx
{
	DECLARE_DYNAMIC(DlgProjectNew)

public:
	DlgProjectNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgProjectNew();

// Dialog Data
	enum { IDD = IDD_DLG_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	/**********************************************************************************************//**
	 * \fn	virtual BOOL DlgProjectNew::OnInitDialog();
	 *
	 * \brief	��ʼ�������������н��б���ͼƬ�ĸ�ֵ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_projectName;
	CString m_projectLocation;

	/**********************************************************************************************//**
	 * \fn	afx_msg void DlgProjectNew::OnBnClickedButtonNewBrowse();
	 *
	 * \brief	��Ӧ�����ť
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnBnClickedButtonNewBrowse();

	/**********************************************************************************************//**
	 * \fn	afx_msg void DlgProjectNew::OnBnClickedOk();
	 *
	 * \brief	��Ӧȷ�ϰ�ť
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnBnClickedOk();

private:
	CBrush m_backbrush;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
