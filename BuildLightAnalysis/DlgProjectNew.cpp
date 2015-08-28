// DlgProjectNew.cpp : implementation file
//

#include "stdafx.h"
#include "BuildLightAnalysis.h"
#include "DlgProjectNew.h"
#include "afxdialogex.h"


// DlgProjectNew dialog

IMPLEMENT_DYNAMIC(DlgProjectNew, CDialogEx)

DlgProjectNew::DlgProjectNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgProjectNew::IDD, pParent)
	, m_projectName(_T(""))
	, m_projectLocation(_T(""))
{

}

DlgProjectNew::~DlgProjectNew()
{
}

void DlgProjectNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEW_NAME, m_projectName);
	DDX_Text(pDX, IDC_EDIT_NEW_LOCATION, m_projectLocation);
}


BEGIN_MESSAGE_MAP(DlgProjectNew, CDialogEx)
END_MESSAGE_MAP()


// DlgProjectNew message handlers
