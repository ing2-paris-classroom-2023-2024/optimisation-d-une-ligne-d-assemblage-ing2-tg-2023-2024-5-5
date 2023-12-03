#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#define NBMAX 1000


int indice(int taille, int nb, t_graphes *graphe){
    for (int i = 0; i < taille; ++i) {
        if (graphe->tache[i].numero == nb){
            return i;
        }
    }
}

t_graphes* Creert_graphes(int ordre){
    t_graphes * Newt_graphes=(t_graphes*)malloc(sizeof(t_graphes));
    Newt_graphes->tache = (taches*)malloc(ordre*sizeof(taches));
    //Newt_graphes->file = (int*)malloc(ordre*sizeof(int));

    for(int i=0; i<ordre; i++){
        Newt_graphes->tache[i]=*(taches*)malloc(sizeof (taches));
        Newt_graphes->tache[i].arc=NULL;
        Newt_graphes->tache[i].marquage = false;

    }

    return Newt_graphes;
}



t_graphes *lire_fichier()
{

    /// Lecture des diffferents fichiers txt
    FILE *tps_cyle = fopen("temps_cycle.txt", "r");
    FILE *exclu = fopen("exclusions.txt", "r");
    FILE *opera = fopen("operations.txt", "r");
    FILE *prece = fopen("precedences.txt", "r");


    /// Si erreur de fichier
    if (!tps_cyle || !exclu || !opera || !prece)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }



    /// Lecture du fichier temps de cycle
    float temps_cycle;

    fscanf(tps_cyle, "%f", &temps_cycle);



    //feof
    /// Lecture du fichier operations
    long ret, pos;
    int sommet, ordre;
    float temps;

    ordre = 0;
    ///Recuperation de la position de la derniere valeur du fichier, puis replacer le curseur a 0
    fseek(opera, 0, SEEK_END);
    ret = ftell(opera);
    rewind(opera);

    ///Prendre la posstion actuelle et la comparer à celle de fin pour avoir le nombre de sommet, l'ordre
    pos = ftell(opera);
    while(pos != ret)
    {
        fscanf(opera, "%d %f", &sommet, &temps);
        ordre++;
        pos = ftell(opera);
    }


    /// Creation du t_graphes tache dynamiquement
    t_graphes *g;
    g = Creert_graphes(ordre);

    g->ordre = ordre;
    g->temps_de_cycle = temps_cycle;

    taches* t = g->tache;



    ///On revient au debut du fichier pour cette fois lire les données et les mettre à leur place
    rewind(opera);
    for (int i = 0; i < ordre; ++i)
    {


        fscanf(opera, "%d %f", &sommet, &temps);

        t[i].numero = sommet;
        t[i].temps = temps;
    }


    /// Lecture fichier texte exclusions
    int s1, s2;
    g->matrice = (int **) malloc(g->ordre*sizeof(int*));

    for (int i = 0; i < g->ordre; ++i) {
        g->matrice[i]=(int *) malloc(g->ordre*sizeof(int));
    }
    for (int i = 0; i < g->ordre; ++i) {
        for (int j = 0; j < g->ordre; ++j) {
            g->matrice[i][j] = 1;
        }

    }

    while (fscanf(exclu, "%d %d", &s1, &s2) == 2) {
        s1 = indice(g->ordre, s1, g);
        s2 = indice(g->ordre, s2, g);
        g->matrice[s1][s2] = 0;
        g->matrice[s2][s1] = 0;
    }

   /* for (int i = 0; i < g->ordre; ++i) {
        for (int j = 0; j < g->ordre; ++j) {
            printf("case[%d][%d] : %d \n", g->tache[i].numero, g->tache[j].numero, gmatrice[i][j]);
        }

    }*/


    ///Fermeture de fichiers
    fclose(opera);
    fclose(tps_cyle);
    fclose(exclu);
    fclose(prece);

    g->tache = t;

    return g;

}

void assignStations(t_graphes g) {
    int rows[g.ordre * g.ordre];
    int cols[g.ordre * g.ordre];
    int indexCount = 0;

    // Collecter les indices
    for (int i = 0; i < g.ordre; i++) {
        for (int j = 0; j < g.ordre; j++) {
            if (g.matrice[i][j] == 1) {
                rows[indexCount] = i;
                cols[indexCount] = j;
                indexCount++;
            }
        }
    }

    // Trier les indices en fonction de leur valeur
    for (int i = 0; i < indexCount - 1; i++) {
        for (int j = 0; j < indexCount - i - 1; j++) {
            if (rows[j] * g.ordre + cols[j] > rows[j + 1] * g.ordre + cols[j + 1]) {
                // Échanger les indices
                int tempRow = rows[j];
                int tempCol = cols[j];

                rows[j] = rows[j + 1];
                cols[j] = cols[j + 1];

                rows[j + 1] = tempRow;
                cols[j + 1] = tempCol;
            }
        }
    }

    // Assigner les indices aux stations
    int stations[g.ordre][g.ordre];
    int stationCount = 0;

    for (int i = 0; i < indexCount; i++) {
        bool assigned = false;

        for (int j = 0; j < stationCount; j++) {
            bool canAssign = true;

            for (int k = 0; k < g.ordre; k++) {
                int rowIndex = rows[i];
                int colIndex = cols[i];

                if (g.matrice[rowIndex][k] == 0 && stations[j][k] == 1) {
                    canAssign = false;
                    break;
                }
            }

            if (canAssign) {
                stations[j][cols[i]] = 1;
                assigned = true;
                break;
            }
        }

        if (!assigned) {
            stations[stationCount][cols[i]] = 1;
            stationCount++;
        }
    }

    printf("Nombre de stations : %d\n", stationCount);

    for (int i = 0; i < stationCount; i++) {
        printf("Station %d : ", i + 1);

        for (int j = 0; j < g.ordre; j++) {
            printf("%d ", stations[i][j]);
        }

        printf("\n");
    }
}

int main() {
    t_graphes *g;
    g = lire_fichier();
    assignStations(*g);
    return 0;

}