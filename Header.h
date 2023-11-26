//
// Created by osave on 21/11/2023.
//

#ifndef PROJET_ING2_HEADER_H
#define PROJET_ING2_HEADER_H




typedef struct tache
{
    int numero;
    float temps;
    int degre;
    int *exclusion;
    int *precedences;
    int nb_de_prece;

}taches;


typedef struct station
{
    int *station;
}t_station;

typedef struct graphe
{
    int ordre;
    int temps_de_cycle;
    taches *tache;
}t_graphes;


#endif //PROJET_ING2_HEADER_H
