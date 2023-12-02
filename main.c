#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include <stdbool.h>

taches* CreerArete(taches* sommet,int s1,int s2)
{
    int i = 0;
    while(sommet[i].numero != s1) i++;

    if(sommet[i].arc==NULL){
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[i].arc=Newarc;
        return sommet;
    }

    else{
        pArc temp=sommet[i].arc;

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

    printf("temps cycle %f\n", graphe->temps_de_cycle),

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
    ptAlignement->ordre = 0;
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
    int res;

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
bool fin_predecesseur(taches *t, int voisin, int current)
{
    printf("l indice est %d\n",voisin);
    for (int i = 0; i < t[voisin].nb_de_prece; ++i) {
        // printf("nombre de precedence du sommet %d est %d\n", t[voisin].numero,t[voisin].nb_de_prece);
        //printf("\n sommet precedent de %d est %d\n",t[voisin].numero,t[voisin].precedences[i]);
        int y=0;
        while(t[y].numero != t[voisin].precedences[i]) y++;
        //printf("l indice du precedent est : %d\n",y);
        // printf("le boolen de la precedence %d est : %d\n",t[voisin].precedences[i],t[y].marquage);

        if (t[y].marquage == false)
            return false;
    }
    return true;
}

bool somme(t_fileDyn *file,taches* t,t_graphes *g,int curent){

    float i=0.0;

    t_maillon *actual = file->Maillon_AEnfiler;
    while(actual!=NULL){
        i += t[actual->num].temps;
        //printf("somme actuel file %f apres l ajout du sommet %d \n",i, actual->num);
        actual = actual->suivant;
    }
    i += t[curent].temps;
    printf("le temps global de la file est : %f\n",i);
    if (i <= g->temps_de_cycle)return 1;
    return 0;
}
void afficher_f(t_fileDyn *file){
    t_maillon *actual = file->Maillon_AEnfiler;
    while(actual!=NULL){
        printf("l etat de la file %d\n",actual->num);
        actual = actual->suivant;
    }

}
void afficher_s(t_station* s, int num,t_graphes *g){
    int numero=1;
    t_station *actual = g->ancre;
//    if (actual!=NULL) printf("ererer");
    while(actual!=NULL){
//        printf("ordre de s : %d", s->ordre);
        for (int i = 0; i < actual->ordre; ++i) {
            printf("l etat de la station %d  est %d\n",numero,g->tache[actual->tab_station[i]].numero );
        }


        numero++;
        //actual->tab_station[g->tache[i].numero]
        actual = actual->suivant;

    }

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

void bfs (t_graphes * graphes, int s_init,int cmpt2, t_station* s,t_fileDyn *file){
    // création d'un tableau dynamique qui va contenir les marquages des sommets
    int* marquage = (int*)malloc(graphes->ordre * sizeof(int));
    // création d'un tableau dynamique qui va contenir les prédécesseurs des sommets
    int* predecesseur = (int*)malloc(graphes->ordre * sizeof(int));
    int cmpt=1;
    bool exit = false;

    int indice=1;

    while (s->suivant!=NULL)
    {
        s = s->suivant;
        indice++;
    }


    // Initialisation d'une file vide


    // On ajoute à la file le sommet initial et on le marque
    //enfiler(file, s_init);


    // on cherche quel est le plus grand sommet dans le cas où les sommets du t_graphes ne commencent pas à 0 :
    exit=false;
    //while (!estVide(file))
    {
        int sommet_courant = s_init; // on enlève de la file le prochain sommet qui va être exploré par le BFS
        printf(" indice sommet courant %d ", sommet_courant); // on affiche le sommet en question

        // On regarde qui sont les voisins du sommet courant et on les parcourt §
        pArc arc = graphes->tache[sommet_courant].arc;

        for (int i = 0; i <graphes->ordre; ++i)
        {
            pArc arc1 = graphes->tache[i].arc;
//            if (arc1!=NULL) printf("il y a un arc entre %d  et : %d  indice %d \n",graphes->tache[i].numero,arc1->sommet,i);
//            else printf("FIN de %d   indice %d\n",graphes->tache[i].numero, i );
        }
        if(arc == NULL) printf(" on est sur un arc null");

        while (arc!= NULL)
        {

            cmpt2++;
            printf(" nombre de tour de boucle %d\n",cmpt2);
            //printf("on est dans bfs \n\n");

            // tant que le sommet à encore des voisins non marqué
            int voisin = arc->sommet;

//            printf(" \nvoisin n            %d\n",voisin);
            int i = 0;
            while(graphes->tache[i].numero != voisin) i++;
            printf("sommet courrant %d a indice %d\n",voisin,i);
//            printf(" correspond au sommet : %d son sommet suivant est  %d\n",graphes->tache[sommet_courant].numero,graphes->tache[i].numero);// on récupère le numéro du sommet voisin du sommet courant


            //fin_predecesseur(graphes->tache,i, sommet_courant);
            if(fin_predecesseur(graphes->tache,i, sommet_courant)==true)
            {
                //printf("sale chien");

                if(somme(file,graphes->tache,graphes,i)==1)
                {
                    enfiler(file, i);
//                    printf("enfiler\n");
                    afficher_f(file);
                    file->ordre++;
                    graphes->tache[i].marquage=true;

                }
                else
                {
//                    printf("le temps est superieur\n");

                    s->tab_station = (int*)malloc((file->ordre)*sizeof(int));
                    s->ordre = file->ordre;
                    afficher_f(file);

//                    printf("ordre de la file : %d", file->ordre);
                    for (int j = 0; j < file->ordre; ++j) s->tab_station[j]=defiler(file);

                    afficher_s(s, indice, graphes);
/*
                    printf("MARQUAGEEEE new de 1  %d\n", graphes->tache[0].marquage);
                    printf("MARQUAGEEEE de 2  %d\n", graphes->tache[1].marquage);
                    printf("MARQUAGEEEE de 3  %d\n", graphes->tache[2].marquage);
                    printf("MARQUAGEEEE de 4  %d\n", graphes->tache[3].marquage);
                    printf("MARQUAGEEEE de 5  %d\n", graphes->tache[4].marquage);
                    printf("MARQUAGEEEE de 6  %d\n", graphes->tache[5].marquage);
*/

                    file->ordre=0;
                    s->suivant = creer_station();
                    s = s->suivant;


                    ///enfiler le nouveau sommet
                    enfiler(file, i);
                    graphes->tache[i].marquage=true;
                    file->ordre++;



                    /*
                    //cmpt2++;
                    //s = (t_station*) realloc(s, cmpt2*sizeof (t_station));

                    //s[cmpt2].tab_station=(int*)malloc(cmpt*sizeof(int));

                    for (int j = 0; j < cmpt; ++j)
                    {
                        s[cmpt2].tab_station[j]=defiler(file);
                    }
                    cmpt=0;
                    graphes->tache[i].marquage=true;
                    */
                }



                arc = arc->arc_suivant; // on passe au voisin suivant
            }
            else
            {
                exit = true;
                break;
            }
            break;




        }

    }
    // on parcourt toute la file tant qu'elle n'est pas vide

    printf("\n");


    // on affiche les prédécesseurs de chaque sommet


/*
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
    */
}


/*
if (graphes->tache[voisin].marquage == 0) { // on vérifie que le marquage soit égal à 0 (càd qu'il n'est pas déjà été visité)
marquage[voisin] = 1; // et on le marque pour montrer qu'il a été visité
predecesseur[voisin] = sommet_courant; // on garde en mémoire le numéro du prédécesseur du voisin
enfiler(&file, voisin); // dans ce cas, on ajoute le sommet en question à la file
}
*/

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
        g->tache=CreerArete(g->tache,s1,s2);
        creer_tab_prece(t, s1, s2);

        pos = ftell(prece);
    }

    for (int i = 0; i < g->ordre; ++i) if(g->tache[i].arc==NULL) g->tache=CreerArete(g->tache,g->tache[i].numero,-2);;



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

bool fini(taches *t, int ordre)
{
    for (int i = 0; i < ordre; ++i) if (t[i].marquage == false) return false;
    return true;
}



t_station precedances(t_graphes *g)
{
    taches *t = g->tache;
    int ordre = g->ordre;
    float temps_cycle = g->temps_de_cycle;


    g->ancre = creer_station();


    int compteur = 0;
    int i=0;
    int temps;

    t_station * actual = g->ancre;
    t_fileDyn file;
    initFile(&file);


    while(fini(t, ordre)==false)
    {

        for (i = 0; i < ordre; ++i)
        {
            if (t[i].precedences == -1 && t[i].marquage == false)
            {
                printf("coucouuuuuu %d\n", i);
                /*
                if ((t[i].temps + actual->temps_tot) > temps_cycle)
                {
                    actual->suivant = creer_station();
                    actual = actual->suivant;
                }
                 */


//                printf("Tache depart : %d\n", t[i].numero);
                if (actual->ordre!=0)
                {
                    enfiler(&file,i);
                    file.ordre++;
//                    actual->tab_station = (int *) realloc(actual->tab_station, ((actual->ordre)+1)* sizeof(int ));
//                    actual->tab_station[actual->ordre] = i;
                }
                else
                {
                    enfiler(&file,i);
                    file.ordre++;

//                    actual->tab_station[actual->ordre] = i;
                }

//                actual->ordre++;
//                actual->temps_tot = actual->temps_tot + t[i].temps;
                t[i].marquage = true;
                bfs(g,i,g->ordre,actual,&file);

                break;


            }
//            if (i == ordre-1) break;

        }


       printf("marquageeee de %d = %d", i, t[i].marquage);
//        printf("Ce projet pue la merde\n");
       // printf("\niciiiii %d\n", i);

/*
        printf("MARQUAGEEEE de 1  %d\n", t[0].marquage);
        printf("MARQUAGEEEE de 2  %d\n", t[1].marquage);
        printf("MARQUAGEEEE de 3  %d\n", t[2].marquage);
        printf("MARQUAGEEEE de 4  %d\n", t[3].marquage);
        printf("MARQUAGEEEE de 5  %d\n", t[4].marquage);
        printf("MARQUAGEEEE de 6  %d\n", t[5].marquage);
*/
        //afficher_f(&file);



/*
        printf("paketa sevran : %f\n", actual->temps_tot);

        for (int j = 0; j < actual->ordre; ++j) {
            printf("%d ", actual->tab_station[j]);

        }

        printf("\n");
        */

    }





//    t[i].exclusion = (int *) realloc(t[i].exclusion, t[i].degre * sizeof(int ));


    return *actual;

}



int main()
{
    t_graphes *graphe;
    t_station station;

    graphe = lire_fichier();
//    graphe_afficher(graphe);
    station = precedances(graphe);


    return 0;
}