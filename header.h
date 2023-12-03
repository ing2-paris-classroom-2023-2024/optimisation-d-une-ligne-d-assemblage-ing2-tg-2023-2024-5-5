//
// Created by wassi on 28/11/2023.
//

#ifndef UNTITLED1_HEADER_H
#define UNTITLED1_HEADER_H
#include <stdbool.h>

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
    struct Arc* arc;
}taches;


typedef struct station
{
    int *station;
}t_station;

typedef struct graphe
{
    int ordre;
    int temps_de_cycle;
    int **matrice;
    taches *tache;
}t_graphes;




#endif //UNTITLED1_HEADER_H

