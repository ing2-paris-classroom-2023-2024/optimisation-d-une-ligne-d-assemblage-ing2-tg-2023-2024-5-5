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



#endif //PROJET_ING2_HEADER_H
