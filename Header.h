//
// Created by osave on 21/11/2023.
//
#include <stdbool.h>

#ifndef PROJET_ING2_HEADER_H
#define PROJET_ING2_HEADER_H

struct Arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial
    int valeur;
    struct Arc* arc_suivant;
};

/* Alias de pointeur sur un Arc */
typedef struct Arc* pArc;




typedef struct tache
{
    int numero;
    float temps;
    int degre;

    int *exclusion;

    int *precedences;
    int nb_de_prece;

    bool marquage;

    struct Arc* arc;
}taches;


typedef struct station
{
    int ordre;
    int *tab_station;
    struct station* suivant;
}t_station;

typedef struct graphe
{
    int ordre;
    int temps_de_cycle;
    taches *tache;
}t_graphes;


#endif //PROJET_ING2_HEADER_H
