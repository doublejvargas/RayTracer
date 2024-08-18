#pragma once

#include "rtImage.hpp"
#include "Camera.hpp"
#include "objsphere.hpp"

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
		rt::ObjSphere m_Sphere;
	};
}