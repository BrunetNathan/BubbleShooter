#pragma once


class CBoard
{
private:
	


public:
	int fin_partie(int nb_bubble, int nb_ligne); // fait, TOTEST
	void delete_suspension(int nb_ligne, int nb_bubble);
	void ajout_ligne(int nb_ligne, int nb_bubble, int nb_color); 
	void delete_voisin(int color_bubble,int position, int nb_ligne, int nb_bubble);
	bool contact(int posX,int posY, int nb_ligne, int nb_bubble, int taille_bubble,bool& left_button,int color ,int &position_touche);
	CBoard();
	CBoard(int nb_ligne, int nb_init_ligne, int nb_bubble, int nb_color);
	~CBoard();
	int* m_board;
	double m_taille_hitbox;
	bool IntersectionCercle(int X1, int Y1, int X2, int Y2, int taille_bubble);
	void trouver_voisin(int id_voisin, int racine, int nb_ligne, int nb_bubble);
	
	int* m_board_voisin;
	int* m_board_suspension;
};

