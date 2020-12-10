#ifndef _RAY_H_
#define _RAY_H_

#include <stdbool.h>

typedef struct Ray
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int isFacingDown;
	int isFacingRight;
	int wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];

void castRay(float rayAngle, int stripID);
float normalizeAngle(float angle);
float distanceBetweenPoints(float x1, float y1, float x2, float y2);
void castAllRays(void);
void renderRays(void);

#endif