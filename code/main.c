#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "etudiants.txt"
#define MAX_NAME 15

typedef struct {
    int numero_inscription;
    char nom[MAX_NAME];
    char prenom[MAX_NAME];
    int annee_naissance;
    char classe[10];
    float notes[4];
    float moyenne;
    int supprime; // 0 = actif, 1 = supprime
} Etudiant;

// Function prototypes
void calculer_moyenne(Etudiant *etudiant);
void ajouter_etudiant();
void rechercher_etudiant();
void modifier_etudiant();
void supprimer_etudiant_logique();
void extraire_etudiants_par_classe();
void reorganiser_etudiants();
void afficher_etudiant(Etudiant etudiant);

// Calculate weighted average
void calculer_moyenne(Etudiant *etudiant) {
    int coefficients[4] = {4, 3, 2, 5};
    float somme_notes = 0;
    int somme_coefficients = 0;

    for (int i = 0; i < 4; i++) {
        somme_notes += etudiant->notes[i] * coefficients[i];
        somme_coefficients += coefficients[i];
    }
    etudiant->moyenne = somme_notes / somme_coefficients;
}

// Add a new student
void ajouter_etudiant() {
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Etudiant etudiant;
    printf("Numero d'inscription : ");
    scanf("%d", &etudiant.numero_inscription);
    printf("Nom : ");
    scanf("%s", etudiant.nom);
    printf("Prenom : ");
    scanf("%s", etudiant.prenom);
    do {
        printf("Annee de naissance : ");
    scanf("%d", &etudiant.annee_naissance);
    }while(etudiant.annee_naissance>2020 || etudiant.annee_naissance<1990);

    printf("Classe : ");
    scanf("%s", etudiant.classe);
    printf("Notes pour les modules (SFSD, POO, Analyse Mathematique, Algebre Lineaire)\n");
    do{
        printf("SFSD : ");
        scanf("%f",&etudiant.notes[0]);
    }while(etudiant.notes[0]>20 || etudiant.notes[0]<0);

    do{
        printf("POO : ");
    scanf("%f",&etudiant.notes[1]);
    }while(etudiant.notes[1]>20 || etudiant.notes[1]<0);

    do{
        printf("Analyse Mathematique : ");
    scanf("%f",&etudiant.notes[2]);
    }while(etudiant.notes[2]>20 || etudiant.notes[2]<0);

    do{
        printf("Algebre Lineaire : ");
    scanf("%f",&etudiant.notes[3]);
    }while(etudiant.notes[3]>20 || etudiant.notes[3]<0);

    calculer_moyenne(&etudiant);
    etudiant.supprime = 0;

    fprintf(file, "%d,%s,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
            etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
            etudiant.annee_naissance, etudiant.classe,
            etudiant.notes[0], etudiant.notes[1],
            etudiant.notes[2], etudiant.notes[3],
            etudiant.moyenne, etudiant.supprime);
    fclose(file);
    printf("etudiant ajoutu avec succes.\n");
}

// Display a student's details
void afficher_etudiant(Etudiant etudiant) {
    if (etudiant.supprime == 1) {
        return;
    }

    printf("\nNumero d'inscription : %d\n", etudiant.numero_inscription);
    printf("Nom : %s\n", etudiant.nom);
    printf("Prenom : %s\n", etudiant.prenom);
    printf("Annee de naissance : %d\n", etudiant.annee_naissance);
    printf("Classe : %s\n", etudiant.classe);
    printf("Notes : SFSD=%.2f, POO=%.2f, Analyse Math=%.2f, Algebre Lineaire=%.2f\n",
           etudiant.notes[0], etudiant.notes[1], etudiant.notes[2], etudiant.notes[3]);
    printf("Moyenne : %.2f\n", etudiant.moyenne);
}

// Search for a student
void rechercher_etudiant() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int numero_inscription;
    printf("Entrez le numero d'inscription de l'etudiant : ");
    scanf("%d", &numero_inscription);

    Etudiant etudiant;
    int trouve = 0;

    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%9[^,],%f,%f,%f,%f,%f,%d\n",
                  &etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                  &etudiant.annee_naissance, etudiant.classe,
                  &etudiant.notes[0], &etudiant.notes[1],
                  &etudiant.notes[2], &etudiant.notes[3],
                  &etudiant.moyenne, &etudiant.supprime) == 11) {
        if (etudiant.numero_inscription == numero_inscription && etudiant.supprime == 0) {
            afficher_etudiant(etudiant);
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("etudiant non trouve.\n");
    }

    fclose(file);
}

