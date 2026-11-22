#include <stdio.h>
#include "image.h"

/* Test : ./convert entree sortie  ->  recopie l'image telle quelle */
int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage : %s entree sortie\n", argv[0]);
        return 1;
    }

    Image *img = lire_image(argv[1]);
    if (img == NULL)
        return 1;

    printf("Image lue : %d x %d, %d canal(aux), max %d\n",
           img->largeur, img->hauteur, img->canaux, img->valeur_max);

    if (!ecrire_image(argv[2], img)) {
        liberer_image(img);
        return 1;
    }

    liberer_image(img);
    printf("Image écrite dans %s\n", argv[2]);
    return 0;
}