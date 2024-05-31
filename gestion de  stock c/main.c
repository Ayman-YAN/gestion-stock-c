#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN 100
#define FILENAME "stock_data.csv"
#define HISTORIQUE_FILE "historique.csv"
#define SEPARATOR ";"

typedef struct {
    int id;
    char nom[MAX_LEN];
    char description[MAX_LEN];
    char categorie[MAX_LEN];
    float prix_unitaire;
    int quantite_stock;
    int seuil_alerte;
    char date_derniere_entree[MAX_LEN];
    char date_derniere_sortie[MAX_LEN];
    int id_fournisseur;
} Produit;

void afficherMenuClient() {
    printf("\n------------*************Menu Client**************------------\n");
    printf("\n");
    printf("         1. Afficher le stock\n");
    printf("         2. Afficher les categories disponibles et leurs produits\n");
    printf("         3. Acheter un produit\n");
    printf("         4. Rechercher un produit par nom\n");
    printf("         5. Afficher les produits tries par prix croissant\n");
    printf("         6. Afficher les produits tries par prix decroissant\n");
    printf("         7. Quitter\n");
    printf("\n");
    printf("\n-----------*************************************----------------\n");
}

void afficherMenuGerant() {
    printf("\n-----------*********Menu Gerant******************\n");
    printf("1. Ajouter un produit\n");
    printf("2. Supprimer un produit\n");
    printf("3. Afficher le stock\n");
    printf("4. Modifier un produit\n");
    printf("5. Historique des achats\n");
    printf("6. Quitter\n");
}

bool verifierMotDePasse() {
    char mot_de_passe[MAX_LEN];
    printf("Veuillez entrer le mot de passe : ");
    fgets(mot_de_passe, MAX_LEN, stdin);
    mot_de_passe[strcspn(mot_de_passe, "\n")] = 0;
    if (strcmp(mot_de_passe, "1234") == 0) {
        printf("Mot de passe correct.\n");
        return true;
    } else {
        printf("Mot de passe incorrect. Acces refuse.\n");
        return false;
    }
}

void afficherStock() {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("\nStock de produits:\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------------------\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, &p.id_fournisseur);
        printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
               p.quantite_stock, p.seuil_alerte, p.id_fournisseur);
    }
    printf("------------------------------------------------------------------------------------------------\n");

    fclose(fp);
}

void afficherCategoriesDisponibles() {
    FILE *fp;
    char buffer[MAX_LEN];
    char categorie[MAX_LEN];
    char categoriesDejaAffichees[MAX_LEN][MAX_LEN];
    int categoriesCount = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("\nCategories disponibles:\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%*d;%*[^;];%*[^;];%[^;];%*f;%*d;%*d;%*d", categorie);
        bool categorieDejaAffichee = false;
        for (int i = 0; i < categoriesCount; i++) {
            if (strcmp(categoriesDejaAffichees[i], categorie) == 0) {
                categorieDejaAffichee = true;
                break;
            }
        }
        if (!categorieDejaAffichee) {
            printf("%s\n", categorie);
            strcpy(categoriesDejaAffichees[categoriesCount], categorie);
            categoriesCount++;
        }
    }

    fclose(fp);
}

void afficherProduitsParCategorie(char categorie[]) {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("\nProduits de la categorie \"%s\":\n", categorie);
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------------------\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, &p.id_fournisseur);
        if (strcmp(p.categorie, categorie) == 0) {
            printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
                   p.quantite_stock, p.seuil_alerte, p.id_fournisseur);
        }
    }
    printf("------------------------------------------------------------------------------------------------\n");

    fclose(fp);
}

