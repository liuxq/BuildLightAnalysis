
// BuildLightAnalysis.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BuildLightAnalysis.h"
#include "MainFrm.h"

#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <Gdiplus.h>
using namespace Gdiplus;

GdiplusStartupInput    m_Gdistart; 
ULONG_PTR    m_GdiplusToken;  //保存GDI+被初始化后在应用程序中的GDI+标识,类似于句柄

// CBuildLightAnalysisApp

BEGIN_MESSAGE_MAP(CBuildLightAnalysisApp, CWinAppEx)
	ON_COMMAND(ID_ABOUT, &CBuildLightAnalysisApp::OnAppAbout)
	// Standard file based document commands
END_MESSAGE_MAP()


// CBuildLightAnalysisApp construction

CBuildLightAnalysisApp::CBuildLightAnalysisApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("BuildLightAnalysis.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CBuildLightAnalysisApp object

CBuildLightAnalysisApp theApp;


// CBuildLightAnalysisApp initialization

BOOL CBuildLightAnalysisApp::InitInstance()
{
	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBuildLightAnalysisDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBuildLightAnalysisView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	GdiplusStartup(&m_GdiplusToken, &m_Gdistart,NULL);

	return TRUE;
}

int CBuildLightAnalysisApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	GdiplusShutdown(m_GdiplusToken);

	return CWinAppEx::ExitInstance();
}

// CBuildLightAnalysisApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CBuildLightAnalysisApp::OnAppAbout()
{
	/*CAboutDlg aboutDlg;
	aboutDlg.DoModal();*/
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (pMain)
		ShellExecute(pMain->m_hWnd, L"open",L"help.doc" ,NULL, NULL, SW_SHOWDEFAULT);
}

// CBuildLightAnalysisApp customization load/save methods

void CBuildLightAnalysisApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	GetContextMenuManager()->AddMenu(_T("房间"), IDR_MENU_ROOM);
}

void CBuildLightAnalysisApp::LoadCustomState()
{
}

void CBuildLightAnalysisApp::SaveCustomState()
{
}

// CBuildLightAnalysisApp message handlers



