// COption.cpp : fichier d'implémentation
//

#include "pch.h"
#include "BubbleShooter.h"
#include "COption.h"
#include "afxdialogex.h"


// boîte de dialogue de COption

IMPLEMENT_DYNAMIC(COption, CDialog)

COption::COption(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_nb_ligne(15)
	, m_nb_init_ligne(9)
	, m_nb_bubble(17)
	, m_nb_color(6)
	, m_cadence(10)
{

}

COption::~COption()
{
}

void COption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NBLIGNE, m_nb_ligne);
	DDV_MinMaxInt(pDX, m_nb_ligne, 14, 19);
	DDX_Text(pDX, IDC_NBLIGNEINIT, m_nb_init_ligne);
	DDV_MinMaxInt(pDX, m_nb_init_ligne, 3, 13);
	DDX_Text(pDX, IDC_NBBUBBLE, m_nb_bubble);
	DDV_MinMaxInt(pDX, m_nb_bubble, 10, 25);
	DDX_Text(pDX, IDC_NBBUBBLE, m_nb_bubble);
	DDX_Text(pDX, IDC_NBCOLOR, m_nb_color);
	DDV_MinMaxInt(pDX, m_nb_color, 3, 10);
	DDX_Text(pDX, IDC_CADENCE, m_cadence);
	DDV_MinMaxInt(pDX, m_cadence, 5, 30);
}


BEGIN_MESSAGE_MAP(COption, CDialog)
END_MESSAGE_MAP()


// gestionnaires de messages de COption


