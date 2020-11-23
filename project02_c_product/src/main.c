#include "../include/raycast.h"

int initializeWindow()
{
	/* SDLライブラリを初期化、失敗時にエラーを返す */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return (FALSE);
	}

	/* ウィンドウの作成 */
	window = SDL_CreateWindow(
		NULL, /* ウィンドウのタイトルバーの内容？ */
		SDL_WINDOWPOS_CENTERED, /* ウィンドウをスクリーン上のどこに表示するかを指定(横方向) */
		SDL_WINDOWPOS_CENTERED, /* ウィンドウをスクリーン上のどこに表示するかを指定(縦方向) */
		WINDOW_WIDTH, /* 表示するウィンドウの幅 */
		WINDOW_HEIGHT, /* 表示するウィンドウの高さ */
		SDL_WINDOW_BORDERLESS /* ボーダー無しでウィンドウを表示 */
	);
	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
		return (FALSE);
	}

	/* rendererの作成 */
	renderer = SDL_CreateRenderer(
		window, /* 作成したwindowポインタを渡すと、それに対するrendererが作成されるっぽい */
		-1, /* デフォルトのドライバを指定 */
		0
	);
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer.\n");
		return (FALSE);
	}

	/* rendererのブレンドモード？を指定する */
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return (TRUE);
}

void destroyWindow ()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/*
 ** ゲームに利用する各種オブジェクトなどの情報を初期化・セットアップする
 */
void setup()
{
    player.x = WINDOW_WIDTH / 2;
    player.y = WINDOW_HEIGHT / 2;
    player.width = 1;
    player.height = 1;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.rotationAngle = PI / 2;
    player.walkSpeed = 100;
    player.turnSpeed = 45 * (PI / 180);
}

/*
 ** マップの描画を行う関数
 */
void renderMap()
{
	int x;
	int y;
	int tile_x;
	int tile_y;
	int tile_color;

	y = 0;
	while (y < MAP_NUM_ROWS)
	{
		x = 0;
		while (x < MAP_NUM_COLS)
		{
			tile_x = x * TILE_SIZE;
			tile_y = y * TILE_SIZE;
			tile_color = (map[y][x] == 1) ? 0 : 255; /* 壁のときは黒く塗る */

			SDL_SetRenderDrawColor(renderer, tile_color, tile_color, tile_color, 255);
			SDL_Rect map_tile = {
				tile_x * MINIMAP_SCALE_FACTOR,
				tile_y * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR
			};
			SDL_RenderFillRect(renderer, &map_tile);
			x++;
		}
		y++;
	}
}

/*
 ** map上の特定の位置が壁かどうかを判定する
 */
int is_wall(float x, float y)
{
	int mapGridIndex_x;
	int mapGridIndex_y;

	if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
		return TRUE;

	mapGridIndex_x = floor(x / TILE_SIZE);
	mapGridIndex_y = floor(y / TILE_SIZE);
	return (map[mapGridIndex_y][mapGridIndex_x] != 0);
}

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

	if (is_wall(newPlayerPos_x, newPlayerPos_y) != TRUE) /* 移動した先が壁ではない場合のみ、座標を更新する */
	{
		player.x = newPlayerPos_x;
		player.y = newPlayerPos_y;
	}
}

/* playerの位置を描画する */
void renderPlayer()
{
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
}

/*
 ** rayを描画する
 */
void renderRays()
{
	int i;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	i = 0;
	while (i < NUM_RAYS)
	{
		SDL_RenderDrawLine(
			renderer,
			MINIMAP_SCALE_FACTOR * player.x,
			MINIMAP_SCALE_FACTOR * player.y,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitY
		);
		i++;
	}

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
			isGameRunning = FALSE;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = FALSE;
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

/*
 ** normalizeAngle(angle):
 ** 与えられた角度を 0 ~ 2π の範囲に変換する
 */
float normalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI);
	if (angle < 0) 
		angle = TWO_PI + angle;
    return angle;
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	/* 三平方の定理で、2点間の距離を導く */
	/* 直角三角形の各辺において、以下が成り立つ */
	/* (斜辺)^2 = (隣辺)^2 + (対辺)^2 */
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 -y1)));
}

