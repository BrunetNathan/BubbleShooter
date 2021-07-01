#pragma once

#include "CBoard.h"

class CPartie
{
	//int* pPlateau;

public:
	int m_score;
	int m_color_bubble; // Couleur de la bulle en cours
	int m_nb_bubble_tiree; // Sert pour ajouter des lignes suivant la cadence
	int m_position_bubble_x;
	int m_position_bubble_y;
	int m_nb_ligne_partie; // Initialisé selon la boite de dialogue 
	int m_nb_init_ligne_partie; // Initialisé selon la boite de dialogue 
	int m_nb_bubble_partie; // Initialisé selon la boite de dialogue 
	int m_nb_color_partie; // Initialisé selon la boite de dialogue 
	double m_angle;
	CPartie(); // Constructeur sans paramètres // TODO faire la construction de toutes les données membres 
	CPartie(int, int, int, int,int); // Constructeur avec paramètres
	void trajectoire(int& position_bubble_x, int& position_bubble_y, int& sens, int top, int right, int taille_bubble,double angle); 	
	CBoard *pPlateau; // Pointeur vers le plateau qui contient les positions des bulles
	void calcul_angle(int mousseX, int mousseY, int bubbleX, int bubbleY);
	int m_vitesse = 24; // TODO Adapter la vitesse à la taille de l'écran.
	int m_rebond;
	int m_cadence; // Initialisé selon la boite de dialogue 
};

