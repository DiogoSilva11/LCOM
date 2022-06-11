#ifndef _LCOM_VBE_FUNC_H_
#define _LCOM_VBE_FUNC_H_

#include <stdint.h>

int(map_memory)(uint16_t mode);

uint16_t(get_hres)();

uint16_t(get_vres)();

int(bytes_per_pixel)();

int(vg_generate_pixel)(uint16_t x, uint16_t y, uint32_t color);

void(vg_clear_screen)();

void(vg_draw_content)();

uint8_t*(vg_read_pixmap)(xpm_row_t *pixmap, uint16_t *width, uint16_t *height);

int(vg_draw_sprite)(uint8_t *sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

int(vg_draw_element)(xpm_row_t *pixmap, uint16_t x, uint16_t y);

#endif /* _LCOM_VBE_FUNC_H */
