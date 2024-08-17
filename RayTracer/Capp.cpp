#include "Capp.hpp"


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
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

		// Initialize the rtImage instance
		m_Image.Initialize(1280, 720, m_pRenderer);

		// Test the camera class
		rt::Camera testCamera;
		testCamera.SetPosition(qbVector<double>(std::vector<double>{0.0, 0.0, 0.0}));
		testCamera.SetLookAt(qbVector<double>(std::vector<double>{0.0, 2.0, 0.0}));  // Camera space Y axis is into/out of screen
		testCamera.SetUp(qbVector<double>(std::vector<double>{0.0, 0.0, 1.0}));
		testCamera.SetLength(1.0);
		testCamera.SetProjScreenWidth(1.0);
		testCamera.SetAspect(1.0);
		testCamera.UpdateCameraGeometry();

		// Get screen center and and U, V vectors and display
		auto screenCenter = testCamera.GetScreenCenter();
		auto screenU = testCamera.GetU();
		auto screenV = testCamera.GetV(); 
		std::cout << "Projected screen center:" << std::endl;
		PrintVector(screenCenter);
		std::cout << "\nProjected screen U vector:" << std::endl;
		PrintVector(screenU);
		std::cout << "\nProjected screen V vector:" << std::endl;
		PrintVector(screenV);
	}
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

void Capp::OnEvent(SDL_Event *event)
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

	// Render the scene
	m_Scene.Render(m_Image);

	// Display the image
	m_Image.Display();

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

// PRIVATE FUNCTIONS		
void Capp::PrintVector(const qbVector<double> &inputVector) const
{
	int rows = inputVector.GetNumDims();
	for (int r = 0; r < rows; r++) 
	{
		std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(r) << std::endl;
	}
}

