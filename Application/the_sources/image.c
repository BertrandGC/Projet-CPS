#include "image.h"
#include <stdio.h>

image init_file(char *path) {
  FILE *file = fopen(path, "r");
  if (file != NULL) {
    image img;
    char *type;
    fscanf(file, "%s %u %u %hu", type, &img.largeur, &img.hauteur,
           &img.val_max);
  } else {
    printf("Impossible d'ouvrir le fichier\n");
  }
}

int main(int argc, char const *argv[]) {
  /* code */
  return 0;
}
