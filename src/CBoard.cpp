#include "pch.h"
#include "CBoard.h"

int CBoard::fin_partie(int nb_bubble,int nb_ligne)
{
    // 0 partie pas finie , 1 partie perdue , 2 partie gagn?e.
    int fin = 2;
    for (int i = 0; i < nb_bubble; ++i)
    {
        if (m_board[i] != 0) // Si une bulle touche le plafond, la partie n'est pas gagn?e
        {
            fin = 0;
        }
    }
    for (int j = (nb_ligne-1)*nb_bubble; j < nb_bubble; ++j)
        if (m_board[j] != 0) // Si un bulle est sur la derni?re lign, la partie est perdue
            fin = 1;
    
    return fin;
}

void CBoard::delete_suspension(int nb_ligne, int nb_bubble)
{
    for (int i = 0; i < (nb_ligne + 1) * nb_bubble; ++i) // Le tableau de suspension devient vierge
        m_board_suspension[i] = 0;
    for (int i = 0; i < nb_bubble; ++i) // Sauf pour les bulles de la premi?re ligne non vides qui seront des racines
        if (m_board[i] != -1)
            m_board_suspension[i] = 1;
    

    bool fini = false; 
    while (!fini)
    {
        fini = true; // On passe fini ? true 
        for (int i = 0; i < nb_ligne * nb_bubble; ++i)
            if (m_board_suspension[i] == 1) // Si l'un des ?l?ments du tableau est ? 1, alors l'exploration n'est pas finie (on repasse ? false)
                fini = false;
        for (int i = 0; i< nb_ligne * nb_bubble; ++i)
            if (m_board_suspension[i] == 1) // On explore de proche en proche
            {
                trouver_voisin(-1, i, nb_ligne, nb_bubble);
                break; 
            }
        

    }
    for (int i = 0; i < nb_bubble * nb_ligne; ++i) // Toutes les bulles non visit?es ne touchent pas le plafond, elles sont supprim?es
        if (m_board_suspension[i] != 2)
            m_board[i] = -1;       
}

void CBoard::ajout_ligne(int nb_ligne,int nb_bubble,int nb_color) 
{


    for (int i = nb_bubble * (nb_ligne - 1); i > nb_bubble ; --i) // On copie par le bas la bulle du dessus pour chaque bulle du plateau (sauf la premi?re ligne)
    {
        m_board[i] = m_board[i - nb_bubble];
    }
    for (int i = 0; i < nb_bubble; ++i) // On remplace la premi?re ligne par des nouvelles bulles al?atoires
    {
        m_board[i] = rand() % nb_color;
    }
    
}

void CBoard::delete_voisin(int color_bubble,int position,int nb_ligne,int nb_bubble) //Doit prendre en compte la position ou est arriv?e la bulle 
{
    int compteur = 0; // On initialise un compteur ? 0, il faut trois bulles minimum pour ?liminer des voisins
    for (int i = 0; i < nb_ligne*nb_bubble; ++i) // Le tableau des voisins devient vierge
    
        m_board_voisin[i] = 0;
    
    m_board_voisin[position] = 1; // On cr?e une racine ? 1 (o? la bille ? touch?) qui permettra de chercher les voisins 
    bool fini = false; // Permet de savoir si l'exploration des voisins est finie 
    while (!fini) // Tant que c'est pas fini, on continue de chercher
    {
        fini = true; // On passe fini ? true 
        for (int i = 0; i < nb_ligne * nb_bubble; ++i)
            if (m_board_voisin[i] == 1) // Si l'un des ?l?ments du tableau est ? 1, alors l'exploration n'est pas finie (on repasse ? false)
                fini = false;  

        for (int i = 0; i < nb_ligne * nb_bubble; ++i)
        {
            if (m_board_voisin[i] == 1) // Tous les ?lements ? 1 sont de la bonne couleur, il faut explorer ses voisins
            {
                trouver_voisin(0,i,nb_ligne, nb_bubble); // On chercher ses voisins 
                break;
            }
        }
    }
    for (int i = 0; i < nb_ligne * nb_bubble; ++i)
        if (m_board_voisin[i] == 2)  // Si une bulle est ajout?e au cluster, le compteur est augment?
            compteur += 1;  

    if (compteur >= 3) // Si plus de trois bulles sont adjacentes de la m?me couleur
    {
        for (int i = 0; i < nb_ligne * nb_bubble; ++i)
        {
            if (m_board_voisin[i] == 2) // Toutes les positions ? 2 sont de la bonne couleur,  et sont entour?s de bulles de couleurs diff?rente
            {
                m_board[i] = -1;
            }
        }
    }

}

