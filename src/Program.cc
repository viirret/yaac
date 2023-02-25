#include "Program.hh"
#include "Button.hh"
#include "Renderer.hh"

#include <SDL2/SDL_mixer.h>

#define wsize window.getSize()
#define bsize 50

Program::Program(int argc, char** argv) 
	: 	argc(argc), argv(argv), window(""),
		mainFont(TTF_OpenFont("../assets/font.ttf", 24)),
		clock(mainFont, wsize),
		bgColor(255, 200, 244, 255)
{
	buttons.emplace_back
	(
		// position and size for the button
		SDL_Rect{ wsize.x / 2 - wsize.x / 8, wsize.y / 2 - wsize.y / 6, bsize, bsize }, 

		// font and text
		mainFont, "+", 

		// what happens when clicked
		[this](Button& button)
		{
			button.isPressed = true;
			clock.addHour();
		}
	);

	buttons.emplace_back
	(
		SDL_Rect{ wsize.x / 2 - wsize.x / 8, wsize.y / 2, bsize, bsize },
		mainFont, "-",
		[this](Button& button)
		{
			button.isPressed = true;
			clock.removeHour();
		}
	);

	buttons.emplace_back
	(
		SDL_Rect{ wsize.x / 2 + wsize.x / 8 - bsize, wsize.y / 2 - wsize.y / 6, bsize, bsize },
		mainFont, "+",
		[this](Button& button)
		{
			button.isPressed = true;
			clock.addMinute();
		}
	);

	buttons.emplace_back
	(
		SDL_Rect{ wsize.x / 2 + wsize.x / 8 - bsize, wsize.y / 2, bsize, bsize },
		mainFont, "-",
		[this](Button& button)
		{
			button.isPressed = true;
			clock.removeMinute();
		}
	);

	// create button that can test sounds
	buttons.emplace_back
	(
		SDL_Rect{ wsize.x - wsize.x / 6, wsize.y / 8, 200, bsize },
		mainFont, "Test sounds",
		[](Button& button)
		{
			button.isPressed = true;

			// play clicksound
			Mix_Music* click = Mix_LoadMUS("../assets/click.mp3");

			if(!click)
			{
				std::cout << "Cannot find clicksound " << Mix_GetError() << std::endl;
			}

			if(Mix_PlayMusic(click, 0) == -1)
			{
				std::cout << "Cannot play clicksound " << Mix_GetError() << std::endl;
			}
		}
	);

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

		// mouse is pressed
		else if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) 
		{
			// get click position
            int mouseX = e.button.x;
            int mouseY = e.button.y;

			// if any of the buttons is clicked
			for(auto& b : buttons)
			{
				if(mouseX >= b.rect.x && mouseX <= b.rect.x + b.rect.w && mouseY >= b.rect.y && mouseY <= b.rect.y + b.rect.h)
				{
					if(b.click)
						b.click(b);
				}
			}
        }
	}
}

void Program::render()
{
	// render background
	Renderer::setColor(bgColor);
	Renderer::clear();

	clock.render();

	// render buttons
	for(auto& button : buttons) 
		button.render();

	// main SDL rendering
	Renderer::render();	
}


