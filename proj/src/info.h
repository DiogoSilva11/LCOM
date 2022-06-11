#ifndef _INFO_H_
#define _INFO_H_

#include <stdint.h>

#define GAME_MODE 0x105
#define GAME_FPS 60
#define CONSOLE_WIDTH 1024
#define CONSOLE_HEIGHT 768

#define GREY 7
// colors

#define KEY_1 0x82
#define KEY_0 0x8b

#define KEY_ESC 0x01

#define KEY_RIGHT 0x4D
#define KEY_LEFT 0x4B
#define KEY_UP 0x48
#define KEY_DOWN 0x50

#define ARENA_WIDTH 22
#define ARENA_HEIGHT 22
#define ARENA_X_ORIGIN 204
#define ARENA_Y_ORIGIN 60

#define NUM_ROBOTS 10
#define NUM_FENCES 94

#define ENTITY_WIDTH 28
#define ENTITY_HEIGHT 25

#define HERO_X_SPEED 14
#define HERO_Y_SPEED 12
#define ROBOT_X_SPEED 4
#define ROBOT_Y_SPEED 3

extern int timer_irq_set;
extern int kbd_irq_set;
extern int mouse_irq_set;

#endif /* _INFO_H */
