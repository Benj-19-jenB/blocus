#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Ajouter cet en-tête pour usleep

#define GRID_SIZE 10
#define CELL_SIZE 50

typedef struct {
    int x, y;
} Player;

typedef enum {
    MODE_TWO_PLAYER,
    MODE_AI
} GameMode;

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

void DessinerBloc(int x, int y, couleur c) {
    ChoisirCouleurDessin(c);
    RemplirRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
}

void PlacerPion(Player *p) {
    while (1) {
        if (SourisCliquee()) {
            int x = _X / CELL_SIZE;
            int y = _Y / CELL_SIZE;
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                p->x = x;
                p->y = y;
                break;
            }
        }
    }
}

void BloquerCase(int grid[GRID_SIZE][GRID_SIZE]) {
    while (1) {
        if (SourisCliquee()) {
            int x = _X / CELL_SIZE;
            int y = _Y / CELL_SIZE;
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && grid[x][y] == 0) {
                grid[x][y] = 1;
                break;
            }
        }
    }
}

void MettreAJourJeu(Player *p1, Player *p2, int grid[GRID_SIZE][GRID_SIZE], int *tour) {
    // Exemple de gestion des touches pour déplacer les joueurs
    if (ToucheEnAttente()) {
        int t = Touche();
        Player *p = (*tour == 0) ? p1 : p2;
        switch (t) {
            case XK_Left:
                if (p->x > 0 && grid[p->x - 1][p->y] == 0) p->x--;
                break;
            case XK_Right:
                if (p->x < GRID_SIZE - 1 && grid[p->x + 1][p->y] == 0) p->x++;
                break;
            case XK_Up:
                if (p->y > 0 && grid[p->x][p->y - 1] == 0) p->y--;
                break;
            case XK_Down:
                if (p->y < GRID_SIZE - 1 && grid[p->x][p->y + 1] == 0) p->y++;
                break;
            // Ajouter les touches pour le joueur 2
        }
        // Bloquer une case après le mouvement
        BloquerCase(grid);
        *tour = (*tour + 1) % 2; // Changer de tour
    }
}

GameMode AfficherMenu() {
    printf("Choisissez le mode de jeu:\n");
    printf("1. Deux joueurs\n");
    printf("2. Jouer contre l'IA\n");
    int choix;
    scanf("%d", &choix);
    return (choix == 1) ? MODE_TWO_PLAYER : MODE_AI;
}

int main(void) {
    Player joueur1, joueur2;
    int grid[GRID_SIZE][GRID_SIZE] = {0}; // 0 = libre, 1 = bloqué
    InitialiserJeu(&joueur1, &joueur2);

    couleur couleurJoueur1 = CouleurParNom("blue");
    couleur couleurJoueur2 = CouleurParNom("red");
    couleur couleurBloc = CouleurParNom("gray");
    couleur fond = CouleurParNom("white");

    GameMode mode = AfficherMenu();

    printf("Joueur 1, placez votre pion.\n");
    PlacerPion(&joueur1);
    printf("Joueur 1, bloquez une case.\n");
    BloquerCase(grid);
    printf("Joueur 2, placez votre pion.\n");
    PlacerPion(&joueur2);
    printf("Joueur 2, bloquez une case.\n");
    BloquerCase(grid);

    int tour = 0;
    while (1) {
        EffacerEcran(fond);
        DessinerGrille();
        DessinerJoueur(joueur1, couleurJoueur1);
        DessinerJoueur(joueur2, couleurJoueur2);
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (grid[i][j] == 1) {
                    DessinerBloc(i, j, couleurBloc);
                }
            }
        }
        MettreAJourJeu(&joueur1, &joueur2, grid, &tour);
        usleep(100000); // Petite pause pour éviter une utilisation excessive du CPU
    }

    FermerGraphique();
    return 0;
}
