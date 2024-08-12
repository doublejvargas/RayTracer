#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class rtImage
{
public:
	// Constructor
	rtImage();

	// Destructor
	~rtImage();

	// Initialize
	void Initialize(const int xSize, const int ySize, SDL_Renderer* pREnderer);

	// Set the color of a pixel
	void SetPixel(const int x, const int y, const double r, const double g, const double b);

	// Return image for display
	void Display();

private:
	uint32_t ConvertColor(const double red, const double green, const double blue);
	void InitTexture();

private:
	// 2D arrays to store image data
	std::vector<std::vector<double>> m_rChannel;
	std::vector<std::vector<double>> m_gChannel;
	std::vector<std::vector<double>> m_bChannel;

	// Store dimensions of the image
	int m_xSize, m_ySize;

	// SDL2 stuff
	SDL_Renderer *m_pRenderer;
	SDL_Texture *m_pTexture;
};