bool CBoard::contact(int posX, int posY, int nb_ligne, int nb_bubble, int taille_bubble, bool& left_button, int color_bubble, int &position_touche)  // Doit prendre position_bulle_courante
{
    position_touche = 0; 
    bool contact = false; // De base le contact est faux
    int decalage = 5 + taille_bubble / 2 ;

    if (posY < 0)
    {
        contact = true; // Si le bulle sort de l'?cran par le haut, il y a contact
        return contact; 
    }
    
    for (int i = 0; i < nb_ligne; ++i)
    {
        for (int j = 0; j < nb_bubble; ++j) 
            // On test toutes les bulles non vides du plateau pour voir s'il y un contact avec la positon courante de la bulle 
        {
            int X1 = 0; int Y1 = 0; int X2 = 0; int Y2 = 0;
            if (m_board[i * nb_bubble + j] != -1)
            {
                position_touche = i * nb_bubble + j;
                if (i % 2 == 1) // La parti? de la ligne infulence la position de la bulle
                {
                    X1 = (int)(j * taille_bubble - taille_bubble / 2.0 + (decalage + taille_bubble / 2.0));
                    Y1 = (int)(i * taille_bubble - taille_bubble / 2.0 + decalage - (taille_bubble / 8.0) * i); // -taille_bubble / 8 sert ? ce que les billes soient encastr?es les unes dans les autres. 
                    X2 = (int)(j * taille_bubble + taille_bubble / 2.0 + (decalage + taille_bubble / 2.0));
                    Y2 = (int)(i * taille_bubble + taille_bubble / 2.0 + decalage - (taille_bubble / 8.0) * i);

                }
                else
                {
                    X1 = (int)(j * taille_bubble - taille_bubble / 2.0 + decalage);
                    Y1 = (int)(i * taille_bubble - taille_bubble / 2.0 + decalage - (taille_bubble / 8.0) * i);
                    X2 = (int)(j * taille_bubble + taille_bubble / 2.0 + decalage);
                    Y2 = (int)(i * taille_bubble + taille_bubble / 2.0 + decalage - (taille_bubble / 8.0) * i);

                }

                contact = IntersectionCercle(posX, posY, (X1 + X2) / 2, (Y1 + Y2) / 2, taille_bubble); 
                // On test l'intersection des cercles pour chaque bulle pr?sente sur le plateau

                if (contact) // En cas de contact, on repasse en mode vis?e (left_button = false;) et on revoie true dans le view.
                {                    
                    left_button = false;
                    return contact;

                }
            }

        }
    }
    
    return contact;
}

CBoard::CBoard()
{
    m_board = NULL;
    m_board_voisin = NULL;
    m_board_suspension = NULL;
    m_taille_hitbox = 0.85;
}

CBoard::CBoard(int nb_ligne, int nb_init_ligne, int nb_bubble, int nb_color) // Constructeur appel? dans le conctrcuter de CPartie
{
    m_board = new int[(nb_ligne + 1) * nb_bubble]; // On initialise tout nos plateaux. 
    m_board_voisin = new int[(nb_ligne + 1) * nb_bubble];
    m_board_suspension = new int[(nb_ligne + 1) * nb_bubble];

    for (int i = 0; i < nb_bubble * nb_init_ligne; ++i)
    {
        m_board[i] = rand() % nb_color; // O naffecte ue couleur al?atoire au bulles de d?part. 
        m_board_voisin[i] = 0;
        m_board_suspension[i] = 0;
    }
    for (int j = nb_bubble * nb_init_ligne; j < (nb_ligne + 1) * nb_bubble; j++)
    {
        m_board[j] = -1 ;
        m_board_voisin[j] = 0;
        m_board_suspension[j] = 0;
    }

    m_taille_hitbox = 0.85; // Sert dans intersection de cercle.
}



CBoard::~CBoard()
{
}


bool CBoard::IntersectionCercle(int X1, int Y1, int X2, int Y2, int taille_bubble)
{
    int dx = X1 - X2;
    int dy = Y1 - Y2;
    double len = sqrt(dx * dx + dy * dy); // Distance entre les centre des cercles 

    if (len < (taille_bubble) * m_taille_hitbox) // La hitbox est une donn?e membre pour ne pas ?tre ?crite en dure dans le programme
        // En th?orie, elle devrait ?tre de 1, mais le jeu est plus agr?able si on la dimiunue, ce qui permet de faire des tir plus pr?cis
        // sans que la bulle se bloque. En effet la bulle pourra plus facilement "se faufiler".
    {
        return true; // Si la distance est assez petite, il y contact. 
    }

    return false;
}


