#ifndef _TEXTURES_H_
    #define _TEXTURES_H_

    typedef struct {
		upng_t* upngTexture;
		int width;
		int height;
		uint32_t* texture_buffer;
    } texture_t;

	texture_t wallTextures[NUM_TEXTURES];

	bool loadWallTextures();
	void freeWallTextures();
#endif