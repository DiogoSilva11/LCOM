#include <lcom/lcf.h>
#include <lcom/liblm.h>

#include <stdint.h>

void *(lm_alloc)(size_t size, mmap_t *map) {
  if (map == NULL) {
    printf("null pointer\n");
    return NULL;
  }

  int mb = 1024 * 1024;
  map->phys = mb;
  map->size = size;
  map->virt = malloc(size);

  return map->virt;
}

bool (lm_free)(const mmap_t *map) {
  if (map == NULL) {
    printf("null pointer\n");
    return false;
  }

  free((void *)map);

  return true;
}
