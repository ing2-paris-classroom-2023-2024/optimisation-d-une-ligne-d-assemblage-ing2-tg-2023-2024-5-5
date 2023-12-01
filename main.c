#include <stdio.h>
#include <stdlib.h>
#include "Header.h"
#include <stdbool.h>

taches* CreerArete(taches* sommet,int s1,int s2){
    if(sommet[s1].arc==NULL){
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1].arc=Newarc;
        return sommet;
    }

    else{
        pArc temp=sommet[s1].arc;

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
// créer le t_graphes
t_graphes* Creert_graphes(int ordre){
    t_graphes * Newt_graphes=(t_graphes*)malloc(sizeof(t_graphes));
    Newt_graphes->tache = (taches*)malloc(ordre*sizeof(taches));
    //Newt_graphes->file = (int*)malloc(ordre*sizeof(int));

    for(int i=0; i<ordre; i++){
        Newt_graphes->tache[i]=*(taches*)malloc(sizeof (taches));
        Newt_graphes->tache[i].arc=NULL;
        Newt_graphes->tache[i].marquage = false;

    }

    return Newt_graphes;
}

void afficher_successeurs(taches * tache, int num){
    printf("Tache %d : ",tache[num].numero);

    pArc arc=tache[num].arc;

    while(arc!=NULL){
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}

void graphe_afficher(struct graphe* graphe){
    printf("graphe\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("temps cycle %d\n", graphe->temps_de_cycle),

    printf("listes d'adjacence :\n");

    for (int i=0; i<graphe->ordre; i++)
    {
        afficher_successeurs(graphe->tache, i);
        printf("\n");
    }

}



// donné
// La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
// Le fichier contient : ordre, taille, orientation (0 ou 1)et liste des arcs




void initFile (t_fileDyn * ptAlignement){
    ptAlignement->Maillon_ADefiler = NULL;
    ptAlignement->Maillon_AEnfiler = NULL;
}

int estVide(t_fileDyn* fAttente){
    if (fAttente->Maillon_ADefiler == NULL && fAttente->Maillon_AEnfiler == NULL)
        return 1;

    else
        return 0;
}
// Tester si elle pleine pas n'est pas nécessaire comme c'est une file dynamique
void enfiler(t_fileDyn * ptAlignement, int nouvelElement){
    t_maillon * nouveau;

    //creation d'un nouveau maillon
    nouveau = (t_maillon*)malloc(sizeof(t_maillon));
    if (nouveau == NULL){
        printf("pb d'allocation Dyn. de maillon\n");
    }

    else{
        //affectation de la valeur du maillon
        nouveau->num = nouvelElement;
        nouveau->suivant=NULL;

        //ajout du maillon dans la liste
        if (estVide(ptAlignement) == 1){
            //liste vide
            ptAlignement->Maillon_AEnfiler = nouveau;
            ptAlignement->Maillon_ADefiler = nouveau;
        }

        else{
            //liste non vide donc ajout en tete et mise à jour du pointeur
            nouveau->suivant = ptAlignement->Maillon_AEnfiler;
            ptAlignement->Maillon_AEnfiler = nouveau;
        }
    }
}

int defiler(t_fileDyn * ptAlignement){
    t_maillon* ptcourant = NULL;
    t_maillon* ptprecedent = NULL;
    char res;

    if (estVide(ptAlignement) == 1){
        printf("il n'y a aucun element à supprimer. La file est vide\n");
        return '*';     //return un caractere non majusucle
    }

    else{
        ptcourant = ptAlignement->Maillon_ADefiler;
        res = ptcourant->num;

        //il faut regarder s'il n'y a plus qu'un seul element qui rendra apres cette operation de suppression la liste vide
        if (ptAlignement->Maillon_ADefiler != ptAlignement->Maillon_AEnfiler){
            //parcourir la liste pour atteindre l'avant dernier maillon
            ptcourant = ptAlignement->Maillon_AEnfiler;
            ptprecedent = ptcourant;

            while(ptcourant->suivant!=NULL){
                ptprecedent = ptcourant;
                ptcourant=ptcourant->suivant;
            }

            ptprecedent->suivant = NULL;
            ptAlignement->Maillon_ADefiler = ptprecedent;
        }

        else{
            ptAlignement->Maillon_ADefiler = NULL;
            ptAlignement->Maillon_AEnfiler = NULL;
        }

        free(ptcourant);
        return res;
    }
}

void bfs (t_graphes * graphes, int s_init, int s_fin, int tab[]){
    // création d'un tableau dynamique qui va contenir les marquages des sommets
    int* marquage = (int*)malloc(graphes->ordre * sizeof(int));
    // création d'un tableau dynamique qui va contenir les prédécesseurs des sommets
    int* predecesseur = (int*)malloc(graphes->ordre * sizeof(int));


    // Initialisation d'une file vide
    t_fileDyn file;
    initFile(&file);

    // On ajoute à la file le sommet initial et on le marque
    enfiler(&file, s_init);


    // on cherche quel est le plus grand sommet dans le cas où les sommets du t_graphes ne commencent pas à 0 :

    while (!estVide(&file)) {
        int sommet_courant = defiler(&file); // on enlève de la file le prochain sommet qui va être exploré par le BFS
        printf(" %d", sommet_courant); // on affiche le sommet en question

        // On regarde qui sont les voisins du sommet courant et on les parcourt §
        pArc arc = graphes->tache[sommet_courant].arc;

        while (arc != NULL) { // tant que le sommet à encore des voisins non marqués
            int voisin = arc->sommet; // on récupère le numéro du sommet voisin du sommet courant
            for (int i = 0; i < graphes->tache[voisin].nb_de_prece; ++i) { // parcourir les sommets precedent du sommet voisin et regatrder si c ets true ou non si ils sont tous true ( sous fonction ) on enfile le sommet et on le marque, faire aussi verifier les temsp de cycle

                if (graphes->tache[voisin].marquage == 0) { // on vérifie que le marquage soit égal à 0 (càd qu'il n'est pas déjà été visité)
                    marquage[voisin] = 1; // et on le marque pour montrer qu'il a été visité
                    predecesseur[voisin] = sommet_courant; // on garde en mémoire le numéro du prédécesseur du voisin
                    enfiler(&file, voisin); // dans ce cas, on ajoute le sommet en question à la file
                }

            }

            arc = arc->arc_suivant; // on passe au voisin suivant
        }
    }
    // on parcourt toute la file tant qu'elle n'est pas vide

    printf("\n");

    // on affiche les prédécesseurs de chaque sommet
    printf("Predecesseurs :\n");
    for (int i = 0; i < t_graphes->ordre; i++) {
        if(predecesseur[i] != -1){
            printf("Sommet %d -> Predecesseur : %d\n", i, predecesseur[i]);
        }
    }


    // on affiche le parcours finalement réalisé par le BSF
    printf("Parcours en BSF du sommet %d au sommet %d: \n", s_init, s_fin);

    // Libération de la mémoire allouée
    free(marquage);
    free(predecesseur);
}





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

        creer_tab_exclu(t, s1, s2);
        creer_tab_exclu(t, s2, s1);

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




    ///Fermeture de fichiers
    fclose(opera);
    fclose(tps_cyle);
    fclose(exclu);
    fclose(prece);

    g->tache = t;

    return g;

}

bool fini(taches *t, int ordre)
{
    for (int i = 0; i < ordre; ++i) if (t[i].marquage == false) return false;
    return true;
}


t_station precedances(t_graphes *g)
{
    taches *t = g->tache;
    int ordre = g->ordre;
    int temps_cycle = g->temps_de_cycle;

    t_station* station;
    station = (t_station *)malloc(sizeof(t_station));

    ///Init de la premiere station (creation new fonction)


    int compteur = 0;
    int i;
    int temps;

    while(fini(t, ordre)==false)
    {
        for (i = 0; i < ordre; ++i)
        {
            if (t[i].precedences == -1 && t[i].marquage==false)
            {
                if (t[i]) /// comparaison temps somme et temps station
                printf("Tache depart : %d\n", t[i].numero);
                station[0].tab_station = (int *) malloc(sizeof(int));
                station[0].tab_station[0] = i;
                break;
            }
        }
    }





//    t[i].exclusion = (int *) realloc(t[i].exclusion, t[i].degre * sizeof(int ));


    return *station;

}



int main()
{
    t_graphes *graphe;
    t_station station;

    graphe = lire_fichier();
    graphe_afficher(graphe);
    station = precedances(graphe);


    return 0;
}

