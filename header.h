//
// Created by osave on 03/12/2023.
//
#include <stdbool.h>

#ifndef PROJET_ING2_TEST_HEADER_H
#define PROJET_ING2_TEST_HEADER_H


typedef struct station
{
    int ordre;
    float temps_tot;
    int *tab_station;
    struct station* suivant;
}t_station;

typedef struct tache
{
    int numero;
    float temps;
    int degre;

    int *exclusion;

    int *precedences;
    int nb_de_prece;

    int couleur;
    bool marquage;

}taches;

typedef struct graphe
{
    int ordre;
    float temps_de_cycle;
    taches *tache;
    t_station *ancre;
    int **matrice;
}t_graphes;


#endif //PROJET_ING2_TEST_HEADER_H
