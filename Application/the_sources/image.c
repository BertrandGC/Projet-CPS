#include "image.h"
#include <stdio.h>
#include <stdlib.h>

image init_file(char *path) {
  FILE *file = fopen(path, "r");
  image img;

  if (file != NULL) {
    char type[2];
    fscanf(file, "%s %u %u %hu", type, &img.largeur, &img.hauteur,
           &img.val_max);
    img.data = malloc(img.largeur * img.hauteur * sizeof(uint64_t));
    printf("%s %u %u %hu\n", type, img.largeur, img.hauteur, img.val_max);
    while (fscanf(file) != EOF) {
      /* code */
    }
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
