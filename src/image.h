#ifndef IMAGE_H
#define IMAGE_H

/* Une image PGM (gris) ou PPM (couleur) en mémoire */
typedef struct {
    int largeur;            /* nombre de colonnes */
    int hauteur;            /* nombre de lignes */
    int canaux;             /* 1 = PGM (gris), 3 = PPM (couleur R,V,B) */
    int valeur_max;         /* valeur maximale d'un pixel (souvent 255) */
    unsigned char *pixels;  /* largeur * hauteur * canaux octets */
} Image;

Image *creer_image(int largeur, int hauteur, int canaux, int valeur_max);
void liberer_image(Image *img);
Image *lire_image(const char *nom_fichier);
int ecrire_image(const char *nom_fichier, const Image *img);

#endif