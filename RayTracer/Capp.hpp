#pragma once

#include <SDL2/SDL.h>
#include "rtRayTrace/rtImage.hpp"
#include "rtRayTrace/Scene.hpp"
#include"rtRayTrace/Camera.hpp"

#include "toolbox/qbVector.h"


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

private:
	// For debugging
	void PrintVector(const qbVector<double> &inputVector) const;

private:
	// Image instance
	rtImage m_Image;
	// Scene instance
	rt::Scene m_Scene;
	// SDL2 stuff
	bool m_isRunning;
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;

};
