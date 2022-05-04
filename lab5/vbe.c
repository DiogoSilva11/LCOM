#include <lcom/lcf.h>
#include <lcom/vbe.h>

#include <stdint.h>

#include "bios.h"

static mmap_t map;

int (vbe_get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); // zero the structure
  memset(vmi_p, 0, sizeof(*vmi_p)); // zero the structure

  vmi_p = lm_alloc(sizeof(*vmi_p), &map); // initialize buf
  
  /* specify the appropriate register values */
  r86.intno = BIOS_INT;
  r86.ah = VBE_FUNC_CALL;
  r86.al = VBE_MODE_INFO;
  r86.cx = mode;

  /* translate the buffer linear address to a far pointer */
  r86.es = PB2BASE(map.phys); // set a segment base
  r86.di = PB2OFF(map.phys); // set the offset accordingly

  if (sys_int86(&r86) != OK) { // call BIOS
    printf("get_vbe_mode_info: sys_int86() failed\n");
    return 1;
  }

  if (!(r86.al == VBE_FUNC_SUP && r86.ah == VBE_FUNC_OK)) {
    printf("unsuccessful call\n");
    return 1;
  }

  return 0;
}
