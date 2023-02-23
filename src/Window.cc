#include "Window.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

Window::Window(const std::string& title)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		return;
	}

	std::cout << "SDL Initialized" << std::endl;

	// get monitor size
	SDL_DisplayMode display;
	if(SDL_GetCurrentDisplayMode(0, &display))
	{
		std::cout << "Couldn't get display info: " << SDL_GetError() << std::endl;
		return;
	}

	// create window	
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_RESIZABLE);
	if(!window)
	{
		std::cout << "Could't create window: " << std::endl;
		return;
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(!renderer)
	{
		std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return;
	}
	else 
	{
		Renderer::set(renderer);
	}

	// update window size
	SDL_GetWindowSize(window, &size.x, &size.y); 
	std::cout << "Window created!" << std::endl;

	// initialize other SDL stuff
	TTF_Init();

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Unable to open audio " << Mix_GetError() << std::endl;
		return;
	}
}

Window::~Window()
{
	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

Vec2i Window::swapSize(Vec2i newSize)
{
	Vec2i oldSize = size;
	size = newSize;
	return oldSize;
}
