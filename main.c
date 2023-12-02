#include <stdio.h>
#include <stdlib.h>
#include "Header.h"
#include <stdbool.h>

#define MAX_OPERATIONS 100 // Assurez-vous que cette valeur est suffisamment grande pour couvrir le nombre maximum d'opérations


void creer_tab_exclu(taches* t, int s1, int s2) {
    int i = 0;

    while (t[i].numero != s1) i++;

    if (t[i].degre == 0) {
        t[i].exclusion = (int*)malloc(sizeof(int));
        t[i].exclusion[0] = s2;
        t[i].degre++;
    } else {
        t[i].exclusion = (int*)realloc(t[i].exclusion, t[i].degre * sizeof(int));
        t[i].exclusion[t[i].degre] = s2;
        t[i].degre++;
    }
}

void creer_tab_prece(taches* t, int s1, int s2)
{
    int i = 0;

    while(t[i].numero != s2) i++;


    if(t[i].nb_de_prece==0)
    {
        t[i].precedences = (int *) malloc(sizeof(int ));
        t[i].precedences[0]= s1;
        t[i].nb_de_prece++;
    }
    else
    {
        t[i].precedences = (int *) realloc(t[i].precedences, t[i].nb_de_prece * sizeof(int ));
        t[i].precedences[t[i].nb_de_prece] = s1;
        t[i].nb_de_prece++;
    }

}
int colorer_taches(taches* t, int ordre) {
    // Initialisation des couleurs
    int* couleurs = (int*)malloc(ordre * sizeof(int));
    for (int i = 0; i < ordre; ++i) {
        couleurs[i] = -1;
    }



    // Attribution des couleurs
    int couleur = 0;
    for (int i = 0; i < ordre; ++i) {
        if (couleurs[i] == -1) {
            couleurs[i] = couleur;

            // Colorer les tâches non adjacentes
            for (int j = 0; j < ordre; ++j) {
                if (couleurs[j] == -1) {
                    bool adjacente = false;
                    for (int k = 0; k < t[j].degre; ++k) {
                        if (couleurs[t[j].exclusion[k]] == couleur) {
                            adjacente = true;
                            break;
                        }
                    }
                    if (!adjacente) {
                        couleurs[j] = couleur;
                    }
                }
            }

            couleur++;
        }
        t[i].couleur = couleurs[i];
    }

    // Le nombre minimum de stations est égal au nombre de couleurs utilisées
    printf("Le nombre minimum de stations necessaire avec l'algo naif est : %d\n", couleur);

    // Le nombre minimum de stations est égal au nombre de couleurs utilisées
    int nombreStations = couleur;
    free(couleurs);
    return nombreStations;
}

void afficher_taches_par_station(taches* t, int ordre) {
    for (int i = 0; i < ordre; ++i) {
        printf("Station %d: Taches ", i + 1);
        for (int j = 0; j < ordre; ++j) {
            if (t[j].couleur == i) {
                printf("%d ", t[j].numero);
            }
        }
        printf("\n");
    }
}


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

    /// Creation du graphe tache dynamiquement
    taches *t;
    t = (taches *)malloc(ordre*sizeof(taches));

//    printf("%d", ordre);
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

    for (int i = 0; i < ordre; ++i) t[i].degre = 0;

    fseek(exclu, 0, SEEK_END);
    ret = ftell(exclu);
    rewind(exclu);

    pos = ftell(exclu);
    while (pos!=ret)
    {
        fscanf(exclu,"%d %d", &s1, &s2);

        creer_tab_exclu(t, s1, s2);
        creer_tab_exclu(t, s2, s1);

        pos = ftell(exclu);

    }




    for (int i = 0; i < ordre; ++i)
    {
        printf("Tache %d:  ", t[i].numero);
        for (int j = 0; j < t[i].degre; ++j)
        {
            printf("%d ",t[i].exclusion[j]);
        }
        printf("\n");

    }




    ///Lecture du fichier precedences
    s1 = s2 = 0;


    for (int i = 0; i < ordre; ++i)
    {
        t[i].nb_de_prece = 0;
        t[i].precedences = -1;

    }


    fseek(prece, 0, SEEK_END);
    ret = ftell(prece);
    rewind(prece);

    pos = ftell(prece);
    while (pos!=ret)
    {
        fscanf(prece,"%d %d", &s1, &s2);

        creer_tab_prece(t, s1, s2);

        pos = ftell(prece);
    }

    colorer_taches(t, ordre);


/*
    for (int i = 0; i < ordre; ++i)
    {
        printf("Tache %d:  ", t[i].numero);
        if (t[i].precedences==-1) printf("-1 ");
        for (int j = 0; j < t[i].nb_de_prece; ++j)
        {
            printf("%d ",t[i].precedences[j]);
        }
        printf("\n");

    }
*/



    ///Fermeture de fichiers
    fclose(opera);
    fclose(tps_cyle);
    fclose(exclu);
    fclose(prece);


    return t;

}


t_station precedances(taches *t, int ordre)
{
    t_station* station;

    station = (t_station *)malloc(sizeof(t_station));


    int compteur = 0;

    for (int i = 0; i < ordre; ++i)
    {
//        if (t[i].precedences==-1) printf();

    }



//    t[i].exclusion = (int *) realloc(t[i].exclusion, t[i].degre * sizeof(int ));


}
// Fonction pour trier les tâches par degré décroissant
void trier_taches_par_degre(taches* t, int ordre) {
    for (int i = 0; i < ordre - 1; ++i) {
        for (int j = 0; j < ordre - i - 1; ++j) {
            if (t[j].degre < t[j + 1].degre) {
                // Échange
                taches temp = t[j];
                t[j] = t[j + 1];
                t[j + 1] = temp;
            }
        }
    }
}



// Algorithme de Welsh-Powell
int welsh_powell(taches* t, int ordre) {
    // Tri des tâches par degré décroissant
    trier_taches_par_degre(t, ordre);

    // Impression des tâches triées
    printf("Taches triees par degre decroissant : ");
    for (int i = 0; i < ordre; ++i) {
        printf("%d ", t[i].numero);
    }
    printf("\n");

    // Appliquer l'algorithme naïf pour colorier les tâches
    int nombreStations = colorer_taches(t, ordre);

    // Impression des couleurs attribuées
    printf("Couleurs attribuees aux taches : ");
    for (int i = 0; i < ordre; ++i) {
        printf("%d ", t[i].couleur);
    }
    printf("\n");

    // Retourner le nombre de stations (couleurs utilisées)
    return nombreStations;
}

int main() {
    t_graphes graphe;
    t_station station;

    graphe.tache = lire_fichier();
    graphe.ordre = 31; // Mettez la valeur correcte ici
    afficher_taches_par_station(graphe.tache, graphe.ordre);
    int nombreStations=welsh_powell(graphe.tache, graphe.ordre);
    printf("Nombre de stations selon l'algorithme de Welsh-Powell : %d\n", nombreStations);

    return 0;
}
