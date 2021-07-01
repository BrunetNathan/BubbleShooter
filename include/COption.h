#pragma once


// boîte de dialogue de COption

class COption : public CDialog
{
	DECLARE_DYNAMIC(COption)

public:
	COption(CWnd* pParent = nullptr);   // constructeur standard
	virtual ~COption();

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_nb_ligne;
	int m_nb_init_ligne;
	int m_nb_bubble;
	int m_nb_color;
	int m_cadence;
};
