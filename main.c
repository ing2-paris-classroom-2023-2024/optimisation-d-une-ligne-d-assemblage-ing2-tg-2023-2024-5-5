#include <stdio.h>
#include <stdbool.h>

// Nombre maximal d'opérations
#define MAX_OPERATIONS 35
#define MAX_EXCLUSIONS 100

// Structure pour représenter une paire d'opérations exclues
typedef struct {
    int operation1;
    int operation2;
} Exclusion;

// Fonction pour vérifier si une paire d'opérations est exclue
bool Exclue(Exclusion exclusions[], int nombreExclusions, int op1, int op2) {
    for (int i = 0; i < nombreExclusions; ++i) {
        if ((exclusions[i].operation1 == op1 && exclusions[i].operation2 == op2) ||
            (exclusions[i].operation1 == op2 && exclusions[i].operation2 == op1)) {
            return true;  // La paire est exclue
        }
    }
    return false;  // La paire n'est pas exclue
}

int welchPowell(Exclusion exclusions[], int nombreExclusions, int nombreOperations) {
    // Initialisation des degrés des opérations à 0
    int degres[MAX_OPERATIONS] = {0};

    // Calcul des degrés en tenant compte des exclusions
    for (int i = 0; i < nombreExclusions; ++i) {
        degres[exclusions[i].operation1 - 1]++;
        degres[exclusions[i].operation2 - 1]++;
    }

    // Tableau pour stocker les opérations triées par degré décroissant
    int operationsTriees[MAX_OPERATIONS];

    // Initialisation du tableau des opérations triées
    for (int i = 0; i < nombreOperations; ++i) {
        operationsTriees[i] = i + 1;  // Les opérations sont numérotées à partir de 1
    }

    // Tri des opérations par degré décroissant (utilisation d'un simple tri à bulles)
    for (int i = 0; i < nombreOperations - 1; ++i) {
        for (int j = 0; j < nombreOperations - i - 1; ++j) {
            if (degres[operationsTriees[j] - 1] < degres[operationsTriees[j + 1] - 1]) {
                // Échange
                int temp = operationsTriees[j];
                operationsTriees[j] = operationsTriees[j + 1];
                operationsTriees[j + 1] = temp;
            }
        }
    }

    // Le nombre de stations est le degré maximum
    int nombreStations = degres[operationsTriees[0] - 1];

    return nombreStations;
}

int algorithmeNaif(Exclusion exclusions[], int nombreExclusions, int nombreOperations) {
    // Nombre initial de stations est 0
    int nombreStations = 0;

    // Tableau pour suivre la station attribuée à chaque opération
    int stations[MAX_OPERATIONS] = {0};

    // Pour chaque paire d'opérations exclues
    for (int i = 0; i < nombreExclusions; ++i) {
        int operation1 = exclusions[i].operation1;
        int operation2 = exclusions[i].operation2;

        // Si les opérations ne sont pas encore attribuées à une station
        if (stations[operation1 - 1] == 0 && stations[operation2 - 1] == 0) {
            // Attribuer une nouvelle station aux deux opérations
            nombreStations++;
            stations[operation1 - 1] = nombreStations;
            stations[operation2 - 1] = nombreStations;
        } else if (stations[operation1 - 1] == 0) {
            // Si seulement l'opération 1 n'est pas attribuée, attribuer la même station que l'opération 2
            stations[operation1 - 1] = stations[operation2 - 1];
        } else if (stations[operation2 - 1] == 0) {
            // Si seulement l'opération 2 n'est pas attribuée, attribuer la même station que l'opération 1
            stations[operation2 - 1] = stations[operation1 - 1];
        } else if (stations[operation1 - 1] != stations[operation2 - 1]) {
            // Si les deux opérations sont attribuées à des stations différentes, fusionner les stations
            int station1 = stations[operation1 - 1];
            int station2 = stations[operation2 - 1];

            for (int j = 0; j < nombreOperations; ++j) {
                if (stations[j] == station2) {
                    stations[j] = station1;
                }
            }
        }
    }

    return nombreStations;
}


int main() {
    //lecture des exclusions à partir d'un fichier
    FILE *fichierExclusions = fopen("exclusions.txt", "r");
    if (fichierExclusions == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'exclusions");
        return 1;
    }

    Exclusion exclusions[MAX_EXCLUSIONS];
    int nombreExclusions = 0;

    // Lecture des paires d'exclusion depuis le fichier
    while (fscanf(fichierExclusions, "%d %d", &exclusions[nombreExclusions].operation1, &exclusions[nombreExclusions].operation2) == 2) {
        nombreExclusions++;
        if (nombreExclusions >= MAX_EXCLUSIONS) {
            printf("Attention : le nombre maximal d'exclusions a ete atteint. Certaines exclusions peuvent ne pas avoir ete prises en compte.\n");
            break;
        }
    }

    fclose(fichierExclusions);

    //vérification si une paire d'opérations est exclue
    int op1 = 1;
    int op2 = 13;
    if (Exclue(exclusions, nombreExclusions, op1, op2)) {
        printf("Les operations %d et %d ne peuvent pas etre affectees a la meme station.\n", op1, op2);
    } else {
        printf("Les operations %d et %d peuvent etre affectees a la meme station.\n", op1, op2);
    }
    // Exemple d'utilisation de l'algorithme de Welch-Powell
    int nombreStations = welchPowell(exclusions, nombreExclusions, MAX_OPERATIONS);
    printf("Le nombre minimum de stations selon l'algorithme de Welch-Powell est : %d\n", nombreStations);

    // Exemple d'utilisation de l'algorithme naïf
    int nombreStationsNaif = algorithmeNaif(exclusions, nombreExclusions, MAX_OPERATIONS);
    printf("Le nombre minimum de stations selon l'algorithme naif est : %d\n", nombreStationsNaif);


    return 0;
}