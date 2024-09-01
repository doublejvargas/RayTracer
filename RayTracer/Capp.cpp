#include "Capp.hpp"


Capp::Capp()
{
	m_isRunning = true;
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
}

bool Capp::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	m_Width = 1280;
	m_Height = 720;

	m_pWindow = SDL_CreateWindow("Multi threaded RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_SHOWN);
	if (m_pWindow)
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

		// Initialize the rtImage instance
		//m_Image.Initialize(1280, 720, m_pRenderer);

		/* Instead of initializing the image, we now initialize the scene with
		   the window dimensions and then generate a tile grid with tiles of the specified size */
		m_Scene.width_ = m_Width;
		m_Scene.height_ = m_Height;

		// Initialize the tile grid
		if (!GenerateTileGrid(128, 90))
		{
			std::cout << "Failed to generate tile grid" << std::endl;
			return false;
		}

		// Set background to white
		SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(m_pRenderer);

		/* Following the introduction of tile-based rendering, we no longer need to render the image here */
		// Render the scene
		//m_Scene.Render(m_Image);

		// Display the image
		//m_Image.Display();

		// Present the result
		SDL_RenderPresent(m_pRenderer);
	}
	else
		return false;

	return true;
}

int Capp::OnExecute()
{
	SDL_Event event;
	if (!OnInit())
		return -1;

	while (m_isRunning) 
	{
		while (SDL_PollEvent(&event) != 0) 
		{
			OnEvent(&event);
		}

		OnLoop();
		OnRender();
	}

	/* The delay here seems counterintuitive, but it actually makes the rendering faster
	   (with multi-threading). The reason for this, I think, is that it reduces the load
	   imposed by looping through this code allowing more of the CPU time to be used for
	   actual rendering. I have tried different values, reducing it below 1 seems to slow things
	   down and increasing it above 4 also slows things down. It would seem that somewhere
	   between 1 and 4 is the 'sweet spot', so I have left it at 1
	*/
	SDL_Delay(1);

	OnExit();
	return 0; //dummy temporary return value
}

void Capp::OnEvent(SDL_Event *event)
{
	if (event->type == SDL_QUIT)
	{
		m_isRunning = false;
	}
}

void Capp::OnLoop()
{
	/*
		The actual rendering is now handled here, each time we come through
		this loop. At the moment, before implementing multi-threading, we
		proceed by looping through all of the tiles and finding the first
		one that has not been rendered yet (as indicated by m_tileFlags).
		We then call the RenderTile function from the scene base class and
		pass a reference to the relevant tile as the only parameter. Finally,
		we update m_tileFlags for this tile to indicate that it has now been
		rendered and then break out of the loop. We don't want to end up
		rendering every tile all at once, we want to one tile each time
		we come through the OnLoop() function.
	*/

	
	// Loop through all tiles and find the first one that hasn't been rendered yet
	for (int i = 0; i < m_Tiles.size(); i++)
	{
		if (m_tileFlags.at(i) == 0)
		{
			// This tile has not been rendered, so render it now
			m_Scene.RenderTile(&m_Tiles.at(i));

			// Set the tile flag to indicate that this tile has been rendered
			m_tileFlags.at(i) = 2;

			// Once complete, break out of the loop
			// This is temporary and will be removed once we implement multi-threading
			break;
		}
	}
	
}

void Capp::OnRender()
{
	
	// Conversion factors from screen width/height to window width/height
	double widthFactor = 1.0;
	double heightFactor = 1.0;

	/*
		The actual display is now generated here. We loop over all of the tiles
		and wherever we find one that has been rendered (tileFlags == 2), then
		we check if we have already display this tile (textureComplete) and if
		not we convert the image data to an SDL texture and then use SDL_RenderCopy
		to copy that texture to the right place in the window.

		In a single-thread implementation, this approach doesn't necessarily make
		much sense, but it forms the foundation on which we can build our multi-threaded
		version.
	*/

	//

	// Render the tiles
	for (int i = 0; i < m_Tiles.size(); i++)
	{
		if (m_tileFlags.at(i) == 2)
		{
			SDL_Rect srcRect, dstRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = m_Tiles.at(i).width;
			srcRect.h = m_Tiles.at(i).height;
			dstRect.x = static_cast<int>(std::round(static_cast<double>(m_Tiles.at(i).x) * widthFactor));
			dstRect.y = static_cast<int>(std::round(static_cast<double>(m_Tiles.at(i).y) * heightFactor));
			dstRect.w = static_cast<int>(std::round(static_cast<double>(m_Tiles.at(i).width) * widthFactor));
			dstRect.h = static_cast<int>(std::round(static_cast<double>(m_Tiles.at(i).height) * heightFactor));

			/* If the textureComplete flag for this tile is not set, then it means that the tile has been
			* rendered, but not yet converted to a texture. So we do that here and ten set the textureComplete
			* flag and blit the texture into the renderer. Note that once this is done, we don't do this again
			* for this tile meaning that we don't keep updating each tile very time we go through this loop.
			* This helps to keep things as efficient as possible.
			*/ 

			//
			if (!m_Tiles.at(i).textureComplete)
			{
				ConvertImageToTexture(m_Tiles.at(i));
				m_Tiles.at(i).textureComplete = true;
				// might need to do something like this here since I haven't implemented textures
				// SDL_UpdateTexture(m_pTexture, nullptr, tempPixels, m_Width * sizeof(uint32_t));
				SDL_RenderCopy(m_pRenderer, m_Tiles.at(i).pTexture, &srcRect, &dstRect);
			}
		}
	}
	
	// Show the result
	SDL_RenderPresent(m_pRenderer);
}

