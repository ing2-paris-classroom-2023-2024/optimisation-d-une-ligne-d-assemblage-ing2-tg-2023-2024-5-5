#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "header.h"

t_graphes* Creert_graphes(int ordre){
    t_graphes * Newt_graphes=(t_graphes*)malloc(sizeof(t_graphes));
    Newt_graphes->tache = (taches*)malloc(ordre*sizeof(taches));
    //Newt_graphes->file = (int*)malloc(ordre*sizeof(int));

    for(int i=0; i<ordre; i++){
        Newt_graphes->tache[i]=*(taches*)malloc(sizeof (taches));
        Newt_graphes->tache[i].marquage = false;

    }
    Newt_graphes->nb_station = 0;
    return Newt_graphes;
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

t_graphes *lire_fichier()
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
    float temps_cycle;

    fscanf(tps_cyle, "%f", &temps_cycle);



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
    int s1, s2;

    for (int i = 0; i < ordre; ++i) t[i].degre = 0;

    fseek(exclu, 0, SEEK_END);
    ret = ftell(exclu);
    rewind(exclu);


    pos = ftell(exclu);
    while (pos!=ret)
    {
        fscanf(exclu,"%d %d", &s1, &s2);

        //creer_tab_exclu(t, s1, s2);
        //creer_tab_exclu(t, s2, s1);

        pos = ftell(exclu);

    }


    /*

    for (int i = 0; i < ordre; ++i)
    {
        printf("Tache %d:  ", t[i].numero);
        for (int j = 0; j < t[i].degre; ++j)
        {
            printf("%d ",t[i].exclusion[j]);
        }
        printf("\n");

    }

    */


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

    g->tache = t;

    return g;

}

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
bool parcourir_m(t_graphes *g, int s1, int s2)
{
    if(g->matrice[s1][s2]==0) {
        return false;
    }
    else{
        return true;
    }


}


t_station * insererAuMilieu(t_station *tete, int valeur, int position) {
    t_station *nouveauMaillon = (t_station *)malloc(sizeof(t_station));
    if (nouveauMaillon == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }

    nouveauMaillon->tab_station = (int *) malloc(sizeof(int));

    if (position == 1) {
        // Cas particulier : insertion en tête de liste
        nouveauMaillon->suivant = tete;
        return nouveauMaillon;
    }

    t_station *courant = tete;
    int i = 1;

    // Parcourir la liste jusqu'à l'emplacement désiré
    while (i < position - 1 && courant != NULL) {
        courant = courant->suivant;
        i++;
    }

    if (courant == NULL) {
        // La position spécifiée est hors de la taille actuelle de la liste
        printf("Position invalide pour l'insertion.\n");
        free(nouveauMaillon);
        return tete;
    }

    // Insérer le nouveau maillon au milieu
    nouveauMaillon->suivant = courant->suivant;
    courant->suivant = nouveauMaillon;

    return tete;
}

void integration(t_graphes *g)
{
    t_station *actual = g->ancre;
    int position=1;
    while (actual!=NULL){
         for (int i = 0; i < actual->ordre-1; ++i)
         {
             int s1 = actual->tab_station[i];
             int s2 = actual->tab_station[i-1];

             int x= 0;
             while (s1 != g->tache[x].numero) x++;

             int y= 0;
             while (s2 != g->tache[y].numero) y++;

             if(parcourir_m(g,x,y)==true)
             {
                 g->nb_station++;


             }else {
                 actual=actual->suivant;
             }

    }


    }


}




int welsh_powell(taches* t, int ordre) {
    /// Tri des tâches par degré décroissant

    trier_taches_par_degre(t, ordre);
/*
    /// Impression des tâches triées
    printf("Taches triees par degre decroissant : ");
    for (int i = 0; i < ordre; ++i) {
        printf("%d ", t[i].numero);
    }
    printf("\n");
    */
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
    printf("le nb de station est %d",nb_s);

    
    /// Liberation
    free(graphe);

    return 0;
}