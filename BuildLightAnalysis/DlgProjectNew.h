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
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_projectName;
	CString m_projectLocation;

	afx_msg void OnBnClickedButtonNewBrowse();
	afx_msg void OnBnClickedOk();

private:
	CBrush m_backbrush;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
