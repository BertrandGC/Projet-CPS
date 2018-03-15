#include <stdint.h>

typedef enum {P1=1, P2=2, P3=3} TYPE_IMAGE;


typedef struct{
  TYPE_IMAGE type;
  uint32_t largeur;
  uint32_t hauteur;
  uint16_t val_max;
  uint64_t *data;
}image;

image init_file(char* path);

// image init_std();
