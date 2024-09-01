#pragma once

#include "rtImage.hpp"
#include "Camera.hpp"
#include "rtPrimitives/objsphere.hpp"
#include "rtPrimitives/objplane.hpp"
#include "rtLights/pointlight.hpp"
#include "rtUtils.hpp"

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

		// Destructor
		virtual ~Scene();

		// Function to perform rendering in normal rendering mode
		bool Render(rtImage &outputImage);

		// Function to render a tile (used for tile-based rendering)
		void RenderTile(rt::DATA::tile *tile);

		// Function to cast a ray into the scene
		bool CastRay(rt::Ray &castRay,
			std::shared_ptr<rt::ObjectBase> &closestObject,
			qbVector3<double> &closestIntPoint,
			qbVector3<double> &closestLocalNormal,
			qbVector3<double> &closestLocalColor
		);

		virtual void SetupSceneObjects();

	// Private methods
	private:
		// Render a pixel
		qbVector3<double> RenderPixel(int x, int y, int width, int height);

		// Function to convert coordinates to a linear index
		int Sub2Ind(int x, int y, int width, int height);


	// Public members
	public:
		// The camera that the scene uses
		rt::Camera camera_;
		// A list of objects in the scene
		std::vector<std::shared_ptr<rt::ObjectBase>> objectList_;
		// A list of of lights in the scene
		std::vector<std::shared_ptr<rt::LightBase>> lightList_;

		// Scene parameters
		int width_, height_;
	};
}