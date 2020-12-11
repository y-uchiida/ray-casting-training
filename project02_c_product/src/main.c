#include "../include/raycast.h"

/*
 ** ゲームに利用する各種オブジェクトなどの情報を初期化・セットアップする
 */
bool setup()
{
	/* ヘッダファイルで宣言だけ行った変数を初期化 */
	ticksLastFrame = 0;

	/* upngライブラリでpng画像を読み込み */
	if (loadWallTextures() == false)
	{
		fprintf(stderr, "Error: failed load wall texture files.'\n");
		return (false);
	}

	/* png画像を読込するので、テクスチャの配列データはロードしなくてよい */
	// /* 壁のテクスチャを配列に取り込みする */
	// textures[0] = (Uint32 *)REDBRICK_TEXTURE;
	// textures[1] = (Uint32 *)PURPLESTONE_TEXTURE;
	// textures[2] = (Uint32 *)MOSSYSTONE_TEXTURE;
	// textures[3] = (Uint32 *)GRAYSTONE_TEXTURE;
	// textures[4] = (Uint32 *)COLORSTONE_TEXTURE;
	// textures[5] = (Uint32 *)BLUESTONE_TEXTURE;
	// textures[6] = (Uint32 *)WOOD_TEXTURE;
	// textures[7] = (Uint32 *)EAGLE_TEXTURE;

	/* テクスチャ画像のデータをtextures.hから取得するので不要 */
	// /* 壁に表示するテクスチャ画像を生成 */
	// /* まずはデータ格納用のメモリ領域を確保 */
	// wallTexture = (Uint32 *)malloc(sizeof(Uint32) * (Uint32)WINDOW_WIDTH * (Uint32)WINDOW_HEIGHT); 
	// /* loopで色を設定 */
	// x = 0;
	// while(x < TEXTURE_WIDTH)
	// {
	// 	y = 0;
	// 	while (y < TEXTURE_HEIGHT)
	// 	{
	// 		wallTexture[(TEXTURE_WIDTH * y) + x] = (x % 8 && y % 8) ? 0xff0000ff : 0xff000000;
	// 		y++;
	// 	}
	// 	x++;
	// }

	return (true);
}


/*
 ** ゲーム実行中に発生したinputイベントの発生を検知して処理する
 */
void processInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{
			isGameRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = false;
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = +1;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = -1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = +1;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = -1;
			break;
		}
		case SDL_KEYUP: 
		{
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = 0;
			break;
		}
	}
}



/* 画面描画前の処理 */
void update()
{
	/* 次回のフレーム更新時間を算出する */
	int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
	/* 前回のフレーム描画からどれだけの時間が経過したかを保持する */
	float deltaTime;

	/* フレーム更新の時間が来るまでの間はupdateを行わない */
	if (0 < timeToWait && timeToWait <= FRAME_TIME_LENGTH)
		SDL_Delay(timeToWait);

	/* 今の時間から、前回の描画のタイミング(ms)を引くと、今回のフレームで変化させる量がわかる */
	deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	ticksLastFrame = SDL_GetTicks();

	movePlayer(deltaTime);

	/* rayを投げて、ぶつかる位置を探す */
	castAllRays();
}

// void generate3DProjection() {
//     for (int i = 0; i < NUM_RAYS; i++) {
//         float perpDistance = rays[i].distance * cos(rays[i].rayAngle - player.rotationAngle);
//         float distanceProjPlane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
//         float projectedWallHeight = (TILE_SIZE / perpDistance) * distanceProjPlane;

//         int wallStripHeight = (int)projectedWallHeight;

//         int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
//         wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

//         int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
//         wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

//         // render the wall from wallTopPixel to wallBottomPixel
//         for (int y = wallTopPixel; y < wallBottomPixel; y++) {
//             colorBuffer[(WINDOW_WIDTH * y) + i] = rays[i].wasHitVertical ? 0xFFFFFFFF : 0xFFCCCCCC;
//         }
//     }
// }

/* mainのloop内で画面描画を行う */
void render()
{
	/* colorBufferをクリアする */
	clearColorBuffer(0xFF000000);

	renderWallProjection();

	/* ゲーム画面上に配置するものを描画 */
	renderMap();
	renderRays();
	renderPlayer();

	renderColorBuffer();

	/*sample01: playerの位置に四角形を描画する*/
	// SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	// SDL_Rect rect = {playerX, playerY, 20, 20};
	// SDL_RenderFillRect(renderer, &rect);

}

int main ()
{
	if ((isGameRunning = initializeWindow()) == false)
	{
		fprintf(stderr, "Error: SDL window cant start.\n");
		return(-1);
	}

	if(setup() == false)
	{
		fprintf(stderr, "start game failed.\n");
		return (-1);
	}

	while (isGameRunning)
	{
		processInput();
		update();
		render();
	}

	destroyWindow();

	return (EXIT_SUCCESS);
}