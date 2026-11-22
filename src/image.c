#include <stdio.h>
#include <stdlib.h>
#include "image.h"

/* Crée une image vide (pixels à 0). Renvoie NULL en cas d'erreur. */
Image *creer_image(int largeur, int hauteur, int canaux, int valeur_max)
{
    Image *img = malloc(sizeof(Image));
    if (img == NULL)
        return NULL;

    img->largeur = largeur;
    img->hauteur = hauteur;
    img->canaux = canaux;
    img->valeur_max = valeur_max;

    /* calloc : alloue ET met tous les pixels à 0 */
    img->pixels = calloc((size_t)largeur * hauteur * canaux, 1);
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }
    return img;
}

/* Libère la mémoire : d'abord les pixels, puis la structure. */
void liberer_image(Image *img)
{
    if (img == NULL)
        return;
    free(img->pixels);
    free(img);
}

/* Lit un nombre de l'en-tête en sautant les espaces et les commentaires '#'. */
static int lire_entier(FILE *f, int *valeur)
{
    int c = fgetc(f);
    /* sauter les espaces et les commentaires */
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '#') {
        if (c == '#') {
            while (c != '\n' && c != EOF)   /* ignorer jusqu'à la fin de ligne */
                c = fgetc(f);
        }
        c = fgetc(f);
    }
    if (c == EOF)
        return 0;
    ungetc(c, f);   /* on remet le premier chiffre */
    return fscanf(f, "%d", valeur) == 1;
}

/* Lit un fichier PGM (P5) ou PPM (P6). Renvoie NULL en cas d'erreur. */
Image *lire_image(const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "rb");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s\n", nom_fichier);
        return NULL;
    }

    /* 1) le "nombre magique" : P5 ou P6 */
    char magique[3] = {0};
    if (fread(magique, 1, 2, f) != 2 || magique[0] != 'P'
        || (magique[1] != '5' && magique[1] != '6')) {
        fprintf(stderr, "Erreur : %s n'est pas un PGM (P5) ou PPM (P6)\n", nom_fichier);
        fclose(f);
        return NULL;
    }
    int canaux = (magique[1] == '5') ? 1 : 3;

    /* 2) largeur, hauteur, valeur max */
    int largeur, hauteur, valeur_max;
    if (!lire_entier(f, &largeur) || !lire_entier(f, &hauteur)
        || !lire_entier(f, &valeur_max)
        || largeur <= 0 || hauteur <= 0 || valeur_max <= 0 || valeur_max > 255) {
        fprintf(stderr, "Erreur : en-tête invalide dans %s\n", nom_fichier);
        fclose(f);
        return NULL;
    }
    fgetc(f);   /* un seul caractère blanc sépare l'en-tête des pixels */

    /* 3) les pixels */
    Image *img = creer_image(largeur, hauteur, canaux, valeur_max);
    if (img == NULL) {
        fprintf(stderr, "Erreur : mémoire insuffisante\n");
        fclose(f);
        return NULL;
    }
    size_t taille = (size_t)largeur * hauteur * canaux;
    if (fread(img->pixels, 1, taille, f) != taille) {
        fprintf(stderr, "Erreur : pixels manquants dans %s\n", nom_fichier);
        liberer_image(img);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return img;
}

/* Écrit l'image en PGM (1 canal) ou PPM (3 canaux). Renvoie 1 si OK, 0 sinon. */
int ecrire_image(const char *nom_fichier, const Image *img)
{
    FILE *f = fopen(nom_fichier, "wb");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible de créer %s\n", nom_fichier);
        return 0;
    }

    /* en-tête */
    fprintf(f, "%s\n%d %d\n%d\n", (img->canaux == 1) ? "P5" : "P6",
            img->largeur, img->hauteur, img->valeur_max);

    /* pixels */
    size_t taille = (size_t)img->largeur * img->hauteur * img->canaux;
    size_t ecrits = fwrite(img->pixels, 1, taille, f);
    fclose(f);
    return ecrits == taille;
}