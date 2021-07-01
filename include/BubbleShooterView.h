
// BubbleShooterView.h : interface de la classe CBubbleShooterView
//

#pragma once


class CBubbleShooterView : public CView
{
protected: // création à partir de la sérialisation uniquement
	CBubbleShooterView() noexcept;
	DECLARE_DYNCREATE(CBubbleShooterView)

// Attributs
public:
	CBubbleShooterDoc* GetDocument() const;

// Opérations
public:

// Substitutions
public:
	virtual void OnDraw(CDC* pDC);  // substitué pour dessiner cette vue
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implémentation
public:
	virtual ~CBubbleShooterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions générées de la table des messages
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void TracerBubble(CDC* pDC);
	// En fonction de nombre de bulle, avec une fenêtre de 1280x720
	int m_taille_bubble;
	int* m_palette_R;
	int* m_palette_G;
	int* m_palette_B;
	int m_mousseX;// position souris X et Y
	int m_mousseY;
	double m_mousseX_clic;
	double m_mousseY_clic;
	COLORREF m_coul;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// coordonées X et Y du coin supérieur gauche du rectangle de l'élipse
	int m_X1;
	int m_Y1;
	// coordonées X et Y du coin supérieur gauche du rectangle de l'élipse
	int m_X2;
	int m_Y2;
	// coords du centre du la bulle tirée
	int m_posX_bubble;
	int m_posY_bubble;
	void TacerDirection(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void TracerPlateau(CDC* pDC);
	double m_coeff_cercle;
	bool m_left_button;
	int m_id_bubble;
	int m_sens;
	int m_largeur;
	int m_hauteur;
	int m_position_touche;
	int m_ligne_bubble_touche;
	int m_colone_bubble_touche;
	int m_posY_touche;
	int m_posX_touche;
	double m_angle_colision; 
};

#ifndef _DEBUG  // version Debug dans BubbleShooterView.cpp
inline CBubbleShooterDoc* CBubbleShooterView::GetDocument() const
   { return reinterpret_cast<CBubbleShooterDoc*>(m_pDocument); }
#endif

