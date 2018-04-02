#include "image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printBits(size_t const size, void const *const ptr) {
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  puts("");
}

image init(TYPE_IMAGE type, uint32_t largeur, uint32_t hauteur,
           uint16_t val_max) {
  image tmp;
  tmp.type = type;
  tmp.largeur = largeur;
  tmp.hauteur = hauteur;
  tmp.val_max = val_max;
  tmp.data = malloc(largeur * hauteur * sizeof(uint64_t));
  return tmp;
}

image init_file(char *path) {
  FILE *file = fopen(path, "r");
  image img;

  if (file != NULL) {
    char type[2];
    fscanf(file, "%s %u %u %hu", type, &img.largeur, &img.hauteur,
           &img.val_max);
    img.data = malloc(img.largeur * img.hauteur * sizeof(uint64_t));
    // printf("%s %u %u %hu\n", type, img.largeur, img.hauteur, img.val_max);
    int i = 0;
    uint32_t bleu;
    uint32_t vert;
    uint64_t pixel;
    while (!feof(file)) {
      pixel = 0;
      fscanf(file, "%lu %u %u", &pixel, &vert, &bleu);
      pixel = pixel << 16;
      pixel += vert;
      pixel = pixel << 16;
      pixel += bleu;

      img.data[i] = pixel;
      i++;
    }
  } else {
    printf("Impossible d'ouvrir le fichier\n");
  }

  fclose(file);
  return img;
}

image ppmtopgm(image *img, float coeff_r, float coeff_v, float coeff_b) {
  uint32_t rouge;
  uint32_t vert;
  uint32_t bleu;
  uint64_t masque16 = pow(2, 16) - 1;
  image img_pgm = init(P2, img->largeur, img->hauteur, img->val_max);

  for (int i = 0; i < img->largeur * img->hauteur; i++) {
    bleu = img->data[i] & masque16;
    vert = (img->data[i] >> 16) & masque16;
    rouge = (img->data[i] >> 32) & masque16;

    img_pgm.data[i] = coeff_r * rouge + coeff_v * vert + coeff_b * bleu;
  }
  return img_pgm;
}

image ppmtopbm(image *ppm, float alpha) {
  uint64_t masque = 65535;
  image pbm = init(P1, ppm->largeur, ppm->hauteur, ppm->val_max);

  int i = 0;
  while (i < pbm.largeur * pbm.hauteur) {
    uint64_t bleu = ppm->data[i] & masque;
    ppm->data[i] = ppm->data[i] >> 16;
    uint64_t vert = ppm->data[i] & masque;
    ppm->data[i] = ppm->data[i] >> 16;
    uint64_t rouge = ppm->data[i] & masque;
    // printf("bleu: %lu, vert: %lu, rouge: %lu\n", bleu, vert, rouge);

    float val = (rouge * vert * bleu) / pow((pbm.val_max), 3);
    if (val < alpha) {
      pbm.data[i] = 1;
    } else {
      pbm.data[i] = 0;
    }
    // printf("Val : %f, Alpha: %f\n", val, alpha);
    // printf("Val pixel pbm: %lu\n", pbm.data[i]);
    i++;
  }

  return pbm;
}

void creation_fichier(char *nom_fichier, image img) {
  char path[50];
  char suffixe[4];
  int l = 0;
  int h = 0;
  int i = 0;

  strcpy(path, "../../Exemples/");
  switch (img.type) {
  case 1:
    strcpy(suffixe, ".pbm");
    break;
  case 2:
    strcpy(suffixe, ".pgm");
    break;
  case 3:
    strcpy(suffixe, ".ppm");
    break;
  default:
    printf("Type non reconnue.\n");
    break;
  }

  strcat(path, nom_fichier);
  strcat(path, suffixe);

  FILE *file = fopen(path, "w");

  if (file != NULL) {
    switch (img.type) {
    case 1:
      fprintf(file, "P1\n");
      break;
    case 2:
      fprintf(file, "P2\n");
      break;
    case 3:
      fprintf(file, "P3\n");
      break;
    }
    fprintf(file, "%u %u\n", img.largeur, img.hauteur);
    fprintf(file, "%hu\n", img.val_max);
    while (h < img.hauteur) {
      l = 0;
      while (l < img.largeur) {
        fprintf(file, "%lu ", img.data[i]);
        l++;
        i++;
      }
      fprintf(file, "\n");
      h++;
    }
  } else {
    printf("Impossible d'ouvrir le fichier\n");
  }

  fclose(file);
}

int main(int argc, char const *argv[]) {

  image disney = init_file("../../Exemples/disney_045.ppm");

  image disney_pgm = ppmtopgm(&disney, 0.5, 0.5, 0.5);
  image disney_pbm = ppmtopbm(&disney, 0.0005);
  creation_fichier("disney_045_pgm", disney_pgm);
  creation_fichier("disney_045_pbm", disney_pbm);

  image chalet = init_file("../../Exemples/chalet.ppm");

  image chalet_pgm = ppmtopgm(&chalet, 0.5, 0.5, 0.5);
  image chalet_pbm = ppmtopbm(&chalet, 0.0005);
  creation_fichier("chalet_pgm", chalet_pgm);
  creation_fichier("chalet_pbm", chalet_pbm);

  image clown = init_file("../../Exemples/Clown.256.ppm");

  image clown_pgm = ppmtopgm(&clown, 0.5, 0.5, 0.5);
  image clown_pbm = ppmtopbm(&clown, 0.0005);
  creation_fichier("clown_pgm", clown_pgm);
  creation_fichier("clown_pbm", clown_pbm);

  return 0;
}