void acheterProduit(char nom[], int quantite) {
    FILE *fp;
    FILE *tempFile;
    char buffer[MAX_LEN];
    Produit produit;

    fp = fopen(FILENAME, "r");
    tempFile = fopen("temp.csv", "w");

    if (fp == NULL || tempFile == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%d",
               &produit.id, produit.nom, produit.description, produit.categorie,
               &produit.prix_unitaire, &produit.quantite_stock, &produit.seuil_alerte, &produit.id_fournisseur);

        if (strcmp(produit.nom, nom) == 0) {
            produit.quantite_stock -= quantite;
            if (produit.quantite_stock < 0) {
                printf("Quantite insuffisante en stock.\n");
                fclose(fp);
                fclose(tempFile);
                remove("temp.csv");
                return;
            }
            printf("Achat de %d unite(s) de %s effectue avec succes.\n", quantite, nom);
            enregistrerAchatDansHistorique(produit, quantite);
        }

        fprintf(tempFile, "%d;%s;%s;%s;%.2f;%d;%d;%d\n",
                produit.id, produit.nom, produit.description, produit.categorie,
                produit.prix_unitaire, produit.quantite_stock, produit.seuil_alerte, produit.id_fournisseur);
    }

    fclose(fp);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.csv", FILENAME);
}

void enregistrerAchatDansHistorique(Produit produit, int quantite) {
    FILE *fp;
    fp = fopen(HISTORIQUE_FILE, "a");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier historique.\n");
        return;
    }

    fprintf(fp, "%d;%s;%d\n", produit.id, produit.nom, quantite);
    fclose(fp);
}

void ajouterProduit(Produit p) {
    FILE *fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fp, "%d;%s;%s;%s;%.2f;%d;%d;%d\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire, p.quantite_stock, p.seuil_alerte, p.id_fournisseur);
    fclose(fp);
    printf("Produit ajoute avec succes.\n");
}

void supprimerProduit(int id) {
    FILE *fp;
    FILE *tempFile;
    char buffer[MAX_LEN];
    Produit produit;

    fp = fopen(FILENAME, "r");
    tempFile = fopen("temp.csv", "w");

    if (fp == NULL || tempFile == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    bool produitSupprime = false;

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%d", &produit.id, produit.nom, produit.description, produit.categorie, &produit.prix_unitaire,
               &produit.quantite_stock, &produit.seuil_alerte, &produit.id_fournisseur);
        if (produit.id != id) {
            fprintf(tempFile, "%d;%s;%s;%s;%.2f;%d;%d;%d\n", produit.id, produit.nom, produit.description, produit.categorie, produit.prix_unitaire,
                    produit.quantite_stock, produit.seuil_alerte, produit.id_fournisseur);
        } else {
            produitSupprime = true;
        }
    }

    fclose(fp);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (produitSupprime) {
        printf("Produit supprime avec succes.\n");
    } else {
        printf("Produit non trouve.\n");
    }
}

void modifierProduit(Produit p) {
    FILE *fp;
    FILE *tempFile;
    char buffer[MAX_LEN];
    Produit produit;

    fp = fopen(FILENAME, "r");
    tempFile = fopen("temp.csv", "w");

    if (fp == NULL || tempFile == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    bool produitModifie = false;

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%d", &produit.id, produit.nom, produit.description, produit.categorie, &produit.prix_unitaire,
               &produit.quantite_stock, &produit.seuil_alerte, &produit.id_fournisseur);
        if (produit.id == p.id) {
            fprintf(tempFile, "%d;%s;%s;%s;%.2f;%d;%d;%d\n", p.id, p.nom, p.description, p.categorie, p.prix_unitaire,
                    p.quantite_stock, p.seuil_alerte, p.id_fournisseur);
            produitModifie = true;
        } else {
            fprintf(tempFile, "%d;%s;%s;%s;%.2f;%d;%d;%d\n", produit.id, produit.nom, produit.description, produit.categorie, produit.prix_unitaire,
                    produit.quantite_stock, produit.seuil_alerte, produit.id_fournisseur);
        }
    }

    fclose(fp);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (produitModifie) {
        printf("Produit modifie avec succes.\n");
    } else {
        printf("Produit non trouve.\n");
    }
}

void afficherHistorique() {
    FILE *fp;
    char buffer[MAX_LEN];

    fp = fopen(HISTORIQUE_FILE, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier historique.\n");
        return;
    }

    printf("\nHistorique des achats:\n");
    printf("----------------------------\n");
    printf("| ID  | Nom          | Quantite |\n");
    printf("----------------------------\n");
    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        int id, quantite;
        char nom[MAX_LEN];
        sscanf(buffer, "%d;%[^;];%d", &id, nom, &quantite);
        printf("| %-3d | %-12s | %-8d |\n", id, nom, quantite);
    }
    printf("----------------------------\n");

    fclose(fp);
}


