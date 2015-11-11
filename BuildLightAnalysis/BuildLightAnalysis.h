
// BuildLightAnalysis.h : main header file for the BuildLightAnalysis application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBuildLightAnalysisApp:
// See BuildLightAnalysis.cpp for the implementation of this class
//

class CBuildLightAnalysisApp : public CWinAppEx
{
public:
	CBuildLightAnalysisApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDoc();
};

extern CBuildLightAnalysisApp theApp;
