#include "../include/raycast.h"

#include <stdlib.h>
#include <fcntl.h>

static const char *textureFileNames[NUM_TEXTURES] = {
	"./images/redbrick.png",
	"./images/purplestone.png",
	"./images/mossystone.png",
	"./images/graystone.png",
	"./images/colorstone.png",
	"./images/bluestone.png",
	"./images/wood.png",
	"./images/eagle.png",
	"./images/pikuma.png"
};

bool loadWallTextures()
{
	int i;
	upng_t *upng;

	i = 0;
	while (i < NUM_TEXTURES)
	{
		upng = upng_new_from_file(textureFileNames[i]); /* pngファイルのデータをロードする */
		if (upng != NULL)
		{
			// printf("upng: %p\n", upng);
			upng_decode(upng); /* upngのデータをデコードする(カラーコード情報に変換する？？) */
			if (upng_get_error(upng) == UPNG_EOK)
			{
				wallTextures[i].upngTexture = upng;
				wallTextures[i].width = upng_get_width(upng);
				wallTextures[i].height = upng_get_height(upng);
				wallTextures[i].texture_buffer = (uint32_t *)upng_get_buffer(upng);
			}
		}
		else
		{
			return (false);
		}
		// printf("texturebuffer[i].height = %d\n", wallTextures[i].height);
		// printf("walltexture[i].width = %d\n", wallTextures[i].width);
		// printf("texturebuffer[i][0] = %x\n", wallTextures[i].texture_buffer[0]);
		i++;
		}  
	return (true);
}

void freeWallTextures()
{
	int i;

	i = 0;
	while (i < NUM_TEXTURES)
	{
		upng_free(wallTextures[i].upngTexture);
		i++;
	}
}