/**********************************************************************************************//**
 * \file	BuildLightAnalysis.h
 *
 * \brief	Main header file for the BuildLightAnalysis application
 **************************************************************************************************/

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

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisApp::OnAppAbout();
	 *
	 * \brief	��Ӧ���ڲ˵��������汾��Ϣ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnAppAbout();
	
	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisApp::OnDoc();
	 *
	 * \brief	����help.doc�����ļ�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnDoc();

	DECLARE_MESSAGE_MAP()
};

extern CBuildLightAnalysisApp theApp;