void castRay(float rayAngle, int stripID)
{
	/* rayとmapの交点におけるxとyの座標 */
	float xintercept;
	float yintercept;

	/* 次のgridとの交点までの距離 */
	float xstep; 
	float ystep;

	/* 座標検査用の変数 */
	float x_toCheck;
	float y_toCheck;

	int isRayFacingDown;
	int isRayFacingRight;

	rayAngle = normalizeAngle(rayAngle);
	isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;

	/* ----- 1. グリッドの横罫線とrayの交点での、壁との距離を計算する ----- */
	int foundHorzWallHit = FALSE;
	int horzWallContent = 0;
	float horzWallHitX = 0;
	float horzWallHitY = 0;

	/* プレイヤーの座標と向きから、最も近い横罫線との交点のy座標を取得する */
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE; /* TILE_SIZEで除算して、何本目の罫線と交点を持つかがわかる。floorで整数に直してから、タイルの大きさを乗じてグリッドとの交点を求める */
	if (isRayFacingDown){ /* rayが下方向に伸びてている時、プレイヤーの座標よりも下にある罫線との交点のy座標を取らなければならないので… TILE_SIZEを加算して、yinterseptを一つ下のグリッドの座標にする */
		yintercept += TILE_SIZE;
	}

	/* 同じく、最も近い横罫線との交点のx座標を取得する */
	xintercept = player.x + ((yintercept - player.y) / tan(rayAngle));

	/* 次の交点を取得するために、x座標とy座標のそれぞれの増加分(xstep, ystep)を保持する */
	ystep = (isRayFacingDown) ? TILE_SIZE : -TILE_SIZE; /* rayが下方向のとき、TILE_SIZEを加算し、上方向のときはTILE_SIZEを減算する */

    xstep = TILE_SIZE / tan(rayAngle);
    if (isRayFacingRight && xstep < 0){ /* rayが右向きの場合は、xstepは正の数でなければならない(繰り返し処理で右の座標へ動く必要がある)ので、xstepが負の数の場合は符号を反転して正の数にする */
      xstep = -xstep;
    }else if (!(isRayFacingRight) && xstep > 0){ /* rayが左向きの場合は、xstepは負の数でなければならない(繰り返し処理で左の座標へ動く必要がある)ので、xstepが正の数の場合は符号を反転して負の数にする */
      xstep = -xstep;
    }

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	/* 壁と衝突するまで、交点を検査する */
	while (0 <= nextHorzTouchX && nextHorzTouchX <= WINDOW_WIDTH && 0 <= nextHorzTouchY && nextHorzTouchY <= WINDOW_HEIGHT){
		x_toCheck = nextHorzTouchX;
		y_toCheck = nextHorzTouchY + ((!isRayFacingDown) ? -1 : 0);
		if (is_wall(x_toCheck, y_toCheck)){ /* 壁と衝突する交点が見つかったら、x, yの座標を保持してループを抜ける */
			foundHorzWallHit = TRUE;
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = map[(int)floor(y_toCheck / TILE_SIZE)][(int)floor(nextHorzTouchX / TILE_SIZE)];
			break;
		}else{ /* 交点の座標で壁と衝突していない場合、次の交点の座標へ移る */
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
      }
    }

	
	/* ----- 2. グリッドの縦罫線とrayの交点での、壁との距離を計算する ----- */
	int foundVertWallHit = FALSE;
	int vertWallContent = 0;
	float vertWallHitX = 0;
	float vertWallHitY = 0;

	/* プレイヤーの座標と向きから、最も近い縦罫線との交点のx座標を取得する */
    xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE; /* TILE_SIZEで除算して、何本目の罫線と交点を持つかがわかる。floorで整数に直してから、タイルの大きさを乗じてグリッドとの交点を求める */
	if (isRayFacingRight){ /* rayが右方向に伸びてている時、プレイヤーの座標よりも右にある罫線との交点のx座標を取らなければならないので… TILE_SIZEを加算して、xinterseptを一つ右のグリッドの座標にする */
		xintercept += TILE_SIZE;
	}

	/* 同じく、最も近い縦罫線との交点のy座標を取得する */
	yintercept = player.y + ((xintercept - player.x) * tan(rayAngle));

	/* 次の交点を取得するために、x座標とy座標のそれぞれの増加分(xstep, ystep)を保持する */
	xstep = (isRayFacingRight) ? TILE_SIZE : -TILE_SIZE; /* rayが右方向のとき、TILE_SIZEを加算し、左方向のときはTILE_SIZEを減算する */

	ystep = TILE_SIZE * tan(rayAngle);
	if (isRayFacingDown && ystep < 0){ /* rayが下向きの場合は、ystepは正の数でなければならない(繰り返し処理で下の座標へ動く必要がある)ので、符号を反転して正の数にする */
		ystep = -ystep;
	}else if (!(isRayFacingDown) && ystep > 0){ /* rayが上向きの場合は、ystepは負の数でなければならない(繰り返し処理で上の座標へ動く必要がある)ので、符号を反転して負の数にする */
		ystep = -ystep;
    }

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;

	/* 壁と衝突するまで、交点を検査する */
	while (0 <= nextVertTouchX && nextVertTouchX <= WINDOW_WIDTH && 0 <= nextVertTouchY && nextVertTouchY <= WINDOW_HEIGHT)
	{
		x_toCheck = nextVertTouchX + (!(isRayFacingRight) ? -1 : 0);
		y_toCheck = nextVertTouchY;
		if (is_wall(x_toCheck, y_toCheck)){ /* 壁と衝突する交点が見つかったら、x, yの座標を保持してループを抜ける */
		foundVertWallHit = TRUE;
		vertWallHitX = nextVertTouchX;
		vertWallHitY = nextVertTouchY;
		vertWallContent = map[(int)floor(y_toCheck / TILE_SIZE)][(int)floor(x_toCheck / TILE_SIZE)];
		break;
      }else{ /* 交点の座標で壁と衝突していない場合、次の交点の座標へ移る */
		nextVertTouchX += xstep;
		nextVertTouchY += ystep;
      }
    }

	/* ----- 3. 横罫線で壁と衝突する距離と縦罫線で壁と衝突する距離を比較し、小さいほうの値をrayのメンバ変数に保持する ----- */
	float horzHitDistance = (foundHorzWallHit)
		? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY) /* 壁と衝突していたら、その距離を保持 */
		: FLT_MAX; /* 衝突する座標がない場合、数値の最大値を入れておく(比較時に混乱しないように) */
	float vertHitDistance = (foundVertWallHit)
		? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
		: FLT_MAX;
	
	rays[stripID].wallHitX = (horzHitDistance < vertHitDistance) ? horzWallHitX : vertWallHitX;
	rays[stripID].wallHitY = (horzHitDistance < vertHitDistance) ? horzWallHitY : vertWallHitY;
	rays[stripID].distance = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;
	rays[stripID].wallHitContent = (horzHitDistance < vertHitDistance) ? horzWallContent : vertWallContent; 
	rays[stripID].wasHitVertical = (vertHitDistance < horzHitDistance); /* 縦罫線と横罫線のどちらで壁に衝突したかを保持する */

	rays[stripID].rayAngle = rayAngle;
	rays[stripID].isFacingDown = isRayFacingDown;
	rays[stripID].isFacingRight = isRayFacingRight;	
}

void castAllRays()
{
	float rayAngle;
	int stripID;

	rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

	stripID = 0;
	while (stripID < NUM_RAYS)
	{
		castRay(rayAngle, stripID);

		rayAngle += FOV_ANGLE / NUM_RAYS;
		stripID++;
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


/* mainのloop内で画面描画を行う */
void render()
{
	/* 画面の描画内容を初期化(黒で塗りつぶし？) */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	/* ゲーム画面上に配置するものを描画 */
	renderMap();
	renderRays();
	renderPlayer();

	/*sample01: playerの位置に四角形を描画する*/
	// SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	// SDL_Rect rect = {playerX, playerY, 20, 20};
	// SDL_RenderFillRect(renderer, &rect);

	/* バッファに描画した画面をrendererに送信する？ */
	SDL_RenderPresent(renderer);
}

int main ()
{
	isGameRunning = initializeWindow();

	setup();

	while (isGameRunning)
	{
		processInput();
		update();
		render();
	}

	destroyWindow();

	return (0);
}