#ifndef _RAYCAST_H_
	#define _RAYCAST_H_
	#include <math.h>
	#include <limits.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <stdio.h>
	#include <SDL2/SDL.h>
	#include "./upng.h"
	#include "./defs.h"
	#include "./graphics.h"
	#include "./textures.h"

	bool isGameRunning;

	int ticksLastFrame;




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
#endif