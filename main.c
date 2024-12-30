#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define NOMBRE_VOITURES 20
#define NBRE_TOURS_QUALI 10


typedef struct {
    int numero; //numéro de la voiture
	double temps_S1; //temps secteur 1
	double temps_S2;
	double temps_S3;
	double meilleur_temps;
	bool estOUT;
	bool estStand;
    double tours;
} Voiture;

Voiture voitures[NOMBRE_VOITURES];

//trie les voitures pour afficher en premier la voiture avec le meilleurs temps le plus court et en dernier la voiture avec le meilleur temps le plus long
int sortVoitures(const void *a, const void *b) {
    Voiture *voitureA = (Voiture *)a;
    Voiture *voitureB = (Voiture *)b;

    // Vérifie si l'une des voitures est OUT
    if (voitureA->estOUT && voitureB->estOUT) {
        return 0;  // Les deux sont OUT, garde l'ordre
    }
    if (voitureA->estOUT) {
        return 1;  // voitureA est OUT, elle va en bas
    }
    if (voitureB->estOUT) {
        return -1; // voitureB est OUT, elle va en bas
    }

    // Trie normalement si aucune des deux n'est OUT
    if (voitureA->meilleur_temps < voitureB->meilleur_temps) {
        return -1;
    } else if (voitureA->meilleur_temps > voitureB->meilleur_temps) {
        return 1;
    } else {
        return 0;
    }
}



void init_voitures(const char *nom) {
    int listeVoitures[NOMBRE_VOITURES] = {1, 11, 44, 63, 16, 55, 4, 81, 14, 18, 10, 31, 23, 2, 22, 3, 77, 24, 20, 27};
    if (strcmp(nom, "ALL") == 0){
        for (int i = 0; i < NOMBRE_VOITURES; i++) {
                voitures[i].numero = listeVoitures[i];
                voitures[i].temps_S1 = 0.0;
                voitures[i].temps_S2 = 0.0;
                voitures[i].temps_S3 = 0.0;
                voitures[i].meilleur_temps = 0.0;
                voitures[i].estOUT = false;
                voitures[i].estStand = false;
                voitures[i].tours = 0;
        }
    }else if(strcmp(nom, "Q2") == 0){
        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);
        for (int i = 0; i < NOMBRE_VOITURES-5; i++) {
                voitures[i].numero = listeVoitures[i];
                voitures[i].temps_S1 = 0.0;
                voitures[i].temps_S2 = 0.0;
                voitures[i].temps_S3 = 0.0;
                voitures[i].meilleur_temps = 0.0;
                voitures[i].estOUT = false;
                voitures[i].estStand = false;
                voitures[i].tours = 0;
        }
    }else if(strcmp(nom, "Q3") == 0){
        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);
        for (int i = 0; i < NOMBRE_VOITURES-10; i++) {
                voitures[i].numero = listeVoitures[i];
                voitures[i].temps_S1 = 0.0;
                voitures[i].temps_S2 = 0.0;
                voitures[i].temps_S3 = 0.0;
                voitures[i].meilleur_temps = 0.0;
                voitures[i].estOUT = false;
                voitures[i].estStand = false;
                voitures[i].tours = 0;
        }
    }
}


typedef struct {
    //meilleur temps total et sa voiture
    double meilleur_temps_tour;
    int voiture_temps_tour;
    //meilleur temps s1 total et sa voiture
    double meilleur_S1;
    int voiture_S1;
    //meilleur temps s2 total et sa voiture 
    double meilleur_S2;
    int voiture_S2;
    //meilleur temps s3 total et sa voiture
    double meilleur_S3;
    int voiture_S3;
} MeilleursTemps;

MeilleursTemps meilleursTempsTotal = {0.0, -1, 0.0, -1, 0.0, -1, 0.0, -1};




void afficher_voitures() {
    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        printf("%d. Num %d - S1: %.2f, S2: %.2f, S3: %.2f, Temps total: %.2f, OUT: %d, PIT: %d\n",
               i + 1,
               voitures[i].numero,
               voitures[i].temps_S1,
               voitures[i].temps_S2,
               voitures[i].temps_S3,
               voitures[i].meilleur_temps,
               voitures[i].estOUT,
               voitures[i].estStand);
    }
}




