#ifndef _RAYCAST_H_
	#define _RAYCAST_H_
	#include <math.h>
	#include <limits.h>
	#include <stdio.h>
	#include <SDL2/SDL.h>
	#include "./constants.h"

	SDL_Window *window = NULL;  /* SDLライブラリで利用するウィンドウのポインタ */
	SDL_Renderer *renderer = NULL; /* 画面描画を行う機能のポインタ */
	int isGameRunning;

	int ticksLastFrame = 0;

	typedef struct s_player
	{
		float x;
		float y;
		float width;
		float height;
		float rotationAngle;
		float walkSpeed;
		float turnSpeed;
		int turnDirection; /* -1 で左へ旋回、 1で右へ旋回 */
		int walkDirection; /* -1 で後退、 1で前進 */
	}	t_player;

	t_player player;

	struct Ray
	{
		float rayAngle;
		float wallHitX;
		float wallHitY;
		float distance;
		int wasHitVertical;
		int isFacingDown;
		int isFacingRight;
		int wallHitContent;
	} rays[NUM_RAYS];

	const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = 
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};
#endif