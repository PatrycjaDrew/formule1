#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define NOMBRE_VOITURES 20
#define ESSAIS_TEMPS 60  // 1h
#define QUALIF1_TEMPS 18 // 18 min

typedef struct {
    int numero; //numéro de la voiture
	double temps_S1; //temps secteur 1
	double temps_S2;
	double temps_S3;
	double meilleur_temps;
	bool estOUT;
	bool estStand;
    double tours;
    double standTemps;
} Voiture;


Voiture voitures[NOMBRE_VOITURES];

double genere_temps(double min, double max){
    return min + (rand() / (RAND_MAX / (max - min)));
}

void init_voitures() {
    int listeVoitures[NOMBRE_VOITURES] = {1, 11, 44, 63, 16, 55, 4, 81, 14, 18, 10, 31, 23, 2, 22, 3, 77, 24, 20, 27};
        for (int i = 0; i < NOMBRE_VOITURES; i++) {
            voitures[i].numero = listeVoitures[i];
            voitures[i].temps_S1 = 0.0;
            voitures[i].temps_S2 = 0.0;
            voitures[i].temps_S3 = 0.0;
            voitures[i].meilleur_temps = 0.0;
            voitures[i].estOUT = false;
            voitures[i].estStand = false;
            voitures[i].tours = 0;
            voitures[i].standTemps = 0.0;
        }
}

//trie les voitures en fonction de leur meilleur temps 
int sortVoitures(const void *a, const void *b) {
    Voiture *voitureA = (Voiture *)a;
    Voiture *voitureB = (Voiture *)b;

    // Vérifie si l'une des voitures est OUT
    if (voitureA->estOUT && voitureB->estOUT) {
        return 0;  // si les 2 sont out, on fait rien 
    }
    if (voitureA->estOUT) {
        return 1;  // si la voiture A est out, elle descend du classement 
    }
    if (voitureB->estOUT) {
        return -1; // si la voiture B est out, elle descend du classement 
    }

    // trie si aucune voiture out
    if (voitureA->meilleur_temps < voitureB->meilleur_temps) {
        return -1;
    } else if (voitureA->meilleur_temps > voitureB->meilleur_temps) {
        return 1;
    } else {
        return 0;
    }
}



void afficherResultat(const char *nomEpreuve, int temps) {
    system("clear");
    printf("╔════════════════╦═════════════════╗\n");
    printf("║%-10s           ║  TEMPS : 00:%-2d  ║\n", nomEpreuve, temps);
    printf("╚════════════════╩═════════════════╝\n");
    printf("Voiture     S1        S2          S3           Meilleur Temps\n");
    printf("─────────────────────────────────────────────────────────────────────\n");
    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        printf("%-10d %-10.3f %-10.3f %-10.3f %-15.3f\n",
            voitures[i].numero,
            voitures[i].temps_S1,
            voitures[i].temps_S2,
            voitures[i].temps_S3,
            voitures[i].meilleur_temps);
    }
}

void essais() {
    time_t debut = time(NULL);
    time_t actuel;
    int temps_ecoule = (int)difftime(actuel, debut);
    while (temps_ecoule < ESSAIS_TEMPS) {
        actuel = time(NULL);
        temps_ecoule = (int)difftime(actuel, debut);
        for (int i = 0; i < NOMBRE_VOITURES; i++) {
            if (!voitures[i].estOUT) {
                voitures[i].temps_S1 = genere_temps(25.0, 45.0);
                voitures[i].temps_S2 = genere_temps(25.0, 45.0);
                voitures[i].temps_S3 = genere_temps(25.0, 45.0);
                double temps_total = voitures[i].temps_S1 + voitures[i].temps_S2 + voitures[i].temps_S3;
                if (temps_total < voitures[i].meilleur_temps || voitures[i].meilleur_temps == 0) {
                    voitures[i].meilleur_temps = temps_total;
                }
            }
        }
        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);
        afficherResultat("Essais",1);
        sleep(1);
    }
}


void qualification() {
    time_t debut = time(NULL);
    time_t actuel;
    int temps_ecoule = (int)difftime(actuel, debut);

    while (temps_ecoule < QUALIF1_TEMPS) {
        actuel = time(NULL);
        temps_ecoule = (int)difftime(actuel, debut);
        for (int i = 0; i < NOMBRE_VOITURES; i++) {
            if (!voitures[i].estOUT) {
                voitures[i].temps_S1 = genere_temps(25.0, 45.0);
                voitures[i].temps_S2 = genere_temps(25.0, 45.0);
                voitures[i].temps_S3 = genere_temps(25.0, 45.0);
                double temps_total = voitures[i].temps_S1 + voitures[i].temps_S2 + voitures[i].temps_S3;
                if (temps_total < voitures[i].meilleur_temps || voitures[i].meilleur_temps == 0) {
                    voitures[i].meilleur_temps = temps_total;
                }
            }
        }
        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);
        afficherResultat("Qualification", temps_ecoule);
        sleep(1);
    }
}


void enregistrerResultats(const char *nomEpreuve, int numEpreuve) {
    FILE *fichier;
    char nomFichier[50];

    int positionCourse = 0;

    sprintf(nomFichier, "resultats_%s_%d.txt", nomEpreuve, numEpreuve);

    fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fprintf(fichier, "Résultats %s %d :\n", nomEpreuve, numEpreuve);
    fprintf(fichier, "┌──────┬───────────┬─────────┬─────────┬─────────┬──────────────────┬─────────┬───────┐\n");
    fprintf(fichier, "│      │  Voiture  │   S1    │   S2    │   S3    │  Meilleur temps  │  STAND  │  OUT  │\n");
    fprintf(fichier, "├──────┼───────────┼─────────┼─────────┼─────────┼──────────────────┼─────────┼───────┤\n");

    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        positionCourse += 1;
        fprintf(fichier, "│ -%-2d- │ %-9d │ %.3f  │ %.3f  │ %.3f  │ %-16.3f │ %d │ %d │\n", positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, voitures[i].estStand, voitures[i].estOUT);
    }
    fprintf(fichier, "└──────┴───────────┴─────────┴─────────┴─────────┴──────────────────┴─────────┴───────┘\n");

    fclose(fichier);
}


int main() {
    srand(time(NULL));
    init_voitures();

    pid_t epreuve = fork();

    if (epreuve < 0) {
        perror("Erreur fork");
        exit(EXIT_FAILURE);
    } else if (epreuve == 0) {
        essais();
        enregistrerResultats("Essais", 1);
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
        qualification();
        enregistrerResultats("Qualification", 2);
    }

    return 0;
}
