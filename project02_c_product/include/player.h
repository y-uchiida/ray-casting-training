#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "./defs.h"

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
} t_player;

void movePlayer(float deltaTime);
void renderPlayer(void);


extern t_player player;

#endif