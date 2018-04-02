#include "image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int dtobin(unsigned char h) {
  double n;
  unsigned int b = 0;

  for (n = 0; n <= 7; n++) {
    b += (pow(10, n) * (h % 2));
    h /= 2;
  }

  return b;
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
    unsigned int bleu;
    unsigned int vert;
    uint64_t pixel;
    while (!feof(file)) {
      printf("//////////////////////////////////////////\n");
      pixel = 0;
      fscanf(file, "%lu %u %u", &pixel, &vert, &bleu);
      printf("vert = %u\n", vert);
      printf("bleu = %u\n", bleu);
      printf("pixel = rouge: %lu\n", pixel);
      pixel = pixel << 16;
      printf("premier décalage: %lu\n", pixel);
      pixel += vert;
      printf("pixel = rouge et vert: %lu\n", pixel);
      pixel = pixel << 16;
      printf("second decalage: %lu\n", pixel);
      pixel += bleu;
      printf("pixel = RGB: %lu\n", pixel);

      img.data[i] = pixel;
      i++;
      printf("//////////////////////////////////////////\n");
    }
    printf("taille tab: %d\n", img.largeur * img.hauteur);
    printf("i: %d\n", i);
    printf("%lu\n", img.data[i]);
  } else {
    printf("Impossible d'ouvrir le fichier\n");
  }

  fclose(file);
  return img;
}

int main(int argc, char const *argv[]) {

  image img = init_file("../../Exemples/chalet.ppm");

  return 0;
}
