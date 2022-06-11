#include <lcom/lcf.h>

#include "game.h"

// ----------------------------------------------------------------------------------------

static Hero hero;
static Fence fences[NUM_FENCES];
static Robot robots[NUM_ROBOTS];

// ----------------------------------------------------------------------------------------

int (gameplay)() {
  if (loadGame())
    return 1;

  int r, ipc_status;
  message msg;

  int freq = 60;
  int_counter = 0;
  uint8_t key_code = 0;

  int process = 1;
  while (process) {
      /* Get a request message. */
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { // received notification
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification
              if (msg.m_notify.interrupts & timer_irq_set) { // subscribed interrupt (Timer 0)
                timer_int_handler();
                if (!(int_counter % (freq / GAME_FPS))) {
                  if (!validGame())
                    return 0;

                  if (displayArena())
                    return 1;
                }
                if (!(int_counter % freq)) {
                  moveRobots();
                }
              }
              if (msg.m_notify.interrupts & kbd_irq_set) { // subscribed interrupt (Keyboard)
                kbc_ih();
                if (!kbd_inc_code()) {
                  key_code = get_key();

                  if (key_code == KEY_ESC)
                    return 0;

                  moveHero(key_code);
                }
              }
              break;
            default:
              break; // no other notifications expected: do nothing
          }
      }
      else { // received a standard message, not a notification
        // no standard messages expected: do nothing
      }
  }

  return 0;
}

// ----------------------------------------------------------------------------------------

int (getPixelX)(int logX) {
  return ARENA_X_ORIGIN + logX * ENTITY_WIDTH;
}

int (getPixelY)(int logY) {
  return ARENA_Y_ORIGIN + logY * ENTITY_HEIGHT;
}

int (loadGame)() {
  hero.x = getPixelX(11);
  hero.y = getPixelY(11);
  hero.alive = true;

  int cnt = 0;

  /* top row */
  for (int i = 0; i < ARENA_WIDTH; i++) {
    Fence f = {getPixelX(i), getPixelY(0)};
    fences[cnt] = f;
    cnt++;
  }

  /* bottom row */
  for (int i = 0; i < ARENA_WIDTH; i++) {
    Fence f = {getPixelX(i), getPixelY(21)};
    fences[cnt] = f;
    cnt++;
  }

  /* right column */
  for (int i = 1; i <= (ARENA_HEIGHT - 2); i++) {
    Fence f = {getPixelX(0), getPixelY(i)};
    fences[cnt] = f;
    cnt++;
  }

  /* left column */
  for (int i = 1; i <= (ARENA_HEIGHT - 2); i++) {
    Fence f = {getPixelX(21), getPixelY(i)};
    fences[cnt] = f;
    cnt++;
  }

  /* random fences inside the arena */

  Fence f1 = {getPixelX(3), getPixelY(3)};
  fences[cnt] = f1;
  cnt++;

  Fence f2 = {getPixelX(5), getPixelY(10)};
  fences[cnt] = f2;
  cnt++;

  Fence f3 = {getPixelX(16), getPixelY(4)};
  fences[cnt] = f3;
  cnt++;

  Fence f4 = {getPixelX(18), getPixelY(17)};
  fences[cnt] = f4;
  cnt++;

  Fence f5 = {getPixelX(9), getPixelY(18)};
  fences[cnt] = f5;
  cnt++;

  Fence f6 = {getPixelX(15), getPixelY(12)};
  fences[cnt] = f6;
  cnt++;

  Fence f7 = {getPixelX(15), getPixelY(19)};
  fences[cnt] = f7;
  cnt++;

  Fence f8 = {getPixelX(5), getPixelY(16)};
  fences[cnt] = f8;
  cnt++;

  Fence f9 = {getPixelX(8), getPixelY(6)};
  fences[cnt] = f9;
  cnt++;

  Fence f10 = {getPixelX(18), getPixelY(10)};
  fences[cnt] = f10;
  cnt++;

  if (cnt != NUM_FENCES) {
    if (leave())
      return 1;
    return 1;
  }

  /* random robots inside the arena */

  Robot r0 = {getPixelX(11), getPixelY(5), true};
  robots[0] = r0;

  Robot r1 = {getPixelX(4), getPixelY(6), true};
  robots[1] = r1;

  Robot r2 = {getPixelX(19), getPixelY(13), true};
  robots[2] = r2;

  Robot r3 = {getPixelX(7), getPixelY(18), true};
  robots[3] = r3;

  Robot r4 = {getPixelX(17), getPixelY(19), true};
  robots[4] = r4;

  Robot r5 = {getPixelX(2), getPixelY(2), true};
  robots[5] = r5;

  Robot r6 = {getPixelX(9), getPixelY(2), true};
  robots[6] = r6;

  Robot r7 = {getPixelX(15), getPixelY(14), true};
  robots[7] = r7;

  Robot r8 = {getPixelX(3), getPixelY(16), true};
  robots[8] = r8;

  Robot r9 = {getPixelX(18), getPixelY(3), true};
  robots[9] = r9;

  return 0;
}

// ----------------------------------------------------------------------------------------

int (displayArena)() {
  vg_clear_screen();

  if (drawFences())
    return 1;

  if (drawHero())
    return 1;

  if (drawRobots())
    return 1;

  vg_draw_content();

  return 0;
}

int (drawFences)() {
  for (int i = 0; i < NUM_FENCES; i++) {
    if (vg_draw_element(fence_sprite, fences[i].x, fences[i].y)) {
      if (leave())
        return 1;
      return 1;
    }
  }

  return 0;
}

int (drawHero)() {
  if (vg_draw_element(hero_sprite, hero.x, hero.y)) {
    if (leave())
      return 1;
    return 1;
  }

  return 0;
}

