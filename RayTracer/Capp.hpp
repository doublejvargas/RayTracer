#pragma once

#include <SDL2/SDL.h>
#include "rtImage.hpp"
#include "ReflectionScene.hpp"
#include"Camera.hpp"
#include "rtUtils.hpp"

// tools
//#include "toolbox/qbVector3.hpp"
#include "toolbox/glm/glm.hpp"

// lib
#include <thread>
#include <atomic>


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

	// Function to handle rendering a tile
	void RenderTile(rt::DATA::tile *tile, std::atomic<int> *threadCounter, std::atomic<int> *tileFlag);

private:
	// For debugging
	void PrintVector(const glm::dvec3 &inputVector) const;

	/* New functions to handle tile-based rendering. This isn't much use 
	*  right now, but forms the basis for the multi-threading implementation that we will look at later */

	// Generate the tile grid
	bool GenerateTileGrid(int tileWidth, int tileHeight);

	// Handle destroying the tile grid
	bool DestroyTileGrid();

	// Reset the tile flags
	void ResetTileFlags();

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
	// Tile flags are now instances of std::atomic<int> as they will be accessed concurrently
	std::vector<std::atomic<int> *> m_tileFlags;
	int m_numTilesX, m_numTilesY;

	// Thread stuff
	int m_maxThreads = 6;
	int m_numCurrentThreads = 0;
	std::vector<int> m_tilesCurrentlyRendering;
	std::vector<std::thread> m_Threads;
	std::atomic<int> *m_threadCounter;

	// Scene instance
	rt::ReflectionScene m_Scene;
	
	// SDL2 stuff
	bool m_isRunning;
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;
};
