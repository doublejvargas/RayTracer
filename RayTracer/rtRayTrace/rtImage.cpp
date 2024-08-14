#include "rtImage.hpp"

// why not call init here w params?
rtImage::rtImage() :
	m_Width{0}, m_Height{0}, m_pRenderer{nullptr}, m_pTexture{nullptr} { }

rtImage::~rtImage()
{
	// Check that texture exists before deleting to avoid segmentation fault/error
	if (m_pTexture)
		SDL_DestroyTexture(m_pTexture);
}

// why not pass these parameters to the constructor and then call initialize from the constructor?
void rtImage::Initialize(const int width, const int height, SDL_Renderer *pRenderer)
{
	// Resize image arrays
	m_rChannel.resize(width, std::vector<double>(height, 0.0));
	m_gChannel.resize(width, std::vector<double>(height, 0.0));
	m_bChannel.resize(width, std::vector<double>(height, 0.0));

	// Store the dimensions
	m_Width = width;
	m_Height = height;

	// Store the pointer to the renderer
	m_pRenderer = pRenderer;

	// Initialize the texture
	InitTexture();
}

void rtImage::SetPixel(const int x, const int y, const double r, const double g, const double b)
{
	m_rChannel.at(x).at(y) = r;
	m_gChannel.at(x).at(y) = g;
	m_bChannel.at(x).at(y) = b;
}

void rtImage::Display()
{
	// ALlocate memory for a pixel buffer (why not use malloc/calloc?)
	//uint32_t *tempPixels = new uint32_t[(size_t)Width * Height];

	// Clear the pixel buffer
	//memset(tempPixels, 0, (size_t)Width * Height * sizeof(uint32_t));

	// Allocate memory for a pixel buffer (my version)
	uint32_t *tempPixels = (uint32_t *)calloc((size_t)m_Width * m_Height, sizeof(uint32_t));  // DON'T forget to free!

	// Set pixel colors inside pixel buffer
	for (int x = 0; x < m_Width; x++) {
		for (int y = 0; y < m_Height; y++) {
			if(tempPixels)
				tempPixels[(y * m_Width) + x] = ConvertColor(m_rChannel.at(x).at(y), m_gChannel.at(x).at(y), m_bChannel.at(x).at(y));
		}
	}

	 // Update the texture with the pixel buffer
	SDL_UpdateTexture(m_pTexture, nullptr, tempPixels, m_Width * sizeof(uint32_t));

	// Free the pixel buffer
	//delete[] tempPixels;
	free(tempPixels);

	// Copy the texture to the renderer
	SDL_Rect srcRect, bounds;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = m_Width;
	srcRect.h = m_Height;
	bounds = srcRect;
	SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

uint32_t rtImage::ConvertColor(const double red, const double green, const double blue)
{
	// Convert colors to unsigned integers
	unsigned char r = static_cast<unsigned char>(red);
	unsigned char g = static_cast<unsigned char>(green);
	unsigned char b = static_cast<unsigned char>(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	uint32_t pixelColor = (r << 24) + (g << 16) + (b << 8) + 255;
#else
	uint32_t pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

	return pixelColor;
}

void rtImage::InitTexture()
{
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

	// Delete any previously created texture
	if (m_pTexture)
		SDL_DestroyTexture(m_pTexture);

	// Create the texture that will store the image
	SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, rmask, gmask, bmask, amask);
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

