#include "pch.h"
#include "CPartie.h"



void CPartie::trajectoire(int& position_bubble_x, int& position_bubble_y, int& sens,int top, int right, int taille_bubble,double angle)
{
// TODO Voir ce qu'on fait si la bille touche la plafond avec l'int top)	
	
	
	if (((position_bubble_x - taille_bubble / 2) <= 0 ) ) // Si ça touche le bord de gauche, on inverse le signe de sens 
	{
		sens = -sens;

		position_bubble_x += 50;
		
	}
	else if ((position_bubble_x + taille_bubble / 2) >= right) // Si ça touche le bord de droite, on inverse le signe de sens 
	{
		sens = -sens; 
		position_bubble_x -= 50;
		
	}
	else // Sinon la bulle continue sa trajectoire selon l'angle de départ 
	{
		position_bubble_x = position_bubble_x - sin(angle) * m_vitesse;
		position_bubble_y = position_bubble_y - cos(angle) * m_vitesse;
	}

}

CPartie::CPartie() // Jamais utilsé en pratique 
{
	m_nb_ligne_partie = 0;
	m_nb_init_ligne_partie = 0;
	m_nb_bubble_partie = 0;
	m_nb_color_partie = 0;
	m_score = 0;
	m_color_bubble = 0;
	m_nb_bubble_tiree = 0;
	m_angle = 0; 
	m_rebond = 0;
	m_position_bubble_x = 0; 
	m_position_bubble_y = 0; 
	pPlateau = new CBoard(0, 0, 0, 0);
	m_cadence = 0;
}

CPartie::CPartie(int nb_ligne, int nb_init_ligne, int nb_bubble, int nb_color,int cadence) 
// On récupère les données de la boite de dialogue pour itialisé les données membres de la partie 
{
	m_nb_ligne_partie = nb_ligne;
	m_nb_init_ligne_partie = nb_init_ligne - 1; // -1 car au début nb_bubble_tire % cadence == 0 
	m_nb_bubble_partie = nb_bubble;
	m_nb_color_partie = nb_color;
	m_score = 0;  // Système de score non implémenté
	m_color_bubble = rand() % nb_color;  // On tire une première couleur de bulle aléatoire 
	m_nb_bubble_tiree = 0;
	m_angle = 0;
	m_rebond = 0;
	m_position_bubble_x = 3;  
	m_position_bubble_y = 0; 
	pPlateau = new CBoard(nb_ligne, nb_init_ligne, nb_bubble, nb_color); // On initialise le board avec son constructeur, en lui passant les données de la boite de dialogue 
	m_cadence = cadence;

}

void CPartie::calcul_angle(int mousseX, int mousseY, int bubbleX, int bubbleY) 

// Méthode qui calcul l'angle entre deux points : on l'utilse pour calculer entre le point de départ et la souris 
{
	double adj = (mousseY - bubbleY); 
	double oppose = (mousseX - bubbleX);
	m_angle = atan(oppose / adj);

}
