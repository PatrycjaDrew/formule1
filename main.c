#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define NOMBRE_VOITURES 20
#define NBRE_TOURS_COURSE 10

#define ESSAIS_TEMPS 3600 
#define QUALIF1_TEMPS 1080 // 18 minutes
#define QUALIF2_TEMPS 900  // 15 minutes
#define QUALIF3_TEMPS 720  // 12 minutes
#define ACCELERATION 60 // Accélération du temps



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



void enregistrerResultats(const char *nomEpreuve, int numEpreuve) {
    FILE *fichier;
    char nomFichier[50];

    qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);
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
                voitures[i].standTemps = 0.0;
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
                voitures[i].standTemps = 0.0;
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
                voitures[i].standTemps = 0.0;
        }
    }
}

void init_stand(){
    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        voitures[i].standTemps = 0.0;
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


void init_tempsTotal() {
    meilleursTempsTotal.meilleur_temps_tour = 0.0;
    meilleursTempsTotal.voiture_temps_tour = -1;
    meilleursTempsTotal.meilleur_S1 = 0.0;
    meilleursTempsTotal.voiture_S1 = -1;
    meilleursTempsTotal.meilleur_S2 = 0.0;
    meilleursTempsTotal.voiture_S2 = -1;
    meilleursTempsTotal.meilleur_S3 = 0.0;
    meilleursTempsTotal.voiture_S3 = -1;
}



void afficher_voitures() {
    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        printf("%d. Num %d - S1: %.3f, S2: %.3f, S3: %.3f, Temps total: %.3f, OUT: %d, PIT: %d, Stand Temps: %.3f\n",
               i + 1,
               voitures[i].numero,
               voitures[i].temps_S1,
               voitures[i].temps_S2,
               voitures[i].temps_S3,
               voitures[i].meilleur_temps,
               voitures[i].estOUT,
               voitures[i].estStand,
               voitures[i].standTemps);
    }
}




void debutTableau(int numEpreuve, int tour, const char *nomEpreuve) {

    if (strcmp(nomEpreuve, "Essais") == 0){
        printf("╔═══════════════════╦═════════════════╗\n");
        printf("║    Essais P%d      ║  TEMPS : 00:%-2d  ║\n", numEpreuve, tour);
        printf("╚═══════════════════╩═════════════════╝\n");
        printf("┌──────┬───────────┬─────────┬─────────┬─────────┬────────────────┬──────────────────┐\n");
        printf("│      │  Voiture  │   S1    │   S2    │   S3    │  Temps actuel  │  Meilleur temps  │\n");
        printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┤\n");
    }else if (strcmp(nomEpreuve, "Qualification") == 0){
            printf("╔═══════════════════════╦═════════════════╗\n");
            printf("║    Qualification %d    ║  TEMPS : 00:%-2d  ║\n", numEpreuve, tour);
            printf("╚═══════════════════════╩═════════════════╝\n");
            printf("┌──────┬───────────┬─────────┬─────────┬─────────┬────────────────┬──────────────────┬──────────┬─────────┬───────┐\n");
            printf("│      │  Voiture  │   S1    │   S2    │   S3    │  Temps actuel  │  Meilleur temps  │   Diff   │  STAND  │  OUT  │\n");
            printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
    }else if (strcmp(nomEpreuve, "Course") == 0){
            printf("╔══════════════╦═══════════════╗\n");
            printf("║    Course    ║  Tours : %-2d  ║\n", tour);
            printf("╚══════════════╩═══════════════╝\n");
            printf("┌──────┬───────────┬─────────┬─────────┬─────────┬────────────────┬──────────────────┬──────────┬─────────┬───────┐\n");
            printf("│      │  Voiture  │   S1    │   S2    │   S3    │  Temps actuel  │  Meilleur temps  │   Diff   │  STAND  │  OUT  │\n");
            printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
    }
}


void ligneTableau(int position, int numVoiture, float s1, float s2, float s3, float meilleurTemps) {
    double tempsTotal = s1 + s2 + s3;

    printf("│ -%-2d- │ %-9d │ %.3f  │ %.3f  │ %.3f  │ %-14.3f │ %-16.3f │\n",position, numVoiture, s1, s2, s3, tempsTotal, meilleurTemps);
}