int (drawRobots)() {
  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (robots[i].alive) {
      if (vg_draw_element(robot_sprite, robots[i].x, robots[i].y)) {
        if (leave())
          return 1;
        return 1;
      }
    }
    else {
      if (vg_draw_element(dead_robot_sprite, robots[i].x, robots[i].y)) {
        if (leave())
          return 1;
        return 1;
      }
    }
  }

  return 0;
}

// ----------------------------------------------------------------------------------------

bool (validGame)() {
  if (!hero.alive)
    return false;

  int dead_robots = 1;
  for(int i = 0 ; i < NUM_ROBOTS; i++)
    if (!robots[i].alive)
      dead_robots++;

  if (dead_robots == NUM_ROBOTS)
    return false;

  return true;
}

bool (checkCollision)(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return ((x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2));
}

bool (canHeroMove)(uint16_t newX, uint16_t newY) {
  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, robots[i].x, robots[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      if (robots[i].alive)
        hero.alive = false;
      return false;
    }
  }

  for (int i = 0; i < NUM_FENCES; i++) {
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, fences[i].x, fences[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      hero.alive = false;
      return false;
    }
  }

  return true;
}

void (moveHero)(uint8_t key_code) {
  switch (key_code) {
      case KEY_RIGHT:
        if (canHeroMove(hero.x + HERO_X_SPEED, hero.y))
          if (hero.alive)
            hero.x += HERO_X_SPEED;
        break;
      case KEY_LEFT:
        if (canHeroMove(hero.x - HERO_X_SPEED, hero.y))
          if (hero.alive)
            hero.x -= HERO_X_SPEED;
        break;
      case KEY_UP:
        if (canHeroMove(hero.x, hero.y - HERO_Y_SPEED))
          if (hero.alive)
            hero.y -= HERO_Y_SPEED;
        break;
      case KEY_DOWN:
        if (canHeroMove(hero.x, hero.y + HERO_Y_SPEED))
          if (hero.alive)
            hero.y += HERO_Y_SPEED;
        break;
      default:
        break;
    }
}

// ----------------------------------------------------------------------------------------

enum Movement (shortestPath)(int x, int y) {
  if (hero.y == y) { // hero and robot are in the same row
		if (hero.x > x) // hero is to the right of the robot
			return RIGHT; // robot moves right
		else if (hero.x < x) // hero is to the left of the robot
			return LEFT; // robot moves left
	}
	else if (hero.x == x) { // hero and robot are in the same column
		if (hero.y > y) // hero is below the robot
			return DOWN; // robot moves down
		else if (hero.y < y) // player is above the robot
			return UP; // robot moves up
	}
	else if (hero.y > y) { // hero is below the robot 
		if (hero.x > x) // hero is to the right of the robot
			return DOWN_RIGHT; // robot moves down and to the right
		else if (hero.x < x) // hero is to the left of the robot
			return DOWN_LEFT; // robot moves down and to the left
	}
	else if (hero.y < y) { // hero is above the robot
		if (hero.x > x) // hero is to the right of the robot
			return UP_RIGHT; // robot moves up and to the right
		else if (hero.x < x) // hero is to the left of the robot
			return UP_LEFT; // robot moves up and to the left
	}

  return INVALID;
}

bool (canRobotMove)(int id, uint16_t newX, uint16_t newY) {
  if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, hero.x, hero.y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
    hero.alive = false;
    return false;
  }

  for (int i = 0; i < NUM_FENCES; i++) {
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, fences[i].x, fences[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      robots[id].alive = false;
      return false;
    }
  }

  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, robots[i].x, robots[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      if (!robots[i].alive)
        return false;
    }
  }

  return true;
}

void (moveRobots)() {
  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (robots[i].alive) {
      switch (shortestPath(robots[i].x, robots[i].y)) {
        case RIGHT:
          if (canRobotMove(i, robots[i].x + ROBOT_X_SPEED, robots[i].y))
            if (robots[i].alive)
              robots[i].x += ROBOT_X_SPEED;
          break;
        case LEFT:
          if (canRobotMove(i, robots[i].x - ROBOT_X_SPEED, robots[i].y))
            if (robots[i].alive)
          robots[i].x -= ROBOT_X_SPEED;
          break;
        case DOWN:
          if (canRobotMove(i, robots[i].x, robots[i].y + ROBOT_Y_SPEED))
            if (robots[i].alive)
          robots[i].y += ROBOT_Y_SPEED;
          break;
        case UP:
          if (canRobotMove(i, robots[i].x, robots[i].y - ROBOT_Y_SPEED))
            if (robots[i].alive)
          robots[i].y -= ROBOT_Y_SPEED;
          break;
        case DOWN_RIGHT:
          if (canRobotMove(i, robots[i].x + ROBOT_X_SPEED, robots[i].y + ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x += ROBOT_X_SPEED;
              robots[i].y += ROBOT_Y_SPEED;
            }
          }
          break;
        case DOWN_LEFT:
          if (canRobotMove(i, robots[i].x - ROBOT_X_SPEED, robots[i].y + ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x -= ROBOT_X_SPEED;
              robots[i].y += ROBOT_Y_SPEED;
            }
          }
          break;
        case UP_RIGHT:
          if (canRobotMove(i, robots[i].x + ROBOT_X_SPEED, robots[i].y - ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x += ROBOT_X_SPEED;
              robots[i].y -= ROBOT_Y_SPEED;
            }
          }
          break;
        case UP_LEFT:
          if (canRobotMove(i, robots[i].x - ROBOT_X_SPEED, robots[i].y - ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x -= ROBOT_X_SPEED;
              robots[i].y -= ROBOT_Y_SPEED;
            }
          }
          break;  
        default:
          break;
      }
    }
  }
} 

// ----------------------------------------------------------------------------------------
