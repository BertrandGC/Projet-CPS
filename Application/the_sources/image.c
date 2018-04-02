#include "image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

image init(uint32_t largeur, uint32_t hauteur, uint16_t val_max) {
  image tmp;
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
  image img_pgm = init(img->largeur, img->hauteur, img->val_max);

  for (int i = 0; i < img->largeur * img->hauteur; i++) {
    bleu = img->data[i] & masque16;
    vert = (img->data[i] >> 16) & masque16;
    rouge = (img->data[i] >> 32) & masque16;

    img_pgm.data[i] = coeff_r * rouge + coeff_v * vert + coeff_b * bleu;
  }
  return img_pgm;
}

int main(int argc, char const *argv[]) {

  image img = init_file("../../Exemples/chalet.ppm");

  image grey_img = ppmtopgm(&img, 0.5, 0.5, 0.5);

  return 0;
}
