#include "../include/raycast.h"

ray_t rays[NUM_RAYS];

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
	bool foundHorzWallHit = false;
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
	while (isInsideMap(nextHorzTouchX, nextHorzTouchY))
	// while (0 <= nextHorzTouchX && nextHorzTouchX <= WINDOW_WIDTH && 0 <= nextHorzTouchY && nextHorzTouchY <= WINDOW_HEIGHT)
	{
		x_toCheck = nextHorzTouchX;
		y_toCheck = nextHorzTouchY + ((!isRayFacingDown) ? -1 : 0);
		if (is_wall(x_toCheck, y_toCheck)){ /* 壁と衝突する交点が見つかったら、x, yの座標を保持してループを抜ける */
			foundHorzWallHit = true;
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = getMapAt((int)floor(y_toCheck / TILE_SIZE), (int)floor(nextHorzTouchX / TILE_SIZE));
			break;
		}else{ /* 交点の座標で壁と衝突していない場合、次の交点の座標へ移る */
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
      }
    }

	
	/* ----- 2. グリッドの縦罫線とrayの交点での、壁との距離を計算する ----- */
	bool foundVertWallHit = false;
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
	while(isInsideMap(nextVertTouchX, nextVertTouchY))
	// while (0 <= nextVertTouchX && nextVertTouchX <= WINDOW_WIDTH && 0 <= nextVertTouchY && nextVertTouchY <= WINDOW_HEIGHT)
	{
		x_toCheck = nextVertTouchX + (!(isRayFacingRight) ? -1 : 0);
		y_toCheck = nextVertTouchY;
		if (is_wall(x_toCheck, y_toCheck)){ /* 壁と衝突する交点が見つかったら、x, yの座標を保持してループを抜ける */
		foundVertWallHit = true;
		vertWallHitX = nextVertTouchX;
		vertWallHitY = nextVertTouchY;
		vertWallContent = getMapAt((int)floor(y_toCheck / TILE_SIZE), (int)floor(x_toCheck / TILE_SIZE));
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

void castAllRays(void)
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

/*
 ** rayを描画する
 */
void renderRays(void)
{
	/*
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
	*/
}