#include "filtres.h"

/* Négatif : chaque valeur devient (valeur_max - valeur).
   Le noir devient blanc, le blanc devient noir.
   Marche pour PGM (1 canal) et PPM (3 canaux). */
void filtre_negatif(Image *img)
{
    int total = img->largeur * img->hauteur * img->canaux;
    for (int i = 0; i < total; i++) {
        img->pixels[i] = img->valeur_max - img->pixels[i];
    }
}