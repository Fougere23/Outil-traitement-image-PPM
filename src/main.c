#include <stdio.h>
#include <string.h>
#include "image.h"
#include "filtres.h"

/* Utilisation : ./convert entree sortie filtre
   Exemple     : ./convert photo.pgm resultat.pgm negatif */
int main(int argc, char *argv[])
{
    /* 1) vérifier le nombre d'arguments */
    if (argc != 4) {
        fprintf(stderr, "Usage : %s entree sortie filtre\n", argv[0]);
        fprintf(stderr, "Filtres disponibles : negatif\n");
        return 1;
    }
    const char *entree = argv[1];
    const char *sortie = argv[2];
    const char *filtre = argv[3];

    /* 2) lire l'image */
    Image *img = lire_image(entree);
    if (img == NULL)
        return 1;

    /* 3) appliquer le bon filtre */
    if (strcmp(filtre, "negatif") == 0) {
        filtre_negatif(img);
    } else {
        fprintf(stderr, "Erreur : filtre inconnu \"%s\"\n", filtre);
        liberer_image(img);
        return 1;
    }

    /* 4) écrire le résultat */
    if (!ecrire_image(sortie, img)) {
        liberer_image(img);
        return 1;
    }

    liberer_image(img);
    printf("Filtre \"%s\" appliqué : %s -> %s\n", filtre, entree, sortie);
    return 0;
}