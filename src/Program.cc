#include "Program.hh"
#include "Renderer.hh"

Program::Program(int argc, char** argv)
	: argc(argc), argv(argv), bgColor(255, 200, 244, 255), window("")
{
	TTF_Init();

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
	Renderer::renderBackground(bgColor);

	// main SDL rendering
	Renderer::render();	
}


