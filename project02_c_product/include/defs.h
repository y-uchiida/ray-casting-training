#ifndef _DEFS_H_
	#define _DEFS_H_
	
	#define PI 3.14159265
	#define TWO_PI 6.28318530

	#define FPS 30
	#define FRAME_TIME_LENGTH (1000 / FPS)

	#define TILE_SIZE 64
	#define MAP_NUM_ROWS 13
	#define MAP_NUM_COLS 20
	#define NUM_TEXTURES 9

	#define MINIMAP_SCALE_FACTOR 0.2
	#define WINDOW_WIDTH 1280
	#define WINDOW_HEIGHT 800

	#define TEXTURE_WIDTH 64
	#define TEXTURE_HEIGHT 64

	#define FOV_ANGLE (60 * (PI / 180))
	#define NUM_RAYS WINDOW_WIDTH

	/* 壁のゆがみを補正するため、画面平面と垂直な直線の長さに直す */
	#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

#endif