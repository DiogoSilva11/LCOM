#ifndef _LCOM_VBE_INFO_H_
#define _LCOM_VBE_INFO_H_

#include <stdint.h>

uint16_t(get_hres)();

uint16_t(get_vres)();

uint8_t(bytes_per_pixel)();

#endif /* _LCOM_VBE_INFO_H */