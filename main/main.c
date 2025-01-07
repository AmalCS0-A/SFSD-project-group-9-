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
    int supprime; // 0 = actif, 1 = supprim�
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
    printf("Num�ro d'inscription : ");
    scanf("%d", &etudiant.numero_inscription);
    printf("Nom : ");
    scanf("%s", etudiant.nom);
    printf("Pr�nom : ");
    scanf("%s", etudiant.prenom);
    printf("Ann�e de naissance : ");
    scanf("%d", &etudiant.annee_naissance);
    printf("Classe : ");
    scanf("%s", etudiant.classe);
    printf("Notes pour les modules (SFSD, POO, Analyse Math�matique, Alg�bre Lin�aire) : ");
    for (int i = 0; i < 4; i++) {
        scanf("%f", &etudiant.notes[i]);
    }

    calculer_moyenne(&etudiant);
    etudiant.supprime = 0;

    fprintf(file, "%d,%s,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
            etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
            etudiant.annee_naissance, etudiant.classe,
            etudiant.notes[0], etudiant.notes[1],
            etudiant.notes[2], etudiant.notes[3],
            etudiant.moyenne, etudiant.supprime);
    fclose(file);
    printf("�tudiant ajout� avec succ�s.\n");
}

// Display a student's details
void afficher_etudiant(Etudiant etudiant) {
    if (etudiant.supprime == 1) {
        return;
    }

    printf("\nNum�ro d'inscription : %d\n", etudiant.numero_inscription);
    printf("Nom : %s\n", etudiant.nom);
    printf("Pr�nom : %s\n", etudiant.prenom);
    printf("Ann�e de naissance : %d\n", etudiant.annee_naissance);
    printf("Classe : %s\n", etudiant.classe);
    printf("Notes : SFSD=%.2f, POO=%.2f, Analyse Math=%.2f, Alg�bre Lin�aire=%.2f\n",
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
    printf("Entrez le num�ro d'inscription de l'�tudiant : ");
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
        printf("�tudiant non trouv�.\n");
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
        perror("Erreur lors de la cr�ation du fichier temporaire");
        fclose(file);
        return;
    }

    int numero_inscription;
    printf("Entrez le num�ro d'inscription de l'�tudiant � modifier : ");
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
            trouve = 1;
            printf("�tudiant trouv�. Entrez les nouvelles informations.\n");

            printf("Nom (%s) : ", etudiant.nom);
            scanf("%s", etudiant.nom);
            printf("Pr�nom (%s) : ", etudiant.prenom);
            scanf("%s", etudiant.prenom);
            printf("Ann�e de naissance (%d) : ", etudiant.annee_naissance);
            scanf("%d", &etudiant.annee_naissance);
            printf("Classe (%s) : ", etudiant.classe);
            scanf("%s", etudiant.classe);
            printf("Notes pour les modules (SFSD=%.2f, POO=%.2f, Analyse Math=%.2f, Alg�bre Lin�aire=%.2f) : ",
                   etudiant.notes[0], etudiant.notes[1], etudiant.notes[2], etudiant.notes[3]);
            for (int i = 0; i < 4; i++) {
                scanf("%f", &etudiant.notes[i]);
            }

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
        printf("�tudiant modifi� avec succ�s.\n");
    } else {
        remove("temp.txt"); // Clean up temp file if no modification was made
        printf("�tudiant non trouv�.\n");
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
    printf("Entrez le num�ro d'inscription de l'�tudiant � supprimer : ");
    if (scanf("%d", &numero_inscription) != 1) {
        printf("Entr�e invalide.\n");
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
        printf("�tudiant supprim� avec succ�s.\n");
    } else {
        // Delete the temporary file if no changes were made
        remove("temp.txt");
        printf("�tudiant non trouv� ou d�j� supprim�.\n");
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
        perror("Erreur lors de la cr�ation du fichier temporaire");
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

    printf("R�organisation effectu�e avec succ�s. Les �tudiants supprim�s ont �t� retir�s.\n");
}
void extraire_etudiants_par_classe() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char classe_recherchee[10];
    printf("Entrez la classe � extraire : ");
    scanf("%s", classe_recherchee);

    Etudiant etudiant;
    int trouve = 0;

    printf("\n�tudiants de la classe %s :\n", classe_recherchee);
    printf("--------------------------------------------------\n");

    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%9[^,],%f,%f,%f,%f,%f,%d\n",
                  &etudiant.numero_inscription, etudiant.nom, etudiant.prenom,
                  &etudiant.annee_naissance, etudiant.classe,
                  &etudiant.notes[0], &etudiant.notes[1],
                  &etudiant.notes[2], &etudiant.notes[3],
                  &etudiant.moyenne, &etudiant.supprime) == 11) {
        if (strcmp(etudiant.classe, classe_recherchee) == 0 && etudiant.supprime == 0) {
            afficher_etudiant(etudiant);
            trouve = 1;
        }
    }

    if (!trouve) {
        printf("Aucun �tudiant trouv� dans la classe %s.\n", classe_recherchee);
    }

    fclose(file);
}


// Main function
int main() {
    int choix;
    do {
        printf("\nMenu Principal\n");
        printf("1. Ajouter un �tudiant\n");
        printf("2. Rechercher un �tudiant\n");
        printf("3. Modifier un �tudiant\n");
        printf("4. Supprimer un �tudiant (logiquement)\n");
        printf("5. Extraire �tudiants par classe\n");
        printf("6. R�organiser le fichier\n");
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
