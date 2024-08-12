#pragma once

#include <SDL2/SDL.h>
#include "rtRayTrace/rtImage.hpp"

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
	// Image instance
	rtImage m_Image;

	// SDL2 stuff
	bool m_isRunning;
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;

};
