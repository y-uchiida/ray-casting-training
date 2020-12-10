#include "../include/raycast.h"

t_player player = {
	.x = WINDOW_WIDTH / 2,
	.y = WINDOW_HEIGHT / 2,
	.width = 1,
	.height = 1,
	.turnDirection = 0,
	.walkDirection = 0,
	.rotationAngle = PI / 2,
	.walkSpeed = 100,
    .turnSpeed = 45 * (PI / 180)
};


/* playerの座標を更新し、移動させる */
void movePlayer(float deltaTime)
{
	float moveStep;
	float newPlayerPos_x;
	float newPlayerPos_y;

    player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;

	moveStep = player.walkDirection * player.walkSpeed * deltaTime;

	newPlayerPos_x = player.x + cos(player.rotationAngle) * moveStep;
	newPlayerPos_y = player.y + sin(player.rotationAngle) * moveStep;

	if (is_wall(newPlayerPos_x, newPlayerPos_y) != true) /* 移動した先が壁ではない場合のみ、座標を更新する */
	{
		player.x = newPlayerPos_x;
		player.y = newPlayerPos_y;
	}
}

/* playerの位置を描画する */
void renderPlayer(void)
{
	drawRect(
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR,
		0xffff0000
	);

	/*
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect playerRect = {
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR
	};
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderDrawLine(
		renderer,
		MINIMAP_SCALE_FACTOR * player.x,
		MINIMAP_SCALE_FACTOR * player.y,
		MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 40,
		MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 40
	);
	*/
}