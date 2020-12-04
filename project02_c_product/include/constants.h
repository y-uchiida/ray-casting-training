#ifndef _CONSTANTS_H_
	#define _CONSTANTS_H_

	#define TRUE 1
	#define FALSE 0
	
	#define PI 3.14159265
	#define TWO_PI 6.28318530

	#define FPS 30
	#define FRAME_TIME_LENGTH (1000 / FPS)

	#define TILE_SIZE 32
	#define MAP_NUM_ROWS 13
	#define MAP_NUM_COLS 20
	#define MINIMAP_SCALE_FACTOR 0.3
	#define WINDOW_WIDTH (MAP_NUM_COLS * TILE_SIZE)
	#define WINDOW_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)

	#define FOV_ANGLE (60 * (PI / 180))
	#define NUM_RAYS WINDOW_WIDTH

#endif