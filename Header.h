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

typedef struct maillon{
    int num;
    struct maillon * suivant;
} t_maillon;

typedef struct fileDyn{
    t_maillon * Maillon_ADefiler;
    t_maillon * Maillon_AEnfiler;
}t_fileDyn;



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
    float temps_tot;
    int *tab_station;
    struct station* suivant;
}t_station;

typedef struct graphe
{
    int ordre;
    float temps_de_cycle;
    taches *tache;
    t_station * ancre;
}t_graphes;


#endif //PROJET_ING2_HEADER_H