void debutTableau(int numEpreuve, int tour, const char *nomEpreuve) {

    if (strcmp(nomEpreuve, "Essais") == 0){
        printf("╔═══════════════════╦═════════════╗\n");
        printf("║    Essais P%d      ║  TOUR : %-2d  ║\n", numEpreuve, tour);
        printf("╚═══════════════════╩═════════════╝\n");
        printf("┌──────┬───────────┬─────────┬─────────┬─────────┬────────────────┬──────────────────┐\n");
        printf("│      │  Voiture  │   S1    │   S2    │   S3    │  Temps actuel  │  Meilleur temps  │\n");
        printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┤\n");
    }else if (strcmp(nomEpreuve, "Qualification") == 0){
            printf("╔═══════════════════════╦═════════════╗\n");
            printf("║    Qualification %d    ║  TOUR : %-2d  ║\n", numEpreuve, tour);
            printf("╚═══════════════════════╩═════════════╝\n");
            printf("┌──────┬───────────┬─────────┬─────────┬─────────┬────────────────┬──────────────────┬──────────┬─────────┬───────┐\n");
            printf("│      │  Voiture  │   S1    │   S2    │   S3    │  Temps actuel  │  Meilleur temps  │   Diff   │  STAND  │  OUT  │\n");
            printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
    }
}


void ligneTableau(int position, int numVoiture, float s1, float s2, float s3, float meilleurTemps) {
    double tempsTotal = s1 + s2 + s3;

    printf("│ -%-2d- │ %-9d │ %.3f  │ %.3f  │ %.3f  │ %-14.3f │ %-16.3f │\n",position, numVoiture, s1, s2, s3, tempsTotal, meilleurTemps);
}

void ligneTableauQualif(int position, int numVoiture, float s1, float s2, float s3, float meilleurTemps, double diff, bool stand, bool out){
    double tempsTotal = s1 + s2 + s3;
    const char *estOutTableau;
    const char *estStandTableau;
    if(out == 1){
        estOutTableau = " OUT ";
    }else{
        estOutTableau = "     ";
    }

    if(stand == 1){
        estStandTableau = "-------";
    }else{
        estStandTableau = "-------";
    }

    printf("│ -%-2d- │ %-9d │ %.3f  │ %.3f  │ %.3f  │ %-14.3f │ %-16.3f │ %-8.3f │ %s │ %s │\n",position, numVoiture, s1, s2, s3, tempsTotal, meilleurTemps, diff, estStandTableau, estOutTableau);
}

void finTableau(const char *nomEpreuve){
    if (strcmp(nomEpreuve, "Essais") == 0){
        printf("└──────┴───────────┴─────────┴─────────┴─────────┴────────────────┴──────────────────┘\n");
    }else if (strcmp(nomEpreuve, "Qualification") == 0){
        printf("└──────┴───────────┴─────────┴─────────┴─────────┴────────────────┴──────────────────┴──────────┴─────────┴───────┘\n");
    }
}


void meilleurAffichage(){
    printf("Meilleur temps : %.3f  --> voiture %d \n", meilleursTempsTotal.meilleur_temps_tour, meilleursTempsTotal.voiture_temps_tour);
    printf("Meilleur temps S1 : %.3f --> voiture %d \n", meilleursTempsTotal.meilleur_S1, meilleursTempsTotal.voiture_S1);
    printf("Meilleur temps S2 : %.3f --> voiture %d \n", meilleursTempsTotal.meilleur_S2, meilleursTempsTotal.voiture_S2);
    printf("Meilleur temps S3 : %.3f --> voiture %d \n", meilleursTempsTotal.meilleur_S3, meilleursTempsTotal.voiture_S3);
}


double genere_temps(double min, double max){
    return min + (rand() / (RAND_MAX / (max - min)));
}

int genere_nbre_tours(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}


