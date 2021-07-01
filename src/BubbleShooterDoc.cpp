
// BubbleShooterDoc.cpp : implémentation de la classe CBubbleShooterDoc
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS peuvent être définis dans les gestionnaires d'aperçu, de miniature
// et de recherche d'implémentation de projet ATL, et permettent le partage de code de document avec ce projet.
#ifndef SHARED_HANDLERS
#include "BubbleShooter.h"
#endif



#include "BubbleShooterDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBubbleShooterDoc

IMPLEMENT_DYNCREATE(CBubbleShooterDoc, CDocument)

BEGIN_MESSAGE_MAP(CBubbleShooterDoc, CDocument)
	ON_COMMAND(ID_BOITEOPTION, &CBubbleShooterDoc::OnBoiteoption)
END_MESSAGE_MAP()


// construction/destruction de CBubbleShooterDoc

CBubbleShooterDoc::CBubbleShooterDoc() noexcept
{
	// TODO: ajoutez ici le code d'une construction unique
	m_partie = NULL;

}

CBubbleShooterDoc::~CBubbleShooterDoc()
{
}

BOOL CBubbleShooterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ajoutez ici le code de réinitialisation
	// (les documents SDI réutiliseront ce document)

	return TRUE;
}




// sérialisation de CBubbleShooterDoc

void CBubbleShooterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ajoutez ici le code de stockage
	}
	else
	{
		// TODO: ajoutez ici le code de chargement
	}
}

#ifdef SHARED_HANDLERS

// Prise en charge des miniatures
void CBubbleShooterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modifier ce code pour dessiner les données du document
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support pour les gestionnaires de recherche
void CBubbleShooterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Définir le contenu de recherche à partir des données du document.
	// Les parties du contenu doivent être séparées par ";"

	// Par exemple :  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBubbleShooterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// diagnostics de CBubbleShooterDoc

#ifdef _DEBUG
void CBubbleShooterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBubbleShooterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// commandes de CBubbleShooterDoc


void CBubbleShooterDoc::OnBoiteoption() // On récupère les données de la boite de dialogue, on les envoie dans le constructeur de la partie
{
	COption dlg; // On créé une boite de dialogue dlg quand le bouton option est cliqué 
	dlg.DoModal();
	m_partie = new CPartie(dlg.m_nb_ligne, dlg.m_nb_init_ligne, dlg.m_nb_bubble, dlg.m_nb_color,dlg.m_cadence); 
	// On initialise la donnée membre m_partie avec les paramètres de la boite de dialogue.
	UpdateAllViews(0);
}

