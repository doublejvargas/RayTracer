#pragma once

#include "Scene.hpp"

namespace rt
{
	class ReflectionScene : public Scene
	{
	public:
		// Default constructor
		ReflectionScene();

		// Destructor
		virtual ~ReflectionScene() override;

		// Set up scene
		virtual void SetupSceneObjects() override;
	};
}
