#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb==NULL) return 1;
  *lsb = val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if(msb==NULL) return 1;
    *msb = val >> 8; 
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
<<<<<<< HEAD
  if (value == NULL) return 1;

  uint32_t v = 0;
  if (sys_inb(port, &v)) return 1;
=======
  if (value == NULL)
    return 1;

  uint32_t v = 0;
  if (sys_inb(port, &v))
    return 1;

>>>>>>> 548933dd2dee9f487de2b115e277b04bbebfc2dc
  *value = v;
  return 0;
}