void Capp::OnExit()
{
	// Tidy up the tile grid
	bool result = DestroyTileGrid();

	// TIdy up SDL2 stuff
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

// PRIVATE FUNCTIONS		
void Capp::PrintVector(const qbVector3<double> &inputVector) const
{
	int rows = inputVector.GetNumDims();
	for (int r = 0; r < rows; r++) 
	{
		std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(r) << std::endl;
	}
}

bool Capp::GenerateTileGrid(int tileWidth, int tileHeight)
{
	// How many tiles will fit horizontally?
	int numTilesX = static_cast<int>(std::floor(m_Width / tileWidth));
	m_numTilesX = numTilesX;

	// How many tiles will fit vertically?
	int numTilesY = static_cast<int>(std::floor(m_Height / tileHeight));
	m_numTilesY = numTilesY;

	// Setup an SDL surface from which we can generate the textures for each tile
	uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, rmask, gmask, bmask, amask);

	// Generate the actual tiles
	for (int y = 0; y < numTilesY; y++)
	{
		for (int x = 0; x < numTilesX; x++)
		{
			rt::DATA::tile tempTile{};
			tempTile.x		=	x * tileWidth;
			tempTile.y		=	y * tileHeight;
			tempTile.width	=	tileWidth;
			tempTile.height =	tileHeight;
			tempTile.renderComplete = 0;
			tempTile.pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
			tempTile.rgbData.resize(tempTile.width * tempTile.height);
			m_Tiles.push_back(tempTile);
		}
	}

	// Set all the tile flags to zero
	for (int i = 0; i < m_Tiles.size(); i++)
	{
		m_tileFlags.push_back(0);
	}
	
	// Tidy up before returning
	SDL_FreeSurface(tempSurface);
	return true;
}

bool Capp::DestroyTileGrid()
{
	for (int i = 0; i < m_Tiles.size(); i++)
	{
		if (m_Tiles.at(i).pTexture != nullptr)
			SDL_DestroyTexture(m_Tiles.at(i).pTexture);
	}

	return true;
}

void Capp::ConvertImageToTexture(rt::DATA::tile &tile)
{
	// Alternative way to allocate buffer
	uint32_t *tempPixels = (uint32_t *)calloc((size_t)m_Width * m_Height, sizeof(uint32_t));  // DON'T forget to free!

	// Allocate memory for a pixel buffer
	//uint32_t *tempPixels = new uint32_t[tile.width * tile.height];

	// Clear the pixel buffer
	//memset(tempPixels, 0, tile.width * tile.height * sizeof(uint32_t));

	// Copy the image into tempPixels
	for (int i = 0; i < tile.width * tile.height; i++)
	{
		if(tempPixels)
			tempPixels[i] = ConvertColor(tile.rgbData.at(i).m_x, tile.rgbData.at(i).m_y, tile.rgbData.at(i).m_z);
	}

	// Update the texture with the pixel buffer
	SDL_UpdateTexture(tile.pTexture, nullptr, tempPixels, tile.width * sizeof(uint32_t));

	// Delete the pixel buffer
	//delete[] tempPixels;

	// Alternate way to delete
	free(tempPixels);
}

uint32_t Capp::ConvertColor(const double red, const double green, const double blue)
{
	// Convert the colors to unsigned integers
	double newRed = std::max(std::min(std::pow(red, m_maxLevel), 1.0), 0.0);
	double newGreen = std::max(std::min(std::pow(green, m_maxLevel), 1.0), 0.0);
	double newBlue = std::max(std::min(std::pow(blue, m_maxLevel), 1.0), 0.0);

	unsigned char r = static_cast<unsigned char>(newRed * 255.0);
	unsigned char g = static_cast<unsigned char>(newGreen * 255.0);
	unsigned char b = static_cast<unsigned char>(newBlue * 255.0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	uint32_t pixelColor = (r << 24) + (g << 16) + (b << 8) + 255;
#else
	uint32_t pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

	return pixelColor;
}

