#include <stdio.h>
#include <stdlib.h>
#include "Header.h"


taches* lire_fichier()
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
    int temps_cycle;

    fscanf(tps_cyle, "%d", &temps_cycle);


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

    /// Creation du graphe tache dynamiquement
    taches *t;
    t = (taches *)malloc(ordre*sizeof(taches));

    ///On revient au debut du fichier pour cette fois lire les données et les mettre à leur place
    rewind(opera);
    for (int i = 0; i < ordre; ++i)
    {
        fscanf(opera, "%d %f", &sommet, &temps);
        t[i].numero = sommet;
        t[i].temps = temps;
    }





    return t;
}


int main() {
    taches *t;

    t = lire_fichier();

    return 0;
}