void ligneTableauQualif(int position, int numVoiture, float s1, float s2, float s3, float meilleurTemps, double diff, double stand, bool out){
    double tempsTotal = s1 + s2 + s3 + stand;
    const char *estOutTableau;
    const char *estStandTableau;
    if(out == 1){
        estOutTableau = " OUT ";
        diff = -1;
    }else{
        estOutTableau = "     ";
        
    }

    char standChar[10];
    if (stand > 0) {
        snprintf(standChar, sizeof(standChar), "%.3f ", stand);
        estStandTableau = standChar;
    } else {
        estStandTableau = "-------";
    }


    printf("│ -%-2d- │ %-9d │ %.3f  │ %.3f  │ %.3f  │ %-14.3f │ %-16.3f │ %-8.3f │ %s │ %s │\n",position, numVoiture, s1, s2, s3, tempsTotal, meilleurTemps, (out ? 0.0 : diff), estStandTableau, estOutTableau);
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


void arret_stand(int tour, int dureeTotale) {
    double temps = 0.0;
    int voiture;

    if(tour < dureeTotale / ACCELERATION - 4){
        do {
            voiture = genere_nbre_tours(0, NOMBRE_VOITURES);
        } while(voitures[voiture].estOUT); 

        temps = genere_temps(25, 30);
        
        voitures[voiture].estStand = true;
        voitures[voiture].standTemps = temps;
    }else{
        for(int i = 0; i < NOMBRE_VOITURES; i++){
            if(voitures[i].estOUT == false && voitures[i].estStand == false){
                temps = genere_temps(25, 30);
                voitures[i].estStand = true;
                voitures[i].standTemps = temps;
            }
        }
    }
}



//attribue un temps alléatoire entre 25 et 45 secondes pour chaque section pour chaque voiture qui fait la course
void donne_tempsSecteur(int tour, int dureeTotale){
    arret_stand(tour, dureeTotale);
    for (int i = 0; i < NOMBRE_VOITURES; i++) {
        if (!voitures[i].estOUT) {
            voitures[i].temps_S1 = genere_temps(25.0, 45.0);
            voitures[i].temps_S2 = genere_temps(25.0, 45.0);
            voitures[i].temps_S3 = genere_temps(25.0, 45.0);
            
            double temps_actuel = voitures[i].temps_S1 + voitures[i].temps_S2 + voitures[i].temps_S3 + voitures[i].standTemps;

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
void essais(int numEssais) {
    time_t debut = time(NULL); // Heure de début de la session
    time_t actuel;

    int tour = 0;
    while (1) {
        actuel = time(NULL);
        int temps_actuel = (int)difftime(actuel, debut);

        // Arrêt de la session après 1h
        if (temps_actuel >= ESSAIS_TEMPS / ACCELERATION) {
            break;
        }

        // Simuler un tour
        tour++;
        system("clear");
        debutTableau(numEssais, temps_actuel, "Essais");
        donne_tempsSecteur(temps_actuel, ESSAIS_TEMPS);

        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);

        int positionCourse = 0;
        for (int i = 0; i < NOMBRE_VOITURES; i++) {
            positionCourse++;
            ligneTableau(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps);
        }
        finTableau("Essais");

        meilleurAffichage();

        sleep(2); 
    }

    enregistrerResultats("Essais", numEssais);
}






void qualificaiton(int numQuali){
    time_t debut = time(NULL);
    time_t actuel;
    int tour = 0;

    int dureeQualif;
    int voituresOut;

    // Définir la durée et le nombre de voitures restantes en fonction de la qualification
    if (numQuali == 1) {
        dureeQualif = QUALIF1_TEMPS;
        voituresOut = NOMBRE_VOITURES - 5; // Q1 -> 15 voitures passent
    } else if (numQuali == 2) {
        dureeQualif = QUALIF2_TEMPS;
        voituresOut = NOMBRE_VOITURES - 10; // Q2 -> 10 voitures passent
    } else if (numQuali == 3) {
        dureeQualif = QUALIF3_TEMPS;
        voituresOut = NOMBRE_VOITURES; // Q3 -> Pas de voitures OUT
    }

    while (1) {
        actuel = time(NULL);
        int temps_ecoule = (int)difftime(actuel, debut);

        // Fin de la qualification si le temps est écoulé
        if (temps_ecoule >= dureeQualif / ACCELERATION) {
            break;
        }

        tour ++;
        system("clear");
        init_stand();
        debutTableau(numQuali, temps_ecoule, "Qualification");
        donne_tempsSecteur(temps_ecoule, dureeQualif);

        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);

        int positionCourse = 0;

        if(numQuali == 1){

            for (int i = 0; i < NOMBRE_VOITURES - 5; i++) {
                positionCourse += 1;

                double tempsDiff = 0.0;
                if(i > 0){
                    tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;
                }

                ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].standTemps, voitures[i].estOUT);
            }

            printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
            for (int i = NOMBRE_VOITURES - 5; i < NOMBRE_VOITURES; i++) {
                positionCourse += 1;

                double tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;

                if(tour == 10){
                    voitures[i].estOUT = true;
                }
                ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].standTemps, voitures[i].estOUT);
            }
        }else{
            if(numQuali == 2 || numQuali == 3){

                for (int i = 0; i < NOMBRE_VOITURES - 10; i++) {
                    positionCourse += 1; 

                    double tempsDiff = 0.0;
                    if(i > 0){
                        tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;
                    }

                    ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].standTemps, voitures[i].estOUT);
                }

                printf("├──────┼───────────┼─────────┼─────────┼─────────┼────────────────┼──────────────────┼──────────┼─────────┼───────┤\n");
                for (int i = NOMBRE_VOITURES - 10; i < NOMBRE_VOITURES; i++) {
                    positionCourse += 1;

                    double tempsDiff = voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;

                    if(tour == 10){
                        voitures[i].estOUT = true;
                    }
                    ligneTableauQualif(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps, tempsDiff, voitures[i].standTemps, voitures[i].estOUT);
                }
            }
        }

        finTableau("Qualification");

        meilleurAffichage();
        sleep(2);
    }

    for (int i = voituresOut; i < NOMBRE_VOITURES; i++) {
        voitures[i].estOUT = true;
    }

    enregistrerResultats("Qualification", numQuali);

}