void rechercherProduitParNom(char nom[]) {
    FILE *fp;
    char buffer[MAX_LEN];
    Produit p;
    bool trouve = false;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%[^;];%[^;];%d", &p.id, p.nom, p.description, p.categorie, &p.prix_unitaire,
               &p.quantite_stock, &p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie, &p.id_fournisseur);
        if (strcmp(p.nom, nom) == 0) {
            printf("\nProduit trouve:\n");
            printf("ID: %d\nNom: %s\nDescription: %s\nCategorie: %s\nPrix Unitaire: %.2f\nQuantite en Stock: %d\nSeuil d'Alerte: %d\nDate Derniere Entree: %s\nDate Derniere Sortie: %s\nID Fournisseur: %d\n",
                   p.id, p.nom, p.description, p.categorie, p.prix_unitaire, p.quantite_stock, p.seuil_alerte, p.date_derniere_entree, p.date_derniere_sortie, p.id_fournisseur);
            trouve = true;
            break;
        }
    }

    if (!trouve) {
        printf("Produit non trouve.\n");
    }

    fclose(fp);
}

void afficherProduitsTriesParPrix(bool croissant) {
    FILE *fp;
    Produit produits[MAX_LEN];
    Produit temp;
    char buffer[MAX_LEN];
    int count = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fgets(buffer, MAX_LEN, fp) != NULL) {
        sscanf(buffer, "%d;%[^;];%[^;];%[^;];%f;%d;%d;%[^;];%[^;];%d", &produits[count].id, produits[count].nom, produits[count].description, produits[count].categorie, &produits[count].prix_unitaire,
               &produits[count].quantite_stock, &produits[count].seuil_alerte, produits[count].date_derniere_entree, produits[count].date_derniere_sortie, &produits[count].id_fournisseur);
        count++;
    }
    fclose(fp);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if ((croissant && produits[j].prix_unitaire > produits[j + 1].prix_unitaire) || (!croissant && produits[j].prix_unitaire < produits[j + 1].prix_unitaire)) {
                temp = produits[j];
                produits[j] = produits[j + 1];
                produits[j + 1] = temp;
            }
        }
    }

    printf("\nProduits tries par prix %s:\n", croissant ? "croissant" : "decroissant");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| ID  | Nom          | Description        | Categorie | Prix   | Quantite | Seuil | Fournisseur |\n");
    printf("------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-12s | %-18s | %-9s | %-6.2f | %-8d | %-5d | %-10d |\n", produits[i].id, produits[i].nom, produits[i].description, produits[i].categorie, produits[i].prix_unitaire,
               produits[i].quantite_stock, produits[i].seuil_alerte, produits[i].id_fournisseur);
    }
    printf("------------------------------------------------------------------------------------------------\n");
}

