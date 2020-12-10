#include "../include/raycast.h"

static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}};

/*
 ** mapの値を返す
*/
int getMapAt(int i, int j)
{
	return (map[i][j]);
}

/*
 ** map上の特定の位置が壁かどうかを判定する
 */
int is_wall(float x, float y)
{
	int mapGridIndex_x;
	int mapGridIndex_y;

	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
		return true;

	mapGridIndex_x = floor(x / TILE_SIZE);
	mapGridIndex_y = floor(y / TILE_SIZE);
	return (map[mapGridIndex_y][mapGridIndex_x] != 0);
}

/*
 ** 与えられた座標が、map内に収まっているか確認する
*/
bool isInsideMap(float x, float y)
{
	return ((0 <= x && x <= WINDOW_WIDTH) && (0 <= y && y <= WINDOW_HEIGHT));
}


/*
 ** マップの描画を行う関数
 */
void renderMap(void)
{
	int x;
	int y;
	int tile_x;
	int tile_y;
	uint32_t tile_color;

	y = 0;
	while (y < MAP_NUM_ROWS)
	{
		x = 0;
		while (x < MAP_NUM_COLS)
		{
			tile_x = x * TILE_SIZE;
			tile_y = y * TILE_SIZE;
			tile_color = (map[y][x] != 0) ? 0xff000000 : 0xffffffff;  // 壁のときは黒く塗る

			/* SDLの関数を使わずにminimapを描画する */
			drawRect(
				tile_x * MINIMAP_SCALE_FACTOR,
				tile_y * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				tile_color
			);

			/* SDLの関数でminimapを描画する例 */
			// SDL_SetRenderDrawColor(renderer, tile_color, tile_color, tile_color, 255);
			// SDL_Rect map_tile = {
			// 	tile_x * MINIMAP_SCALE_FACTOR,
			// 	tile_y * MINIMAP_SCALE_FACTOR,
			// 	TILE_SIZE * MINIMAP_SCALE_FACTOR,
			// 	TILE_SIZE * MINIMAP_SCALE_FACTOR
			// };
			// SDL_RenderFillRect(renderer, &map_tile);
			x++;
		}
		y++;
	}
}