#ifndef _LCOM_VBE_INFO_H_
#define _LCOM_VBE_INFO_H_

#include <stdint.h>

uint16_t(get_hres)();

uint16_t(get_vres)();

int(bytes_per_pixel)();

uint32_t(get_color)(uint8_t no_rectangles, uint32_t first, uint8_t step, uint8_t row, uint8_t col);

#endif /* _LCOM_VBE_INFO_H */
