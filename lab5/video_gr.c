#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include <stdint.h>

#include "bios.h"
#include "vbe_info.h"

static void *video_mem;

static uint16_t hres; // XResolution
static uint16_t vres; // YResolution
static uint8_t clrdepth; // color depth (no. of bits per pixel)

void *(vg_init)(uint16_t mode) {
  /* initially, get mode information */
  vbe_mode_info_t vmi_p;
  if (vbe_get_mode_info(mode, &vmi_p) != OK)
    return NULL;

  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  clrdepth = vmi_p.BitsPerPixel;

  int r;
  struct minix_mem_range mr;
  mr.mr_base = vmi_p.PhysBasePtr;
  unsigned int vram_size = hres * vres * bytes_per_pixel();
  mr.mr_limit = mr.mr_base + vram_size;

  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }
  
  /* map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

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

  return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (x + i >= hres || y >= vres) {
      printf("out of range\n");
      return 1;
    }
    
    if (clrdepth == 8) { // mode 0x105
      uint8_t *ptr;
      ptr = (uint8_t *)video_mem + (y * hres + x + i) * bytes_per_pixel();
      *ptr = color;
    }
    else if (clrdepth == 24) { // mode 0x115
      uint8_t *R;
      R = (uint8_t *)video_mem + (y * hres + x + i) * bytes_per_pixel();
      *R = color;

      uint8_t *G;
      G = (uint8_t *)video_mem + (y * hres + x + i) * bytes_per_pixel() + 1;
      *G = color >> 8;

      uint8_t *B;
      B = (uint8_t *)video_mem + (y * hres + x + i) * bytes_per_pixel() + 2;
      *B = color >> 16;
    }
  }

  return 0;
}
  
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++)
    if (vg_draw_hline(x, y + i, width, color) != OK)
      return 1;

  return 0;
}

uint16_t (get_hres)() {
  return hres;
}

uint16_t (get_vres)() {
  return vres;
}

int (bytes_per_pixel)() {
  int bits = (int)clrdepth;
  int bytes = (bits / 8) + ((bits % 8) ? 1 : 0);
  return bytes;
}
