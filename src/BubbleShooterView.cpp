
// BubbleShooterView.cpp : implémentation de la classe CBubbleShooterView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS peuvent être définis dans les gestionnaires d'aperçu, de miniature
// et de recherche d'implémentation de projet ATL, et permettent le partage de code de document avec ce projet.
#ifndef SHARED_HANDLERS
#include "BubbleShooter.h"
#endif

#include "BubbleShooterDoc.h"
#include "BubbleShooterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBubbleShooterView

IMPLEMENT_DYNCREATE(CBubbleShooterView, CView)

BEGIN_MESSAGE_MAP(CBubbleShooterView, CView)
	// Commandes d'impression standard
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBubbleShooterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// construction/destruction de CBubbleShooterView

CBubbleShooterView::CBubbleShooterView() noexcept
{
	m_X1 = 0;  m_Y1 = 0; m_X2 = 0; m_Y2 = 0; m_posX_bubble = 0; m_posY_bubble = 0;
	m_taille_bubble = 0;
	m_mousseX = 0;
	m_mousseY = 0;
	m_mousseX_clic = 0.0;
	m_mousseY_clic = 0.0;
	// On crée les palettes de couleurs
	m_palette_R = new int[11];
	m_palette_G = new int[11];
	m_palette_B = new int[11];
	m_palette_R[0] = 0; m_palette_R[1] = 255; m_palette_R[2] = 0; m_palette_R[3] = 255; m_palette_R[4] = 255; m_palette_R[5] = 0; m_palette_R[6] = 100; m_palette_R[7] = 26; m_palette_R[8] = 230; m_palette_R[9] = 21; m_palette_R[10] = 0;
	m_palette_G[0] = 0; m_palette_G[1] = 0; m_palette_G[2] = 255; m_palette_G[3] = 255; m_palette_G[4] = 0; m_palette_G[5] = 255; m_palette_G[6] = 50; m_palette_G[7] = 190; m_palette_G[8] = 125; m_palette_G[9] = 125; m_palette_G[10] = 125;
	m_palette_B[0] = 255; m_palette_B[1] = 0; m_palette_B[2] = 0; m_palette_B[3] = 0; m_palette_B[4] = 255; m_palette_B[5] = 255; m_palette_B[6] = 130; m_palette_B[7] = 150; m_palette_B[8] = 30; m_palette_B[9] = 0; m_palette_B[10] = 34;
	m_coul = RGB(0, 0, 0);
	m_coeff_cercle = 1.2;
	m_left_button = false;
	m_sens = 1; // Le sens gère les rebonds
	m_largeur = 0;
	m_hauteur = 0;
	m_ligne_bubble_touche = 0;
	m_colone_bubble_touche = 0;
	m_posY_touche = 0 ;
	m_posX_touche = 0 ;
	m_position_touche = 0;
	m_angle_colision = 0.0;
	m_id_bubble = 0;
}

CBubbleShooterView::~CBubbleShooterView()
{
}

BOOL CBubbleShooterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// dessin de CBubbleShooterView

void CBubbleShooterView::OnDraw(CDC* pDC)
{
	CBubbleShooterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);

	
	if (pDoc->m_partie != NULL) // Si le pointeur vers la partie n'est pas NULL

	{	
		
		m_taille_bubble = min((int)((rect.right-35) / pDoc->m_partie->m_nb_bubble_partie),(int)(rect.bottom / pDoc->m_partie->m_nb_ligne_partie)); 
		// On calcul la taille des bulles en fonctions de la largeur et de la longeur de la fenêtre, les proportions sont gardées avec le min.
		m_largeur = m_taille_bubble* pDoc->m_partie->m_nb_bubble_partie;
		m_hauteur = m_taille_bubble * pDoc->m_partie->m_nb_ligne_partie;
		// Le -35 sert à ce que le bulle de droite décalé d'une demi ligne ne soient pas mangées par le bord de l'écran.
		
		//CPen penBlack;
		//penBlack.CreatePen(PS_SOLID, 10, RGB(20, 20, 20));
		//CPen* pOldPen = pDC->SelectObject(&penBlack);

		CRgn fond; // Permet de délimiter la zone de jeu.

		fond.CreateRectRgn(0, 0, m_taille_bubble*pDoc->m_partie->m_nb_bubble_partie+m_taille_bubble/2 + m_taille_bubble/8, m_taille_bubble * pDoc->m_partie->m_nb_ligne_partie);

		CBrush brush_fond(RGB(50, 0, 50));
		pDC->SelectObject(&brush_fond);
		pDC->FillRgn(&fond, &brush_fond);
		pDC->FrameRgn(&fond, &brush_fond, 3, 3);

		CPen pen10(PS_SOLID, 3, m_coul);
		pDC->SelectObject(&pen10);
		
		
		TracerPlateau(pDC); // On appelle la fonction pour tracer le plateau

		int fin = pDoc->m_partie->pPlateau->fin_partie(pDoc->m_partie->m_nb_bubble_partie, pDoc->m_partie->m_nb_ligne_partie);

		CString str;
		str.Format(_T("Etat : %u"), fin);
		pDC->TextOutW(100 , m_taille_bubble * pDoc->m_partie->m_nb_ligne_partie - 100, str);
		CString etat;
		etat = "Victoire = 2 , Defait = 1, En cours = 0 ";
		pDC->TextOutW(100, m_taille_bubble * pDoc->m_partie->m_nb_ligne_partie - 150,etat);

		// Permet de connaitre l'état de la partie, avec la fonction de CBoard fin_partie()

		SetTimer(1, 20, NULL);
		// On appelle le timer et donc tout les éléments qui dépendent du timer


	}
}

void CBubbleShooterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBubbleShooterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// préparation par défaut
	return DoPreparePrinting(pInfo);
}

void CBubbleShooterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ajoutez une initialisation supplémentaire avant l'impression
}

void CBubbleShooterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ajoutez un nettoyage après l'impression
}

void CBubbleShooterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBubbleShooterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// diagnostics de CBubbleShooterView

#ifdef _DEBUG
void CBubbleShooterView::AssertValid() const
{
	CView::AssertValid();
}

void CBubbleShooterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBubbleShooterDoc* CBubbleShooterView::GetDocument() const // la version non Debug est inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBubbleShooterDoc)));
	return (CBubbleShooterDoc*)m_pDocument;
}
#endif //_DEBUG


// gestionnaires de messages de CBubbleShooterView


void CBubbleShooterView::TracerBubble(CDC* pDC) // Méthode appelée dans le timer, elle trace la bulle, on fonction de sa position, jusqu'à ce que rentre en contat.
{

	CBubbleShooterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);


	if (m_left_button) // Si le joueur a cliqué sur le bouton gauche de la souris
	{
		
		pDoc->m_partie->calcul_angle(m_mousseX_clic, m_mousseY_clic, ((m_X1 + m_X2) / 2), ((m_Y1 + m_Y2) / 2));
		// Calcul de l'angle entre le départ et le clic.

		int X1_temp = (int)(pDoc->m_partie->m_position_bubble_x - m_taille_bubble / 2);
		int Y1_temp = (int)(pDoc->m_partie->m_position_bubble_y - m_taille_bubble / 2);
		int X2_temp = (int)(pDoc->m_partie->m_position_bubble_x + m_taille_bubble / 2);
		int Y2_temp = (int)(pDoc->m_partie->m_position_bubble_y + m_taille_bubble / 2);

		// Positions temporaires pour tracer la bulle

		CRgn draw_bubble_erase;

		draw_bubble_erase.CreateEllipticRgn(X1_temp, Y1_temp, X2_temp, Y2_temp);

		CBrush brush_blanc(RGB(130, 0, 50));
		pDC->SelectObject(&brush_blanc);
		pDC->FillRgn(&draw_bubble_erase, &brush_blanc);
		pDC->FrameRgn(&draw_bubble_erase, &brush_blanc, 3, 3);

		pDoc->m_partie->trajectoire(pDoc->m_partie->m_position_bubble_x, pDoc->m_partie->m_position_bubble_y, m_sens, rect.top, m_taille_bubble * (pDoc->m_partie->m_nb_bubble_partie), m_taille_bubble, (pDoc->m_partie->m_angle) * m_sens);

		
		bool contact = (pDoc->m_partie->pPlateau->contact(pDoc->m_partie->m_position_bubble_x, pDoc->m_partie->m_position_bubble_y, pDoc->m_partie->m_nb_ligne_partie, pDoc->m_partie->m_nb_bubble_partie, m_taille_bubble, m_left_button, pDoc->m_partie->m_color_bubble, m_position_touche));
		// On appelle la méthode contact pour savoir si à cette position, ce trouve un bulle du plateau

		if (!contact) // Tant que la bulle ne touche rien, on la dessine 
		{
			CRgn draw_bubble_shot;

			m_coul = RGB(m_palette_R[pDoc->m_partie->m_color_bubble], m_palette_G[pDoc->m_partie->m_color_bubble], m_palette_B[pDoc->m_partie->m_color_bubble]);

			X1_temp = (int)(pDoc->m_partie->m_position_bubble_x - m_taille_bubble / 2);
			Y1_temp = (int)(pDoc->m_partie->m_position_bubble_y - m_taille_bubble / 2);
			X2_temp = (int)(pDoc->m_partie->m_position_bubble_x + m_taille_bubble / 2);
			Y2_temp = (int)(pDoc->m_partie->m_position_bubble_y + m_taille_bubble / 2);

			draw_bubble_shot.CreateEllipticRgn(X1_temp, Y1_temp, X2_temp, Y2_temp);
			CBrush brush2(m_coul);

			pDC->SelectObject(&brush2);
			pDC->FillRgn(&draw_bubble_shot, &brush2);
			pDC->FrameRgn(&draw_bubble_shot, &brush2, 3, 3);

		}
		else // Si il y contact 
		{
			
			m_ligne_bubble_touche = m_position_touche / pDoc->m_partie->m_nb_bubble_partie;
			m_colone_bubble_touche = m_position_touche%(pDoc->m_partie->m_nb_bubble_partie);

			//int posX_touche = colone_bubble_touche * m_taille_bubble + m_taille_bubble / 2 + (ligne_bubble_touche % 2)* m_taille_bubble;

			m_posY_touche = m_ligne_bubble_touche * (m_taille_bubble*(1-1/8)) + m_taille_bubble/2;
			m_posX_touche = m_colone_bubble_touche*m_taille_bubble+ m_taille_bubble / 2 + (m_ligne_bubble_touche % 2)*m_taille_bubble;
			
			double diffX = (double)(-pDoc->m_partie->m_position_bubble_x + m_posX_touche); // On calcul l'angle entre les deux centre des cercles deus bulles qui se sont touchés
			double diffY = (double)(pDoc->m_partie->m_position_bubble_y - m_posY_touche);
			m_angle_colision = atan(diffX /diffY);

			bool place = false;

			// Toutes les possiblités, on fonction de l'angle d'incidence et des position possible pour la nouvelle bulle
			if (pDoc->m_partie->pPlateau->m_board[m_position_touche - 1] == -1 && m_angle_colision < -(3.141592) / 6 && m_angle_colision > -(3.141592) / 2 && !place)
			{
				pDoc->m_partie->pPlateau->m_board[m_position_touche - 1] = pDoc->m_partie->m_color_bubble;
				m_id_bubble = m_position_touche - 1;
				place = true;
			}
			// Repli 1
			else if (pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie - 1 + (m_ligne_bubble_touche % 2)] == -1 && m_angle_colision < 0 && !place)
			{
				pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie - 1 + (m_ligne_bubble_touche % 2)] = pDoc->m_partie->m_color_bubble;
				m_id_bubble = m_position_touche + pDoc->m_partie->m_nb_bubble_partie - 1 + (m_ligne_bubble_touche % 2);
				place = true;
			}
			if (pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie - 1 + (m_ligne_bubble_touche % 2)] == -1 && m_angle_colision > -(3.141592) / 6 && m_angle_colision < 0 && !place)
			{
				pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie - 1 + (m_ligne_bubble_touche % 2)] = pDoc->m_partie->m_color_bubble;
				m_id_bubble = m_position_touche + pDoc->m_partie->m_nb_bubble_partie - 1 + (m_ligne_bubble_touche % 2);
				place = true;
			}
			if (pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie + (m_ligne_bubble_touche % 2)] == -1 && m_angle_colision < (3.141592) / 6 && m_angle_colision > 0 && !place)
			{
				pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie + (m_ligne_bubble_touche % 2)] = pDoc->m_partie->m_color_bubble;
				m_id_bubble = m_position_touche + pDoc->m_partie->m_nb_bubble_partie + (m_ligne_bubble_touche % 2);
				place = true;
			}
			if (pDoc->m_partie->pPlateau->m_board[m_position_touche + 1] == -1 && m_angle_colision < (3.141592) / 2 && m_angle_colision >(3.141592) / 6 && !place)
			{
				pDoc->m_partie->pPlateau->m_board[m_position_touche + 1] = pDoc->m_partie->m_color_bubble;
				m_id_bubble = m_position_touche + 1;
				place = true;
			}
			// Repli 2
			else if (pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie + (m_ligne_bubble_touche % 2)] == -1 && m_angle_colision > 0 && !place)
			{
				pDoc->m_partie->pPlateau->m_board[m_position_touche + pDoc->m_partie->m_nb_bubble_partie + (m_ligne_bubble_touche % 2)] = pDoc->m_partie->m_color_bubble;
				m_id_bubble = m_position_touche + pDoc->m_partie->m_nb_bubble_partie + (m_ligne_bubble_touche % 2);
				place = true;
			}
			
			if (pDoc->m_partie->m_position_bubble_y < 0) // Si la bulle touche le haut
			{
				pDoc->m_partie->pPlateau->m_board[pDoc->m_partie->m_position_bubble_x/m_taille_bubble] = pDoc->m_partie->m_color_bubble;
				place = true;
			}
			
			if (place) // Chaque bulle place, incrémente le nombre de bulle tirée
			{
				pDoc->m_partie->m_nb_bubble_tiree += 1 ;
			}
			pDoc->m_partie->pPlateau->delete_voisin(pDoc->m_partie->m_color_bubble, m_id_bubble, pDoc->m_partie->m_nb_ligne_partie, pDoc->m_partie->m_nb_bubble_partie);
			pDoc->m_partie->pPlateau->delete_suspension(pDoc->m_partie->m_nb_ligne_partie, pDoc->m_partie->m_nb_bubble_partie);
			
			m_left_button = false; // Une fois que le bulle est arrivée, on repasse en mode tir

			RedrawWindow(0); //On fait un redraw pour redessiner le plateau 
			pDoc->m_partie->m_color_bubble = rand() % pDoc->m_partie->m_nb_color_partie; // On tire la couleur de la nouvelle bulle
		}
	}
	else // Si le joueur n'a pas encore cliqué, on trace la bulle au départ
	{
		m_coul = RGB(m_palette_R[pDoc->m_partie->m_color_bubble], m_palette_G[pDoc->m_partie->m_color_bubble], m_palette_B[pDoc->m_partie->m_color_bubble]);
		CRgn draw_bubble_shot;

		draw_bubble_shot.CreateEllipticRgn(m_X1, m_Y1, m_X2, m_Y2);
		CBrush brush2(m_coul);

		pDC->SelectObject(&brush2);
		pDC->FillRgn(&draw_bubble_shot, &brush2);
		pDC->FrameRgn(&draw_bubble_shot, &brush2, 3, 3);

		pDoc->m_partie->m_position_bubble_x = (int)((m_X1 + m_X2) / 2);
		pDoc->m_partie->m_position_bubble_y = (int)((m_Y1 + m_Y2) / 2);

	}

}