void course(){
    
    for(int tour = 1; tour < NBRE_TOURS_COURSE; tour++){
        system("clear");
        debutTableau(0, tour, "Course");
        donne_tempsSecteur(tour, NBRE_TOURS_COURSE);

        qsort(voitures, NOMBRE_VOITURES, sizeof(Voiture), sortVoitures);

        int positionCourse = 0;

        for (int i = 0; i < NOMBRE_VOITURES; i++) {
            positionCourse += 1;
            ligneTableau(positionCourse, voitures[i].numero, voitures[i].temps_S1, voitures[i].temps_S2, voitures[i].temps_S3, voitures[i].meilleur_temps);
        }
        finTableau("Qualification");

        meilleurAffichage();
        sleep(2);
    }
    enregistrerResultats("Course", 0);
}


void vendredi(){
    essais(1);
    init_voitures("ALL");
    init_tempsTotal();

    essais(2);
    init_voitures("ALL");
    init_tempsTotal();
}

void samedi(){
    essais(3);
    init_voitures("ALL");
    init_tempsTotal();

    qualificaiton(1);
    init_voitures("Q2");
    init_tempsTotal();

    qualificaiton(2);
    init_voitures("Q3");
    init_tempsTotal();

    qualificaiton(3);
}

void dimanche(){
    
}

void weekEndClassic(){
    vendredi();
    samedi();
    dimanche();
}


void weekEndSpecial(){
    //essais libre 1h Vendredi matin
    essais(1);
    init_voitures("ALL");
    init_tempsTotal();

    //vendredi après midi qualif sprint


    //samedi matin sprint

    //qualif samedi après midi
    samedi();

    //la course
    dimanche();
}


int main() {
    srand(time(NULL));
    init_voitures("ALL");
    init_tempsTotal();

    //weekEndClassic();
    course();

    return 0;
}