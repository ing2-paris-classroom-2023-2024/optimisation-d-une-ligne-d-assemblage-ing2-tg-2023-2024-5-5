#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "header.h"

int colorer_taches(taches* t, int ordre) {
    /// Initialisation des couleurs
    int* couleurs = (int*)malloc(ordre * sizeof(int));
    for (int i = 0; i < ordre; ++i) {
        couleurs[i] = -1;
    }



    /// Attribution des couleurs
    int couleur = 0;
    for (int i = 0; i < ordre; ++i) {
        if (couleurs[i] == -1) {
            couleurs[i] = couleur;

            /// Colorer les tâches non adjacentes
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



    /// Le nombre minimum de stations est égal au nombre de couleurs utilisées
    int nombreStations = couleur;
    free(couleurs);
    return nombreStations;
}

t_graphes* Creert_graphes(int ordre){
    t_graphes * Newt_graphes=(t_graphes*)malloc(sizeof(t_graphes));  // on crée dynamiquement  la structure graphe
    Newt_graphes->tache = (taches*)malloc(ordre*sizeof(taches)); // on crée dynamiquement le tableau de structure tache
    // initialisiation des variables
    for(int i=0; i<ordre; i++){
        Newt_graphes->tache[i]=*(taches*)malloc(sizeof (taches));
        Newt_graphes->tache[i].marquage = false;

    }
    Newt_graphes->nb_station = 0;
    return Newt_graphes;
}
 // creation du tableau de precedence
void creer_tab_prece(taches* t, int s1, int s2)
{
    // on récupère l indice
    int i = 0;

    while(t[i].numero != s2) i++;


    // creation du tableau si il ya pas encore de precedence
    if(t[i].nb_de_prece==0)
    {
        t[i].precedences = (int *) malloc(sizeof(int ));
        t[i].precedences[0]= s1;
        t[i].nb_de_prece++;
    }
    else
    { // reallocation du tableau avec le bon nombre
        t[i].precedences = (int *) realloc(t[i].precedences, t[i].nb_de_prece * sizeof(int ));
        t[i].precedences[t[i].nb_de_prece] = s1;
        t[i].nb_de_prece++;
    }

}
// meme fonctionnement que la fonction precedente
void creer_tab_exclu(taches* t, int s1, int s2)
{

    int i = 0;

    while(t[i].numero != s1) i++;


    if (t[i].degre == 0)
    {
        t[i].exclusion = (int *) malloc(sizeof(int));
        t[i].exclusion[0] = s2;
        t[i].degre++;
    }
    else
    {
        t[i].exclusion = (int *) realloc(t[i].exclusion, t[i].degre * sizeof(int ));
        t[i].exclusion[t[i].degre] = s2;
        t[i].degre++;
    }
}



/// fonctions permettant d'avoir l'indice du sommet en fonction des oppérations
int indice(int taille, int nb, t_graphes *graphe){
    for (int i = 0; i < taille; ++i) {
        if (graphe->tache[i].numero == nb){
            return i;
        }
    }
}

t_graphes *lire_fichier()
{

    /// Lecture des diffferents fichiers txt
    FILE *tps_cyle = fopen("temps_cycle2.txt", "r");
    FILE *exclu = fopen("exclusions2.txt", "r");
    FILE *opera = fopen("operations2.txt", "r");
    FILE *prece = fopen("precedences2.txt", "r");



    /// Si erreur de fichier
    if (!tps_cyle || !exclu || !opera || !prece)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }



    /// Lecture du fichier temps de cycle
    float temps_cycle;

    fscanf(tps_cyle, "%f", &temps_cycle);



    ///feof
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
    /// création d'une matrice initialisée à 1 dans laquelle nous allons stocker les sommets et assigner 0 lorsqu'ils sont adjacents
    int s1, s2;

    for (int i = 0; i < ordre; ++i) t[i].degre = 0;

    fseek(exclu, 0, SEEK_END);
    ret = ftell(exclu);
    rewind(exclu);

    g->matrice = (int **) malloc(g->ordre*sizeof(int*));

    for (int i = 0; i < g->ordre; ++i) {
        g->matrice[i]=(int *) malloc(g->ordre*sizeof(int));
    }
    for (int i = 0; i < g->ordre; ++i) {
        for (int j = 0; j < g->ordre; ++j) {
            g->matrice[i][j] = 1;
        }

    }
    pos = ftell(exclu);
    while (pos!=ret)
    {
        fscanf(exclu,"%d %d", &s1, &s2);
        s1 = indice(g->ordre, s1, g);
        s2 = indice(g->ordre, s2, g);
        g->matrice[s1][s2] = 0;
        g->matrice[s2][s1] = 0;
        creer_tab_exclu(t, s1, s2);
        creer_tab_exclu(t, s2, s1);

        pos = ftell(exclu);

    }


    ///Lecture du fichier precedences
    s1 = s2 = 0;


    for (int i = 0; i < ordre; ++i)
    {
        t[i].nb_de_prece = 0;
        t[i].precedences = (int *) -1;

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
    g->tache = t;
    int couleur = colorer_taches(g->tache, g->ordre);
    printf("Le nombre minimum de stations necessaire avec l'algo naif est : %d\n", couleur);

    /// Fermeture de fichiers
    fclose(opera);
    fclose(tps_cyle);
    fclose(exclu);
    fclose(prece);



    return g;

}

void trier_taches_par_degre(taches* t, int ordre)
{ // parcourt du tableau de taches
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
// creation d une station qui sera en liste chaine
t_station * creer_station() {
    t_station *nouvelle_station;
    nouvelle_station = (t_station *) malloc(sizeof(t_station));
    nouvelle_station->temps_tot = 0;
    nouvelle_station->ordre = 0;
    nouvelle_station->suivant = NULL;
    nouvelle_station->tab_station = (int *) malloc(sizeof (int ));
    return nouvelle_station;
}

void assigner_a_station(t_graphes * g, t_station * station, taches * tache) {
    printf("On assigne tache %d a station.\n", tache->numero);

    if (station == NULL) {
        printf("Erreur: Station null.\n");
        return;
    }

    /// On parcours la liste chainée de station pour arriver a la deniere
    while (station->suivant != NULL) {
        station = station->suivant;
    }

    /// Allocution de la mémoire si on a pas de tab_station
    if (station->tab_station == NULL) {
        station->tab_station = (int *)malloc(g->ordre * sizeof(int));
        if (station->tab_station == NULL) {
            printf("Erreur de memoire.\n");
            return;
        }
    }

    /// On regarde le temps de la station + temps de la tache puis on a compare au temps de cycle
    if (station->temps_tot + tache->temps <= g->temps_de_cycle) {

        /// Assignement de la tache a la station
        station->tab_station[station->ordre++] = tache->numero;

        /// Mise a jour du nouveau temps
        station->temps_tot += tache->temps;

    } else {
        /// Si le temps est supérieur, création d'une nouvelle tache
        t_station *nouveau = creer_station();
        g->nb_station++;
        nouveau->tab_station = (int *)malloc(g->ordre * sizeof(int));

        /// Assignement de la tache a la station
        nouveau->tab_station[nouveau->ordre++] = tache->numero;

        /// Mise a jour du nouveau temps
        nouveau->temps_tot += tache->temps;

        /// Mise a jour de la station suivante avec la nouvelle station
        station->suivant = nouveau;

    }
}
//
void afficher_s(t_graphes *g){
    int numero=1;
    t_station *actual = g->ancre;
    while(actual!=NULL){
        printf("l etat de la station %d  est ",numero);

        for (int i = 0; i < actual->ordre; ++i) {
            printf("%d ",actual->tab_station[i]);
        }
        printf("\n");
        numero++;
        actual = actual->suivant;
    }
}

void BFS(t_graphes *g) {
    int *tab = (int *)malloc(g->ordre * sizeof(int));
    int x = -1, y = -1;
    g->ancre = creer_station();
    g->nb_station++;

    /// On détecte ici tous les points de départ potentiels
    for (int i = 0; i < g->ordre; ++i) {
        if (g->tache[i].nb_de_prece == 0) {
            tab[++y] = i;
        }
    }

    /// Boucle principale du BFS
    while (x != y) {
        int indice_tache_actuel = tab[++x];

        /// On assigne la tache à la station correspondante
        assigner_a_station(g, g->ancre, &g->tache[indice_tache_actuel]);

        /// Mise à jour du temps et marquage de la tâche comme visitée
        g->tache[indice_tache_actuel].marquage = 1;

        /// Changement de nb_de_prece
        for (int i = 0; i < g->ordre; ++i) {
            if (g->tache[i].nb_de_prece > 0) {
                g->tache[i].nb_de_prece--;

                if (g->tache[i].nb_de_prece == 0 && !g->tache[i].marquage) {
                    tab[++y] = i;
                }
            }
        }
    }

    /// liberation de la mémoire
    free(tab);
}
// on regarde si les deux sommets sont compato
bool parcourir_m(t_graphes *g, int s1, int s2)
{
    if(g->matrice[s1][s2]==0) {
        return false;
    }
    else{
        return true;
    }


}

void inserer_maillon_a_un_index(t_station * ancre, t_station * maillon_a_inserer, int index)
{
    t_station * actual = ancre;
    int cmpt=1;

    while(actual->suivant!= NULL) {
        actual = actual->suivant;
        cmpt++;
        if (cmpt==index)
        {
            maillon_a_inserer->suivant=actual->suivant;
            actual->suivant=maillon_a_inserer;
        }
    }
}
void integration(t_graphes *g)
{
    t_station *actual = g->ancre;
    while (actual!=NULL){
         for (int i = 0; i < actual->ordre-1; ++i)
         {
             int s1 = actual->tab_station[i];
             int s2 = actual->tab_station[i+1];

             int x= 0;
             while (s1 != g->tache[x].numero) x++;

             int y= 0;
             while (s2 != g->tache[y].numero) y++;

             if(parcourir_m(g,x,y)==true)
             {
                 g->nb_station++;


             }
//             else
//             {
//                 actual=actual->suivant;
//             }

        }

         actual = actual->suivant;
    }


}

int welsh_powell(taches* t, int ordre) {
    /// Tri des tâches par degré décroissant

    trier_taches_par_degre(t, ordre);

    /// Appliquer l'algorithme naïf pour colorier les tâches
    int nombreStations = colorer_taches(t, ordre);

    /// Retourner le nombre de stations (couleurs utilisées)
    return nombreStations;
}





int main() {
    /// Lecture des fichiers txt et création de notre graphe
    t_graphes *graphe = lire_fichier();

    /// Lancement du bfs
    BFS(graphe);
    printf("%d\n", graphe->nb_station);
    integration(graphe);
    
    /// Affichage de nos stations
    afficher_s(graphe);
    int nb_s= welsh_powell(graphe->tache,graphe->ordre);
    printf("%d \n", graphe->ordre);
    printf("le nb de station est %d\n",nb_s);
    printf("le nb de station est %d\n",graphe->nb_station);

    
    /// Liberation
    free(graphe);

    return 0;
}