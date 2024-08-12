#include "Capp.h"

Capp::Capp()
{
	m_isRunning = true;
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
}

bool Capp::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	m_pWindow = SDL_CreateWindow("Multithreaded RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	if (m_pWindow)
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	else
		return false;

	return true;
}

int Capp::OnExecute()
{
	SDL_Event event;
	if (!OnInit())
		return -1;

	while (m_isRunning) 
	{
		while (SDL_PollEvent(&event) != 0) 
		{
			OnEvent(&event);
		}

		OnLoop();
		OnRender();
	}

	return 0; //dummy temporary return value
}

void Capp::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_QUIT)
	{
		m_isRunning = false;
	}
}

void Capp::OnLoop()
{

}

void Capp::OnRender()
{	
	// Set background color to white
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(m_pRenderer);

	// Display the result
	SDL_RenderPresent(m_pRenderer);
}

void Capp::OnExit()
{
	// TIdy up SDL2 stuff
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