void CBubbleShooterView::OnMouseMove(UINT nFlags, CPoint point)
{

	m_mousseX = point.x;
	m_mousseY = point.y;
	CView	::OnMouseMove(nFlags, point); // Permet de connaitre la position de la souris pour tracer la trait de trajectoire
	
}


void CBubbleShooterView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	CDC* pDC = GetDC();

	CView::OnTimer(nIDEvent);
	
	
	TacerDirection(pDC); // On appelle les fonctions qui dépendent du timer.
	TracerBubble(pDC);

	ReleaseDC(pDC); // On évite les fuites mémoires


}


void CBubbleShooterView::TacerDirection(CDC* pDC) // Permet de tracer le trait de visée
{
	
		
	CBubbleShooterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);
	CRgn draw_clean;

	m_X1 = (m_taille_bubble * (pDoc->m_partie->m_nb_bubble_partie)) / 2 - m_taille_bubble / 2.0;
	m_Y1 = (m_taille_bubble*pDoc->m_partie->m_nb_ligne_partie - m_taille_bubble);
	m_X2 = ((m_taille_bubble * (pDoc->m_partie->m_nb_bubble_partie)) /2 + m_taille_bubble / 2.0 );
	m_Y2 = (m_taille_bubble *pDoc->m_partie->m_nb_ligne_partie);

	int X1_blanc_temp = m_X1 - m_taille_bubble * (m_coeff_cercle);
	int Y1_blanc_temp = m_Y1 - m_taille_bubble * (m_coeff_cercle) ;
	int X2_blanc_temp = m_X2 + m_taille_bubble * (m_coeff_cercle);
	int Y2_blanc_temp = m_Y2 + m_taille_bubble * (m_coeff_cercle);

	draw_clean.CreateEllipticRgn(X1_blanc_temp, Y1_blanc_temp, X2_blanc_temp, Y2_blanc_temp);
	CBrush brush3(RGB(50,0,50));

	pDC->SelectObject(&brush3);
	pDC->FillRgn(&draw_clean, &brush3);

	CPen pen0(PS_SOLID, 5, RGB(220, 230, 150));
	CPen* poldpen = pDC->SelectObject(&pen0);
	
	pDoc->m_partie->calcul_angle(m_mousseX, m_mousseY, (m_X1 + m_X2) / 2, (m_Y1 + m_Y2) / 2); // Modifie sa donnée membre m_angle, qu'on utilise juste après. 

	double coord_directionX = (m_X1 + m_X2) / 2 + m_taille_bubble* m_coeff_cercle *cos(-(pDoc->m_partie->m_angle)-(3.141592)/2);
	double coord_directionY = (m_Y1 + m_Y2) / 2 + m_taille_bubble* m_coeff_cercle *sin(-(pDoc->m_partie->m_angle)-(3.141592)/2);

	pDC->MoveTo((m_X1 + m_X2) / 2, (m_Y1 + m_Y2) / 2);
	pDC->LineTo(coord_directionX, coord_directionY);
	}


void CBubbleShooterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Quand le joueur clique sur le bouton gauche de la souris, on enregistre la position de la souris, et on passe en mode dessin de la bulle tirée
	
	if (!m_left_button)
	{
		m_mousseX_clic = point.x;
		m_mousseY_clic = point.y;
		m_left_button = true;
		m_sens = 1; 
	}
	CView::OnLButtonDown(nFlags, point);
}


void CBubbleShooterView::TracerPlateau(CDC* pDC)
{
	// Chaque bulle du plateau est tracé en fonction de sa couleur.
	CBubbleShooterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	int decalage = 5 + m_taille_bubble / 2;

	CRgn draw_bubble_shot;

	if (pDoc->m_partie->m_nb_bubble_tiree % pDoc->m_partie->m_cadence == 0)
	{
		pDoc->m_partie->pPlateau->ajout_ligne(pDoc->m_partie->m_nb_ligne_partie, pDoc->m_partie->m_nb_bubble_partie, pDoc->m_partie->m_nb_color_partie);
	}

	for (int i = 0; i < pDoc->m_partie->m_nb_ligne_partie; ++i)
	{
		
		for (int j = 0; j < pDoc->m_partie->m_nb_bubble_partie; ++j)
		{
			if (pDoc->m_partie->pPlateau->m_board[i * (pDoc->m_partie->m_nb_bubble_partie) + j]!=-1)
			{
				CRgn draw_bubble_shot;
				if (i % 2 == 1)
				{
					int X1 = (int)(j * m_taille_bubble - m_taille_bubble / 2.0 + (decalage + m_taille_bubble / 2.0));
					int Y1 = (int)(i * m_taille_bubble - m_taille_bubble / 2.0 + decalage - (m_taille_bubble / 8.0) * i); // -taille_bubble / 8 sert à ce que les billes soient encastrées les unes dans les autres. 
					int X2 = (int)(j * m_taille_bubble + m_taille_bubble / 2.0 + (decalage + m_taille_bubble / 2.0));
					int Y2 = (int)(i * m_taille_bubble + m_taille_bubble / 2.0 + decalage - (m_taille_bubble / 8.0) * i);
					draw_bubble_shot.CreateEllipticRgn(X1, Y1, X2, Y2);
				}
				else
				{
					int X1 = (int)(j * m_taille_bubble - m_taille_bubble / 2.0 + decalage);
					int Y1 = (int)(i * m_taille_bubble - m_taille_bubble / 2.0 + decalage - (m_taille_bubble / 8.0) * i);
					int X2 = (int)(j * m_taille_bubble + m_taille_bubble / 2.0 + decalage);
					int Y2 = (int)(i * m_taille_bubble + m_taille_bubble / 2.0 + decalage - (m_taille_bubble / 8.0) * i);
					draw_bubble_shot.CreateEllipticRgn(X1, Y1, X2, Y2);
				}

				m_coul = RGB(m_palette_R[pDoc->m_partie->pPlateau->m_board[i * (pDoc->m_partie->m_nb_bubble_partie) + j]], m_palette_G[pDoc->m_partie->pPlateau->m_board[i * (pDoc->m_partie->m_nb_bubble_partie) + j]], m_palette_B[pDoc->m_partie->pPlateau->m_board[i * (pDoc->m_partie->m_nb_bubble_partie) + j]]);
				CBrush brush2(m_coul);
				pDC->SelectObject(&brush2);
				pDC->FillRgn(&draw_bubble_shot, &brush2);
			}
		}

	}

}



