#pragma once

#include <SDL2/SDL.h>
#include "rtImage.hpp"
#include "ReflectionScene.hpp"
#include"Camera.hpp"
#include "rtUtils.hpp"

#include "toolbox/qbVector.h"


class Capp
{
public:
	Capp();

	bool OnInit();
	int OnExecute();
	void OnEvent(SDL_Event *event);
	void OnLoop();
	void OnRender();
	void OnExit();

private:
	// For debugging
	void PrintVector(const qbVector3<double> &inputVector) const;

	/* New functions to handle tile-based rendering. This isn't much use 
	*  right now, but forms the basis for the multi-threading implementation that we will look at later */

	// Generate the tile grid
	bool GenerateTileGrid(int tileWidth, int tileHeight);

	// Handle destroying the tile grid
	bool DestroyTileGrid();

	// Convert tile image to texture
	void ConvertImageToTexture(rt::DATA::tile &tile); // might be obsolete

	// Handle converting colors from RGB to UINT32
	uint32_t ConvertColor(const double red, const double green, const double blue);

private:
	// Instance of the rtImage class to store the output image
	rtImage m_Image;

	// Display configuration
	int m_Width, m_Height;

	// value to be used for gamma-correction
	double m_maxLevel = 0.8;

	// Array to store tiles
	std::vector <rt::DATA::tile> m_Tiles;
	std::vector<int> m_tileFlags;
	int m_numTilesX, m_numTilesY;

	// Scene instance
	rt::ReflectionScene m_Scene;
	
	// SDL2 stuff
	bool m_isRunning;
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;
};
