#include "../include/graphics.h"


static SDL_Window *window;  /* SDLライブラリで利用するウィンドウのポインタ */
static SDL_Renderer *renderer; /* 画面描画を行う機能のポインタ */

static uint32_t *colorBuffer;
// Uint32 *wallTexture = NULL; /* 壁のテクスチャをloopで生成するためのもの。画像テクスチャを利用するので不要 */
// Uint32 *textures[NUM_TEXTURES];

static SDL_Texture *colorBufferTexture;

bool initializeWindow(void)
{
	/* SDLライブラリを初期化、失敗時にエラーを返す */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return (false);
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
		return (false);
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
		return (false);
	}

	/* rendererのブレンドモード？を指定する */
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * (uint32_t)WINDOW_WIDTH * (uint32_t)WINDOW_HEIGHT);
	if (colorBuffer == NULL)
	{
		fprintf(stderr, "Error: failed memory alocate for colorBuffer.\n");
		return (false);
	}

	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);
	if (colorBufferTexture == NULL)
	{
		fprintf(stderr, "Error: failed create texture for 'colorBufferTexture.'\n");
		return (false);
	}

	return (true);
}

void destroyWindow (void)
{
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/*
 ** colorBufferを指定の色で塗りつぶす
 ** yが内側のwhileの条件になっているので、縦一列を塗りつぶして次の列に移動する
 */
void clearColorBuffer(uint32_t color)
{
	int x;
	int y;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		y = 0;
		while (y < WINDOW_HEIGHT)
		{
			colorBuffer[(WINDOW_HEIGHT * y) + x] = color;
			y++;
		}
		x++;
	}
}

/*
 ** colorBufferのレンダリングを行う？
 */
void renderColorBuffer(void)
{
	/* 1.Textureに対して、2.指定の領域の長方形を、 3.指定の色で塗りつぶし */
	/* NULLを与えるとTexture全体を対象にする */
	SDL_UpdateTexture(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(int)((Uint32)WINDOW_WIDTH * sizeof(Uint32))
	);

	SDL_RenderCopy(
		renderer,
		colorBufferTexture,
		NULL,
		NULL
	);
	
	/* バッファに描画した画面をrendererに送信する？ */
	SDL_RenderPresent(renderer);
}

void drawPixel(int x, int y, uint32_t color)
{
	colorBuffer[(WINDOW_WIDTH * y) + x] = color;
	return ;
}

/* rendererに長方形を描画する */
void drawRect(int x, int y, int width, int height, uint32_t color)
{
	int i;
	int j;

	i = x;
	while(i <= (x + width))
	{
		j = y;
		while(j <= (y + height))
		{
			drawPixel(i, j, color);
			j++;
		}
		i++;
	}
}