int main() {
    int choix;
    while (1) {
        printf("\nBienvenue dans Electro-Computer . Veuillez choisir votre role:\n");
        printf("1. Client\n");
        printf("2. Gerant\n");
        printf("3. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);
        getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf

        if (choix == 1) {
            int choixClient;
            do {
                afficherMenuClient();
                printf("Choix: ");
                scanf("%d", &choixClient);
                getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf

                if (choixClient == 1) {
                    afficherStock();
                } else if (choixClient == 2) {
                    afficherCategoriesDisponibles();
                    printf("\nVeuillez entrer la categorie dont vous voulez voir les produits: ");
                    char categorie[MAX_LEN];
                    fgets(categorie, MAX_LEN, stdin);
                    categorie[strcspn(categorie, "\n")] = 0;
                    afficherProduitsParCategorie(categorie);
                } else if (choixClient == 3) {
                    printf("Veuillez entrer le nom du produit que vous voulez acheter: ");
                    char nom[MAX_LEN];
                    fgets(nom, MAX_LEN, stdin);
                    nom[strcspn(nom, "\n")] = 0;
                    printf("Veuillez entrer la quantite que vous voulez acheter: ");
                    int quantite;
                    scanf("%d", &quantite);
                    getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf
                    acheterProduit(nom, quantite);
                } else if (choixClient == 4) {
                    printf("Veuillez entrer le nom du produit que vous voulez rechercher: ");
                    char nom[MAX_LEN];
                    fgets(nom, MAX_LEN, stdin);
                    nom[strcspn(nom, "\n")] = 0;
                    rechercherProduitParNom(nom);
                } else if (choixClient == 5) {
                    afficherProduitsTriesParPrix(true);
                } else if (choixClient == 6) {
                    afficherProduitsTriesParPrix(false);
                } else if (choixClient == 7) {
                    printf("Retour au menu principal...\n");
                    break;
                } else {
                    printf("Choix invalide. Veuillez reessayer.\n");
                }
            } while (choixClient != 7);
        } else if (choix == 2) {
            if (!verifierMotDePasse()) {
                continue;
            }
            int choixGerant;
            do {
                afficherMenuGerant();
                printf("Choix: ");
                scanf("%d", &choixGerant);
                getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf

                if (choixGerant == 1) {
                    Produit p;
                    printf("Veuillez entrer les informations du produit a ajouter:\n");
                    printf("ID: ");
                    scanf("%d", &p.id);
                    getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf
                    printf("Nom: ");
                    fgets(p.nom, MAX_LEN, stdin);
                    p.nom[strcspn(p.nom, "\n")] = 0;
                    printf("Description: ");
                    fgets(p.description, MAX_LEN, stdin);
                    p.description[strcspn(p.description, "\n")] = 0;
                    printf("Categorie: ");
                    fgets(p.categorie, MAX_LEN, stdin);
                    p.categorie[strcspn(p.categorie, "\n")] = 0;
                    printf("Prix unitaire: ");
                    scanf("%f", &p.prix_unitaire);
                    printf("Quantite en stock: ");
                    scanf("%d", &p.quantite_stock);
                    printf("Seuil d'alerte: ");
                    scanf("%d", &p.seuil_alerte);
                    printf("ID fournisseur: ");
                    scanf("%d", &p.id_fournisseur);
                    getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf

                    ajouterProduit(p);
                } else if (choixGerant == 2) {
                    printf("Veuillez entrer l'ID du produit a supprimer: ");
                    int id;
                    scanf("%d", &id);
                    getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf
                    supprimerProduit(id);
                } else if (choixGerant == 3) {
                    afficherStock();
                } else if (choixGerant == 4) {
                    Produit p;
                    printf("Veuillez entrer les informations du produit a modifier:\n");
                    printf("ID: ");
                    scanf("%d", &p.id);
                    getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf
                    printf("Nom: ");
                    fgets(p.nom, MAX_LEN, stdin);
                    p.nom[strcspn(p.nom, "\n")] = 0;
                    printf("Description: ");
                    fgets(p.description, MAX_LEN, stdin);
                    p.description[strcspn(p.description, "\n")] = 0;
                    printf("Categorie: ");
                    fgets(p.categorie, MAX_LEN, stdin);
                    p.categorie[strcspn(p.categorie, "\n")] = 0;
                    printf("Prix unitaire: ");
                    scanf("%f", &p.prix_unitaire);
                    printf("Quantite en stock: ");
                    scanf("%d", &p.quantite_stock);
                    printf("Seuil d'alerte: ");
                    scanf("%d", &p.seuil_alerte);
                    printf("ID fournisseur: ");
                    scanf("%d", &p.id_fournisseur);
                    getchar(); // Pour consommer le caractère de nouvelle ligne laissé par scanf

                    modifierProduit(p);
                } else if (choixGerant == 5) {
                    afficherHistorique();
                } else if (choixGerant == 6) {
                    printf("Retour au menu principal...\n");
                    break;
                } else {
                    printf("Choix invalide. Veuillez reessayer.\n");
                }
            } while (choixGerant != 6);
        } else if (choix == 3) {
            printf("Merci d'avoir utilise le gestionnaire de stock. Au revoir!\n");
            break;
        } else {
            printf("Choix invalide. Veuillez reessayer.\n");
        }
    }

    return 0;
}
