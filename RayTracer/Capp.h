#pragma once

#include <SDL2/SDL.h>

class Capp
{
public:
	Capp();

	bool OnInit();
	int OnExecute();
	void OnEvent(SDL_Event* event);
	void OnLoop();
	void OnRender();
	void OnExit();

private:
	// SDL2 stuff
	bool m_isRunning;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

};
