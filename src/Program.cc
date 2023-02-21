#include "Program.hh"
#include <ctime>

Program::Program(int argc, char** argv) 
	: argc(argc), argv(argv), window(""), 
	mainFont(TTF_OpenFont("../assets/font.ttf", 24)),
	clock(mainFont),
	bgColor(255, 200, 244, 255)
{
	while(!close)
	{
		update();
	}
}

void Program::update()
{
	eventHandler();
	render();
}

void Program::eventHandler()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || e.type == SDL_QUIT)
			close = true;
	}
}

void Program::render()
{
	// render background
	Renderer::setColor(bgColor);
	Renderer::clear();

	clock.render();

	// main SDL rendering
	Renderer::render();	
}


