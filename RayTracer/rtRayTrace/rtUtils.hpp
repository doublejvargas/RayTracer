#pragma once

#include "toolbox/qbVector3.hpp" // includes std::vector

// lib
#include <SDL2/SDL.h>

namespace rt
{
	namespace DATA
	{
		// Structure for handling rendering tiles
		struct tile
		{
			int x;
			int y;
			int width;
			int height;
			int renderComplete = 0;
			bool textureComplete = false; // might be obsolete
			SDL_Texture *pTexture;
			std::vector<qbVector3<double>> rgbData;
		};
	}
}