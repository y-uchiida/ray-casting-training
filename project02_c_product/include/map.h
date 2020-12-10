#ifndef _MAP_H_
#define _MAP_H_

#include <stdbool.h>
#include "./defs.h"
#include "./graphics.h"

#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20

int is_wall(float x, float y);
bool isInsideMap(float x, float y);
void renderMap(void);
int getMapAt(int i, int j);

#endif