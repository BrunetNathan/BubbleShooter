
// BubbleShooter.h : fichier d'en-tête principal de l'application BubbleShooter
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux


// CBubbleShooterApp :
// Consultez BubbleShooter.cpp pour l'implémentation de cette classe
//

class CBubbleShooterApp : public CWinAppEx
{
public:
	CBubbleShooterApp() noexcept;


// Substitutions
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implémentation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBubbleShooterApp theApp;
