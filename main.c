#include <stdio.h>
#include "graphe.h"
#include <stdlib.h>

void afficher_successeurs(pSommet * sommet, int num){
    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc;

    while(arc!=NULL){
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}


// donné
// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2){
    if(sommet[s1]->arc==NULL){
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc=Newarc;
        return sommet;
    }

    else{
        pArc temp=sommet[s1]->arc;

        while( !(temp->arc_suivant==NULL)){
            temp=temp->arc_suivant;
        }

        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;

        if(temp->sommet>s2){
            Newarc->arc_suivant=temp->arc_suivant;
            Newarc->sommet=temp->sommet;
            temp->sommet=s2;
            temp->arc_suivant=Newarc;
            return sommet;
        }

        temp->arc_suivant=Newarc;
        return sommet;
    }
}


// donné
// créer le graphe
Graphe* CreerGraphe(int ordre){
    Graphe * Newgraphe=(Graphe*)malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));

    for(int i=0; i<ordre; i++){
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->valeur=i;
        Newgraphe->pSommet[i]->arc=NULL;
    }

    return Newgraphe;
}


// donné
// La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
// Le fichier contient : ordre, taille, orientation (0 ou 1)et liste des arcs
Graphe * lire_graphe(char * nomFichier) {
    Graphe* graphe;
    FILE * ifs = fopen(nomFichier,"r");
    int taille, ordre, s1, s2;

    if (!ifs){
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    fscanf(ifs,"%d",&ordre);

    graphe=CreerGraphe(ordre); // créer le graphe d'ordre sommets

    fscanf(ifs,"%d",&taille);

    graphe->ordre=ordre;
    graphe->taille = taille;

    // créer les arêtes du graphe
    for (int i=0; i<taille; ++i){
        fscanf(ifs,"%d%d",&s1,&s2);
        graphe->pSommet=CreerArete(graphe->pSommet, s1, s2);
    }

    return graphe;
}


// donné
// affichage du graphe avec les successeurs de chaque sommet
void graphe_afficher(Graphe* graphe){
    printf("graphe\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'adjacence :\n");

    for (int i=0; i<graphe->ordre; i++){
      afficher_successeurs(graphe->pSommet, i);
      printf("\n");
    }

}




void intervertir(struct Sommet *a, struct Sommet *b) {
    struct Sommet *temp;
    temp->valeur = a->valeur;
    temp->couleur = a->couleur;
    temp->degre = a->degre;
    temp->arc = a->arc;

    a->valeur = b->valeur;
    a->couleur = b->couleur;
    a->degre = b->degre;
    a->arc = b->arc;

    b->valeur = temp->valeur;
    b->couleur = temp->couleur;
    b->arc = temp->arc;
}

// Fonction pour trier un tableau de structures par ordre croissant
void trie_a_bulle(pSommet *tabarc, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d\n",tabarc[n]->degre);

    }
    /*
    int echange; // on initialise une variable qui va nous permettre d'arreter la fonction quand on a finit
    for (int i = 0; i < n - 1; i++) {
        echange = 0;
        for (int j = 0; j < n - i - 1; j++) { // on parcourt le tableau en paramètre pour effectuer le trie a bulle
            if (tabarc[j]->degre < tabarc[j + 1]->degre) {
                intervertir(tabarc[j], tabarc[j+1]); // on utilise la fonction fait au dessus pour échanger deux valeurs
                echange = 1;
            }
        }
        // Si aucun échange n'a été effectué pendant une itération, le tableau est déjà trié
        if (echange == 0) {
            break;
        }
    }
     */
}
void W_P(Graphe *graphe,int ordre){
    pSommet tab[ordre];

    for (int i = 0; i <ordre ; ++i) {
        tab[i]=graphe->pSommet[ordre];
        printf("%d",tab[i]->degre);
    }

    trie_a_bulle(tab,ordre);
    for (int i = 0; i < ordre; ++i) {
        printf("%d",tab[i]->degre);
    }


}
void bsf_change(){

}

int main() {
    Graphe * g;
    char nom_fichier[50];

    printf("entrer le nom du fichier du graphe:\n");
    gets(nom_fichier);


    g = lire_graphe(nom_fichier);
    graphe_afficher(g);
    return 0;
}