// Modify a student
void modifier_etudiant() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        perror("Erreur lors de la creation du fichier temporaire");
        fclose(file);
        return;
    }

    int numero_inscription;
    printf("Entrez le numuro d'inscription de l'etudiant a modifier : ");
    scanf("%d", &numero_inscription);

    Etudiant etudiant;
    int trouve = 0;
    float a;
    int b;
    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%9[^,],%f,%f,%f,%f,%f,%d\n",
                  &etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                  &etudiant.annee_naissance, etudiant.classe,
                  &etudiant.notes[0], &etudiant.notes[1],
                  &etudiant.notes[2], &etudiant.notes[3],
                  &etudiant.moyenne, &etudiant.supprime) == 11) {
        if (etudiant.numero_inscription == numero_inscription && etudiant.supprime == 0) {
            trouve = 1;
            printf("etudiant trouvu. Entrez les nouvelles informations.\n");

            printf("Nom (%s) : ", etudiant.nom);
            scanf("%s", etudiant.nom);
            printf("Prenom (%s) : ", etudiant.prenom);
            scanf("%s", etudiant.prenom);
            b=etudiant.annee_naissance;
            do {
                 printf("Annee de naissance (%d) : ", b);
                 scanf("%d", &etudiant.annee_naissance);
                }while(etudiant.annee_naissance>2020 || etudiant.annee_naissance<1990);
            printf("Classe (%s) : ", etudiant.classe);
            scanf("%s", etudiant.classe);

            printf("Notes pour les modules\n");
            a=etudiant.notes[0];
            do{
        printf("SFSD=(%.2f): ",a);
        scanf("%f",&etudiant.notes[0]);
    }while(etudiant.notes[0]>20 || etudiant.notes[0]<0);
    a=etudiant.notes[1];
    do{
        printf("POO=(%.2f): ",a);
    scanf("%f",&etudiant.notes[1]);
    }while(etudiant.notes[1]>20 || etudiant.notes[1]<0);
     a=etudiant.notes[2];
    do{
        printf("Analyse Mathematique=(%.2f): ",a);
    scanf("%f",&etudiant.notes[2]);
    }while(etudiant.notes[2]>20 || etudiant.notes[2]<0);
    a=etudiant.notes[3];
    do{
        printf("Algebre Lineaire=(%.2f): ",a);
    scanf("%f",&etudiant.notes[3]);
    }while(etudiant.notes[3]>20 || etudiant.notes[3]<0);

            calculer_moyenne(&etudiant);
        }

        // Write either the modified or original record to the temp file
        fprintf(tempFile, "%d,%s,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
                etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                etudiant.annee_naissance, etudiant.classe,
                etudiant.notes[0], etudiant.notes[1],
                etudiant.notes[2], etudiant.notes[3],
                etudiant.moyenne, etudiant.supprime);
    }

    fclose(file);
    fclose(tempFile);

    if (trouve) {
        remove(FILENAME);           // Delete the original file
        rename("temp.txt", FILENAME); // Rename temp file to original name
        printf("etudiant modifie avec succes.\n");
    } else {
        remove("temp.txt"); // Clean up temp file if no modification was made
        printf("etudiant non trouvu.\n");
    }
}


