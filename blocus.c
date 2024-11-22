#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Ajouter cet en-tête pour usleep

#define GRID_SIZE 10
#define CELL_SIZE 50

typedef struct {
    int x, y;
} Player;

void InitialiserJeu(Player *p1, Player *p2) {
    InitialiserGraphique();
    CreerFenetre(100, 100, GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE);
    p1->x = 0;
    p1->y = 0;
    p2->x = GRID_SIZE - 1;
    p2->y = GRID_SIZE - 1;
}

void DessinerGrille() {
    couleur c = CouleurParNom("black");
    ChoisirCouleurDessin(c);
    for (int i = 0; i <= GRID_SIZE; i++) {
        DessinerSegment(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
        DessinerSegment(0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
    }
}

void DessinerJoueur(Player p, couleur c) {
    ChoisirCouleurDessin(c);
    RemplirRectangle(p.x * CELL_SIZE, p.y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
}

void MettreAJourJeu(Player *p1, Player *p2) {
    // Exemple de gestion des touches pour déplacer les joueurs
    if (ToucheEnAttente()) {
        int t = Touche();
        switch (t) {
            case XK_Left:
                if (p1->x > 0) p1->x--;
                break;
            case XK_Right:
                if (p1->x < GRID_SIZE - 1) p1->x++;
                break;
            case XK_Up:
                if (p1->y > 0) p1->y--;
                break;
            case XK_Down:
                if (p1->y < GRID_SIZE - 1) p1->y++;
                break;
            // Ajouter les touches pour le joueur 2
        }
    }
}

int main(void) {
    Player joueur1, joueur2;
    InitialiserJeu(&joueur1, &joueur2);

    couleur couleurJoueur1 = CouleurParNom("blue");
    couleur couleurJoueur2 = CouleurParNom("red");
    couleur fond = CouleurParNom("white");

    while (1) {
        EffacerEcran(fond);
        DessinerGrille();
        DessinerJoueur(joueur1, couleurJoueur1);
        DessinerJoueur(joueur2, couleurJoueur2);
        MettreAJourJeu(&joueur1, &joueur2);
        usleep(100000); // Petite pause pour éviter une utilisation excessive du CPU
    }

    FermerGraphique();
    return 0;
}
