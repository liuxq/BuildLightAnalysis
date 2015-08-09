
// BuildLight.h : main header file for the BuildLight application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBuildLightApp:
// See BuildLight.cpp for the implementation of this class
//

class CBuildLightApp : public CWinAppEx
{
public:
	CBuildLightApp();


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
};

extern CBuildLightApp theApp;
