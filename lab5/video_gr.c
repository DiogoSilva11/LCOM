#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include <stdint.h>

void *(vg_init)(uint16_t mode) {
  // to do
}
  
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  // to do 
  return 0;
}
  
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // to do
  return 0;
}
  
int (vg_exit)() {
  // to do
  return 0;
} 
   
int (vg_display_vbe_contr_info)(struct vg_vbe_contr_info *info_p) {
  // to do
  return 0;
}
