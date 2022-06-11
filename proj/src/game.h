#ifndef _GAME_H_
#define _GAME_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "timer_info.h"
#include "kbd.h"
#include "vbe_func.h"

#include "info.h"
#include "controller.h"
#include "sprites.h"

typedef struct {
  int x;
  int y;
  bool alive;
} Hero;

typedef struct {
  int x;
  int y;
  bool alive;
} Robot;

typedef struct {
  int x;
  int y;
} Fence;

enum Movement {
  RIGHT,
  LEFT,
  DOWN,
  UP,
  DOWN_RIGHT,
  DOWN_LEFT,
  UP_RIGHT,
  UP_LEFT,
  INVALID
};

int (gameplay)();

int (getPixelX)(int logX);

int (getPixelY)(int logY);

int (loadGame)();

int (displayArena)();

int (drawFences)();

int (drawHero)();

int (drawRobots)();

bool (validGame)();

bool (checkCollision)(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

bool (canHeroMove)(uint16_t newX, uint16_t newY);

void (moveHero)(uint8_t key_code);

enum Movement (shortestPath)(int x, int y);

bool (canRobotMove)(int id, uint16_t newX, uint16_t newY);

void (moveRobots)();

#endif /* _GAME_H */
