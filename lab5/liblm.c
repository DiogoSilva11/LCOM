#include <lcom/lcf.h>
#include <lcom/liblm.h>

#include <stdint.h>

void *lm_alloc(size_t size, mmap_t *map) {
  int r;
  struct minix_mem_range mr;

  /* allow memory mapping */
  mr.mr_base = map->phys;
  mr.mr_limit = mr.mr_base + size;

  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }

  return map->virt;
}

bool lm_free(const mmap_t *map) {
  // to do
  return true;
}
