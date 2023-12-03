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


void BFS(t_graphes *g);

// Function to assign a task to a station
void assignToStation(t_graphes *g, t_station *station, taches *task);

// Function to free allocated memory
void freeMemory(t_graphes *g);

int main() {
    // Read input files and create the graph
    t_graphes *graph = lire_fichier();

    // Perform BFS to assign tasks to stations
    BFS(graph);

    // Output the results or further processing as needed
    // (You need to implement this based on your requirements)

    // Cleanup and free memory
    freeMemory(graph);

    return 0;
}

void BFS(t_graphes *g) {
    int *queue = (int *)malloc(g->ordre * sizeof(int));
    int front = -1, rear = -1;
    g->ancre = creer_station();

    // Enqueue all tasks with no dependencies
    for (int i = 0; i < g->ordre; ++i) {
        if (g->tache[i].nb_de_prece == 0) {
            queue[++rear] = i;
        }
    }

    // Perform BFS
    while (front != rear) {
        int currentTaskIndex = queue[++front];

        // Process the task
        assignToStation(g, g->ancre, &g->tache[currentTaskIndex]);

        // Update time and mark the task as visited
        // (You need to implement this based on your data structures)

        // Enqueue tasks that can now be processed
        for (int i = 0; i < g->ordre; ++i) {
            if (g->tache[i].nb_de_prece > 0) {
                g->tache[i].nb_de_prece--;

                if (g->tache[i].nb_de_prece == 0) {
                    queue[++rear] = i;
                }
            }
        }
    }

    free(queue);
}

void assignToStation(t_graphes *g, t_station *station, taches *task) {
    printf("Assigning task %d to station.\n", task->numero);

    // Check if station is null
    if (station == NULL) {
        printf("Error: Station pointer is null.\n");
        return;
    }

    // Debug print: Print the initial state of the station
    printf("Initial station: ordre=%d, temps_tot=%.2f\n", station->ordre, station->temps_tot);

    // Traverse the list to find the last station
    while (station->suivant != NULL) {
        station = station->suivant;
    }

    // Debug print: Print the final state of the station after traversal
    printf("Final station after traversal: ordre=%d, temps_tot=%.2f\n", station->ordre, station->temps_tot);

    // Allocate memory for tab_station if not already allocated
    if (station->tab_station == NULL) {
        station->tab_station = (int *)malloc(g->ordre * sizeof(int));
        if (station->tab_station == NULL) {
            printf("Error: Memory allocation failed.\n");
            return;
        }
        // Debug print: Indicate that memory allocation was successful
        printf("Memory allocated for tab_station.\n");
    }

    // Debug print: Print the assigned task information
    printf("Task assigned: numero=%d, temps=%.2f\n", task->numero, task->temps);

    // Assuming each station has a time limit, check if the task fits within the time constraint
    if (station->temps_tot + task->temps <= g->temps_de_cycle) {
        // Debug print: Indicate that the task fits into the time constraint
        printf("Task fits into the time constraint. Assigning...\n");

        // Assign the task to the station
        station->tab_station[station->ordre++] = task->numero;

        // Update the total time of the station
        station->temps_tot += task->temps;

        // Debug print: Print the updated state of the station after task assignment
        printf("Updated station after assignment: ordre=%d, temps_tot=%.2f\n", station->ordre, station->temps_tot);
    } else {
        // Debug print: Indicate that the task doesn't fit into the time constraint
        printf("Task doesn't fit into the time constraint. Creating a new station...\n");

        // If the task doesn't fit within the time constraint, create a new station
        t_station *newStation = creer_station();


        // Assign the task to the new station
        newStation->tab_station[newStation->ordre++] = task->numero;

        // Update the total time of the new station
        newStation->temps_tot += task->temps;

        // Link the new station to the existing station list
        station->suivant = newStation;

        // Debug print: Print the state of the new station after creation
        printf("New station created: ordre=%d, temps_tot=%.2f\n", newStation->ordre, newStation->temps_tot);
    }
}


void freeMemory(t_graphes *g) {
    // Freeing allocated memory for the tache array
    free(g->tache);

    // Additional cleanup based on your data structures
    // ...

    // Freeing memory for the graph structure
    free(g);
}


