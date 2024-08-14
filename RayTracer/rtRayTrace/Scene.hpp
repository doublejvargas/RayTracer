#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "rtImage.hpp"

namespace rt
{
	class Scene
	{
	public:
		// Default constructor
		Scene();

		// Function to perform rendering
		bool Render(rtImage &outputImage);

	private:
		// Private methods

	private:
		// Private members
	};
}