//attribue un temps alléatoire entre 25 et 45 secondes pour chaque section pour chaque voiture qui fait la course
void donne_tempsSecteur(){
    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        if (!voitures[i].estOUT) {
            voitures[i].temps_S1 = genere_temps(25.0, 45.0);
            voitures[i].temps_S2 = genere_temps(25.0, 45.0);
            voitures[i].temps_S3 = genere_temps(25.0, 45.0);
            
            double temps_actuel = voitures[i].temps_S1 + voitures[i].temps_S2 + voitures[i].temps_S3;

            //met à jour le meilleur temps de chaque voiture si il est plus petit que le précédent
            if(temps_actuel < voitures[i].meilleur_temps || voitures[i].meilleur_temps == 0){
                voitures[i].meilleur_temps = temps_actuel;
            }

            if(temps_actuel < meilleursTempsTotal.meilleur_temps_tour || meilleursTempsTotal.meilleur_temps_tour == 0){
                meilleursTempsTotal.meilleur_temps_tour = temps_actuel;
                meilleursTempsTotal.voiture_temps_tour = voitures[i].numero;
            }
            if(voitures[i].temps_S1 < meilleursTempsTotal.meilleur_S1|| meilleursTempsTotal.meilleur_S1 == 0){
                meilleursTempsTotal.meilleur_S1 = voitures[i].temps_S1;
                meilleursTempsTotal.voiture_S1= voitures[i].numero;
            }
            if(voitures[i].temps_S2 < meilleursTempsTotal.meilleur_S2|| meilleursTempsTotal.meilleur_S2 == 0){
                meilleursTempsTotal.meilleur_S2 = voitures[i].temps_S2;
                meilleursTempsTotal.voiture_S2= voitures[i].numero;
            }
            if(voitures[i].temps_S3 < meilleursTempsTotal.meilleur_S3|| meilleursTempsTotal.meilleur_S3 == 0){
                meilleursTempsTotal.meilleur_S3 = voitures[i].temps_S3;
                meilleursTempsTotal.voiture_S3= voitures[i].numero;
            }
        }
    }
}

//lance un essais
void essais(int numEssais, int tours_nombre_max){
    
    for(int tour = 1; tour < tours_nombre_max; tour++){
        system("clear");
        debutTableau(numEssais, tour, "Essais");
        donne_tempsSecteur();

        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);

        int positionCourse = 0;

        for (int i = 0; i < NOMBRE_VOITURES; i++) {
            positionCourse += 1;
            ligneTableau(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps);
        }
        finTableau("Essais");

        meilleurAffichage();
        sleep(2);
    }
}



void qualificaiton(int numQuali){
    for(int tour = 1; tour < NBRE_TOURS_QUALI + 1; tour++){
        system("clear");
        debutTableau(numQuali, tour, "Qualification");
        donne_tempsSecteur();

        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);

        int positionCourse = 0;

        if(numQuali == 1){

            for (int i = 0; i < NOMBRE_VOITURES - 5; i++) {
                positionCourse += 1;

                double tempsDiff = 0.0;
                if(i > 0){
                    tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;
                }

                ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].estStand, voitures[i].estOUT);
            }

            printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
            for (int i = NOMBRE_VOITURES - 5; i < NOMBRE_VOITURES; i++) {
                positionCourse += 1;

                double tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;

                if(tour == 10){
                    voitures[i].estOUT = true;
                }
                ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].estStand, voitures[i].estOUT);
            }
        }else{
            if(numQuali == 2 || numQuali == 3){

                for (int i = 0; i < NOMBRE_VOITURES - 10; i++) {
                    positionCourse += 1; 

                    double tempsDiff = 0.0;
                    if(i > 0){
                        tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;
                    }

                    ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].estStand, voitures[i].estOUT);
                }

                printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
                for (int i = NOMBRE_VOITURES - 10; i < NOMBRE_VOITURES; i++) {
                    positionCourse += 1;

                    double tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;

                    if(tour == 10){
                        voitures[i].estOUT = true;
                    }
                    ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].estStand, voitures[i].estOUT);
                }
            }
        }

        finTableau("Qualification");

        meilleurAffichage();
        sleep(2);
    }
}







int main() {
    srand(time(NULL));
    init_voitures("ALL");
    int nbreTours = genere_nbre_tours(5, 7);
    essais(1, nbreTours);
    init_voitures("ALL");
    qualificaiton(1);
    init_voitures("Q2");
    qualificaiton(2);
    init_voitures("Q3");
    qualificaiton(3);
    return 0;
}