void CBoard::trouver_voisin(int id_voisin,int racine, int nb_ligne, int nb_bubble)

// Pour trouver les voisins, on explore de proche en proche. Une racine dont les voisins sont ? explorer aura la valeur 1
// Une bulle du cluster et dont les voisins ont ?t? explor?s aura la valeur 2
// Une bulle non visit? aura la valeur 0 
// Tant qu'une bulle a la valeur 1, i'exploration n'est pas finie
// Quand il ne reste plus de bulle dans l'?tat 1, les bulles 2 sont dans le m?me cluster, les autres non.
{
    if (id_voisin == -1) // On traite la suspension
    {
        int n = (racine / nb_bubble) % 2;
        m_board_suspension[racine] = 2;
        switch (n)
        {
        case 0: // Ligne paire
            if (racine > nb_bubble && racine % nb_bubble > 0) // En haut ? gauche, on v?rifie que le bulle peut exister (on ?limine la premi?re ligne et la premi?re bulle de chaque ligne)
                if (m_board[racine - nb_bubble - 1] != -1 && m_board_suspension[racine - nb_bubble - 1] == 0)
                    m_board_suspension[racine - nb_bubble - 1] = 1;

            if (racine >= nb_bubble) // En haut ? Droite, on v?rifie que le bulle peut exister (on ?limine la premi?re ligne)
                if (m_board[racine - nb_bubble] != -1 && m_board_suspension[racine - nb_bubble] == 0)
                    m_board_suspension[racine - nb_bubble] = 1;

            if (racine % nb_bubble > 0) // ? gauche (on ?limine la premi?re bulle de chaque ligne)
                if (m_board[racine - 1] != -1 && m_board_suspension[racine - 1] == 0)
                    m_board_suspension[racine - 1] = 1;

            if (racine % nb_bubble < (nb_bubble - 1)) // ? droite (on ?limine la derni?re bulle de chaque ligne)
                if (m_board[racine + 1] != -1 && m_board_suspension[racine + 1] == 0)
                    m_board_suspension[racine + 1] = 1;

            if (racine < nb_bubble * (nb_ligne - 1) && racine % nb_bubble > 0) // En bas ? gauche, on v?rifie que le bulle peut exister (on ?limine la derni?re ligne et la premi?re bulle de chaque ligne)
                if (m_board[racine + nb_bubble - 1] != -1 && m_board_suspension[racine + nb_bubble - 1] == 0)
                    m_board_suspension[racine + nb_bubble - 1] = 1;

            if (racine < nb_bubble * (nb_ligne - 1)) // En bas ? droite, on v?rifie que le bulle peut exister (on ?limine la derni?re ligne)
                if (m_board[racine + nb_bubble] != -1 && m_board_suspension[racine + nb_bubble] == 0)
                    m_board_suspension[racine + nb_bubble] = 1;

            break;
        case 1:  // Ligne impaire
            if (racine >= nb_bubble) // En haut ? gauche, on v?rifie que le bulle peut exister
                if (m_board[racine - nb_bubble] != -1 && m_board_suspension[racine - nb_bubble] == 0)
                    m_board_suspension[racine - nb_bubble] = 1;

            if (racine >= nb_bubble && racine % nb_bubble < (nb_bubble - 1)) // En haut ? Droite, on v?rifie que le bulle peut exister
                if (m_board[racine - nb_bubble + 1] != -1 && m_board_suspension[racine - nb_bubble + 1] == 0)
                    m_board_suspension[racine - nb_bubble + 1] = 1;

            if (racine % nb_bubble > 0) // ? gauche
                if (m_board[racine - 1] != -1 && m_board_suspension[racine - 1] == 0)
                    m_board_suspension[racine - 1] = 1;

            if (racine % nb_bubble < (nb_bubble - 1)) // ? droite
                if (m_board[racine + 1] != -1 && m_board_suspension[racine + 1] == 0)
                    m_board_suspension[racine + 1] = 1;

            if (racine < nb_bubble * (nb_ligne - 1)) // En bas ? gauche, on v?rifie que le bulle peut exister
                if (m_board[racine + nb_bubble] != -1 && m_board_suspension[racine + nb_bubble] == 0)
                    m_board_suspension[racine + nb_bubble] = 1;

            if (racine < nb_bubble * (nb_ligne - 1) && racine % nb_bubble < (nb_bubble - 1)) // En bas ? droite, on v?rifie que le bulle peut exister
                if (m_board[racine + nb_bubble + 1] != -1 && m_board_suspension[racine + nb_bubble + 1] == 0)
                    m_board_suspension[racine + nb_bubble + 1] = 1;

            break;

        }
        
    } 
    else // On traite les voisins de m?me couleur 
    {
        int n = (racine / nb_bubble) % 2; // Les voisins disponibles sont diff?rents entre les lignes pairs et impairs
        m_board_voisin[racine] = 2;  // En fonction de la parti? de la ligne, les bulles adajacentes diff?rent.
        switch (n) // TODO Faire ?a bien 
        {
        case 0: // Ligne paire
            if (racine > nb_bubble && racine % nb_bubble > 0) // En haut ? gauche, on v?rifie que le bulle peut exister (on ?limine la premi?re ligne et la premi?re bulle de chaque ligne)
                if (m_board[racine - nb_bubble -1] == m_board[racine] && m_board_voisin[racine - nb_bubble -1] == 0)
                    m_board_voisin[racine - nb_bubble -1] = 1;
            
            if (racine >= nb_bubble) // En haut ? Droite, on v?rifie que le bulle peut exister (on ?limine la premi?re ligne)
                if (m_board[racine - nb_bubble ] == m_board[racine] && m_board_voisin[racine - nb_bubble ] == 0)
                    m_board_voisin[racine - nb_bubble ] = 1;
                        
            if (racine % nb_bubble > 0) // ? gauche (on ?limine la premi?re bulle de chaque ligne)
                if (m_board[racine - 1] == m_board[racine] && m_board_voisin[racine - 1] == 0)
                    m_board_voisin[racine - 1] = 1;

            if (racine % nb_bubble < (nb_bubble - 1)) // ? droite (on ?limine la derni?re bulle de chaque ligne)
                if (m_board[racine + 1] == m_board[racine] && m_board_voisin[racine + 1] == 0)
                    m_board_voisin[racine + 1] = 1;

            if (racine < nb_bubble * (nb_ligne - 1) && racine % nb_bubble > 0) // En bas ? gauche, on v?rifie que le bulle peut exister (on ?limine la derni?re ligne et la premi?re bulle de chaque ligne)
                if (m_board[racine + nb_bubble - 1] == m_board[racine] && m_board_voisin[racine + nb_bubble - 1] == 0)
                    m_board_voisin[racine + nb_bubble - 1] = 1;

            if (racine < nb_bubble * (nb_ligne - 1)) // En bas ? droite, on v?rifie que le bulle peut exister (on ?limine la derni?re ligne)
                if (m_board[racine + nb_bubble ] == m_board[racine] && m_board_voisin[racine + nb_bubble ] == 0)
                    m_board_voisin[racine + nb_bubble ] = 1;

            break;
        case 1:  // Ligne impaire
            if (racine >= nb_bubble) // En haut ? gauche, on v?rifie que le bulle peut exister
                if (m_board[racine - nb_bubble] == m_board[racine] && m_board_voisin[racine - nb_bubble] ==0 )
                    m_board_voisin[racine - nb_bubble] = 1;

            if (racine >= nb_bubble && racine % nb_bubble < (nb_bubble - 1)) // En haut ? Droite, on v?rifie que le bulle peut exister
                if (m_board[racine - nb_bubble + 1] == m_board[racine] && m_board_voisin[racine - nb_bubble + 1] == 0)
                    m_board_voisin[racine - nb_bubble + 1] = 1;

            if (racine % nb_bubble > 0) // ? gauche
                if (m_board[racine - 1] == m_board[racine] && m_board_voisin[racine - 1] == 0)
                    m_board_voisin[racine - 1] = 1;

            if (racine % nb_bubble < (nb_bubble - 1)) // ? droite
                if (m_board[racine + 1] == m_board[racine] && m_board_voisin[racine + 1] == 0)
                    m_board_voisin[racine + 1] = 1;

            if (racine < nb_bubble*(nb_ligne-1)) // En bas ? gauche, on v?rifie que le bulle peut exister
                if (m_board[racine + nb_bubble] == m_board[racine] && m_board_voisin[racine + nb_bubble] ==0 )
                    m_board_voisin[racine + nb_bubble] = 1;

            if (racine < nb_bubble * (nb_ligne - 1) && racine % nb_bubble < (nb_bubble - 1)) // En bas ? droite, on v?rifie que le bulle peut exister
                if (m_board[racine + nb_bubble + 1] == m_board[racine] && m_board_voisin[racine + nb_bubble + 1] == 0)
                    m_board_voisin[racine + nb_bubble + 1] = 1;

            break;

        }
         

    }
}
