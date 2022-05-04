#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include <stdint.h>

#include "bios.h"

static vbe_mode_info_t vmi_p;

void *(vg_init)(uint16_t mode) {
  /* initially, get mode information */
  if (vbe_get_mode_info(mode, &vmi_p))
    return NULL;

  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); // zero the structure

  /* specify the appropriate register values */
  r86.intno = BIOS_INT; 
  r86.ah = VBE_FUNC_CALL; 
  r86.al = VBE_SET_MODE; 
  r86.bx = mode | VBE_LINEAR_BUF; 
 
  if (sys_int86(&r86) != OK) {
    printf("set_vbe_mode: sys_int86() failed\n");
    return NULL;
  }

  if (r86.al == VBE_FUNC_SUP && r86.ah == VBE_FUNC_OK) {
    printf("unsuccessful call\n");
    return NULL;
  }

  
}
  
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  // to do 
  return 0;
}
  
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // to do
  return 0;
}
