#include "Scene.hpp"

rt::Scene::Scene()
{

}

bool rt::Scene::Render(rtImage &outputImage)
{
	// Get the dimensions of the output image
	int width = outputImage.GetWidth();
	int height = outputImage.GetHeight();

	// Compute color variations
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double red = (static_cast<double>(x) / static_cast<double>(width)) * 255.0; // the ratio results in a number between 0-1, then multiply by 255 to obtain a color
			double green = (static_cast<double>(y) / static_cast<double>(height)) * 255.0; // the ratio results in a number between 0-1, then multiply by 255 to obtain a color
			outputImage.SetPixel(x, y, red, green, 0.0);
		}
	}

	return true;
}