// Logically delete a student
void supprimer_etudiant_logique() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        perror("Erreur lors de la cr�ation du fichier temporaire");
        fclose(file);
        return;
    }

    int numero_inscription;
    printf("Entrez le numuro d'inscription de l'etudiant a supprimer : ");
    if (scanf("%d", &numero_inscription) != 1) {
        printf("Entree invalide.\n");
        fclose(file);
        fclose(tempFile);
        return;
    }

    Etudiant etudiant;
    int trouve = 0;

    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%9[^,],%f,%f,%f,%f,%f,%d\n",
                  &etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                  &etudiant.annee_naissance, etudiant.classe, &etudiant.notes[0],
                  &etudiant.notes[1], &etudiant.notes[2], &etudiant.notes[3],
                  &etudiant.moyenne, &etudiant.supprime) == 11) {
        if (etudiant.numero_inscription == numero_inscription && etudiant.supprime == 0) {
            etudiant.supprime = 1; // Mark as logically deleted
            trouve = 1;
        }
        // Write the record to the temporary file (updated or unchanged)
        fprintf(tempFile, "%d,%s,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
                etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                etudiant.annee_naissance, etudiant.classe, etudiant.notes[0],
                etudiant.notes[1], etudiant.notes[2], etudiant.notes[3],
                etudiant.moyenne, etudiant.supprime);
    }

    fclose(file);
    fclose(tempFile);

    if (trouve) {
        // Replace the original file with the updated temporary file
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("etudiant supprime avec succes.\n");
    } else {
        // Delete the temporary file if no changes were made
        remove("temp.txt");
        printf("etudiant non trouvu ou deja supprime.\n");
    }
}

// Reorganize the file
void reorganiser_etudiants() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        perror("Erreur lors de la creation du fichier temporaire");
        fclose(file);
        return;
    }

    Etudiant etudiant;

    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%9[^,],%f,%f,%f,%f,%f,%d\n",
                  &etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                  &etudiant.annee_naissance, etudiant.classe, &etudiant.notes[0],
                  &etudiant.notes[1], &etudiant.notes[2], &etudiant.notes[3],
                  &etudiant.moyenne, &etudiant.supprime) == 11) {
        if (etudiant.supprime == 0) { // Only write non-deleted students
            fprintf(tempFile, "%d,%s,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
                    etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                    etudiant.annee_naissance, etudiant.classe, etudiant.notes[0],
                    etudiant.notes[1], etudiant.notes[2], etudiant.notes[3],
                    etudiant.moyenne, etudiant.supprime);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    printf("Reorganisation effectue avec succes. Les etudiants supprimes ont et retires.\n");
}
// Extract students by class
void extraire_etudiants_par_classe() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char classe_recherchee[10];
    printf("Entrez la classe de extraire : ");
    scanf("%s", classe_recherchee);

    Etudiant etudiants[100]; // Assuming a maximum of 100 students for simplicity
    int count = 0;

    Etudiant etudiant;
    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%9[^,],%f,%f,%f,%f,%f,%d\n",
                  &etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                  &etudiant.annee_naissance, etudiant.classe,
                  &etudiant.notes[0], &etudiant.notes[1],
                  &etudiant.notes[2], &etudiant.notes[3],
                  &etudiant.moyenne, &etudiant.supprime) == 11) {
        if (strcmp(etudiant.classe, classe_recherchee) == 0 && etudiant.supprime == 0) {
            etudiants[count++] = etudiant;
        }
    }
    fclose(file);

    // Sort students by moyenne in descending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (etudiants[i].moyenne < etudiants[j].moyenne) {
                Etudiant temp = etudiants[i];
                etudiants[i] = etudiants[j];
                etudiants[j] = temp;
            }
        }
    }

    // Display the sorted list
    if (count > 0) {
        printf("\netudiants de la classe %s (tries par moyenne decroissante) :\n", classe_recherchee);
        printf("--------------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            afficher_etudiant(etudiants[i]);
        }
    } else {
        printf("Aucun etudiant trouvu dans la classe %s.\n", classe_recherchee);
    }
}

// Main function
int main() {
    int choix;
    do {
        printf("\nMenu Principal\n");
        printf("1. Ajouter un etudiant\n");
        printf("2. Rechercher un etudiant\n");
        printf("3. Modifier un etudiant\n");
        printf("4. Supprimer un etudiant (logiquement)\n");
        printf("5. Extraire etudiants par classe\n");
        printf("6. Reorganiser le fichier\n");
        printf("7. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouter_etudiant(); break;
            case 2: rechercher_etudiant(); break;
            case 3: modifier_etudiant(); break;
            case 4: supprimer_etudiant_logique(); break;
            case 5: extraire_etudiants_par_classe(); break;
            case 6: reorganiser_etudiants(); break;
            case 7: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 7);

    return 0;
}
