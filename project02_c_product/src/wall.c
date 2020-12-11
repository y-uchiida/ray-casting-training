#include "../include/raycast.h"

void renderWallProjection(void)
{
	int x;
	int y;
	float projectedWallHeight;
	float perpDistance;
	int wallStripHeight;
	int wallTopPixel;
	int wallBottomPixel;
	int texNum;
	uint32_t texture_pixel_color;
	int textureOffsetX;
	int textureOffsetY;
	int texture_width;
	int texture_height;
	int distanceFromTop;

	x = 0;
	while(x < NUM_RAYS)
	{
		perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle); /* 壁のゆがみを補正するため、画面平面と垂直な直線の長さに直す */
		projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE; /* 描画する壁の高さ */
		wallStripHeight = (int)projectedWallHeight;
		wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2); /* 画面の高さの中心から、描画する壁の高さの半分だけ上にずらすと、壁の一番上の位置のピクセルがわかる */
		wallTopPixel = (wallTopPixel < 0) ? 0 : wallTopPixel; /* 画面領域からはみ出す場合は0に直す */
		wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = (wallBottomPixel > WINDOW_HEIGHT) ? WINDOW_HEIGHT : wallBottomPixel; /* 画面領域からはみ出す場合は0に直す */

		/* 今回のrayにおけるテクスチャの参照位置を、壁との交点座標から得る */
		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE; /* TILE_SIZEとTEXTURE_HEIGHTは同じ値で設定しているので、余りを取るとテクスチャ1枚分の座標位置がわかる */
		else
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
		
		/* ぶつかった壁のインデックスの値を見て、参照するテクスチャを決定する */
		texNum = rays[x].wallHitContent - 1;
        texture_width = wallTextures[texNum].width;
        texture_height = wallTextures[texNum].height;

		/* 画面の上端から壁の下端まで、colorBufferの値を変更する */
		y = 0;
		while(y < WINDOW_HEIGHT)
		{
			if (y < wallTopPixel)
				drawPixel(x, y, 0xff333333);
				// colorBuffer[(WINDOW_WIDTH * y) + x] = 0xff333333;
			else if (wallTopPixel <= y && y < wallBottomPixel)
			{
				/* 描画する壁の高さが画面の高さを超えている(画面からはみ出す)場合、テクスチャ上の参照すべきy座標ははみ出す分だけ下にずれていなければならない */
				/* distanceFromTopに、はみ出し部分を考慮したy座標を保持する */
				/* distanceFromTop = (今回描画するピクセルのy座標) - ( 壁の描画開始位置(調整していないのでマイナスの値になる場合あり) ) */
				distanceFromTop = y - ( (WINDOW_HEIGHT / 2) - (wallStripHeight / 2) );

				/* wallStripHeightは、壁との交点の位置の距離によって伸縮する */
				/* wallStripHeight(描画する壁の高さ)でtexture_heightを割った比率をかけてやることで、テクスチャ上のピクセルの位置がわかる */
				textureOffsetY = (int)((distanceFromTop) * ( (float)texture_height / wallStripHeight ));

				texture_pixel_color = wallTextures[texNum].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];

				/* rayが縦軸方向のグリッドに当たった場合、描画色を暗くする */
				if (rays[x].wasHitVertical)
					changeColorIntensity(&texture_pixel_color, 0.65);
				drawPixel(x, y, texture_pixel_color);
				// colorBuffer[(WINDOW_WIDTH * y) + x] = texture_pixel_color;
			}
			else
				drawPixel(x, y, 0xff777777);
				// colorBuffer[(WINDOW_WIDTH * y) + x] = 0xff777777;
			y++;
		}
		x++;
	}
}

void changeColorIntensity(color_t *color, float factor)
{
	color_t a;
	color_t r;
	color_t g;
	color_t b;

	a = (*color & 0xff000000);
	r = (*color & 0x00ff0000) * factor;
	g = (*color & 0x0000ff00) * factor;
	b = (*color & 0x000000ff) * factor;

	*color = (a | (r & 0x00ff0000) | (g & 0x0000ff00) | (b & 0x000000ff));
}
