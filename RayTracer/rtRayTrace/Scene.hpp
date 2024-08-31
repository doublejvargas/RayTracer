#pragma once

#include "rtImage.hpp"
#include "Camera.hpp"
#include "rtPrimitives/objsphere.hpp"
#include "rtPrimitives/objplane.hpp"
#include "rtLights/pointlight.hpp"

// lib
#include <vector>
#include <SDL2/SDL.h>

namespace rt
{
	class Scene
	{
	public:
		// Default constructor
		Scene();

		// Function to perform rendering
		bool Render(rtImage &outputImage);
	
	// Private methods
	private:

	// Private members
	private:
		rt::Camera m_Camera;
		// A list of objects in the scene
		std::vector<std::shared_ptr<rt::ObjectBase>> m_objectList;
		// A list of of lights in the scene
		std::vector<std::shared_ptr<rt::LightBase>> m_lightList;
	};
}