#include "Program.hh"
#include "Button.hh"
#include "Clock.hh"
#include "Renderer.hh"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>

#include <ctime>
#include <string>

#define wsize window.getSize()
#define bsize 50

Program::Program(int argc, char** argv) 
	: 	argc(argc), argv(argv), 
		config("../config", '='),
		window(""),
		mainFont(TTF_OpenFont("../assets/font.ttf", 24)),
		bgColor(255, 200, 244, 255),
		clock(mainFont, wsize, &bgColor)
{
	// TODO assign bgColor from hex value
	std::string hex = config.get("color");
	SDL_Log("color=%s", hex.c_str());

	buttons.emplace_back
	(
		// position and size for the button
		SDL_Rect{ wsize.x / 2 - wsize.x / 8, wsize.y / 2 - wsize.y / 6, bsize, bsize }, 

		// font and text
		mainFont, "+", 

		// what happens when clicked
		[this](Button& button)
		{
			// currently flicks color, could add something like sound later if want
			button.isPressed = true;

			// first clock function
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

	buttons.emplace_back
	(
		// TODO this is not properly centered, I'll do this when I have time for styling
	 	SDL_Rect{ wsize.x / 2 - bsize * 2, wsize.y / 2 + wsize.y / 6, bsize * 4, bsize },
		mainFont, "Start alarm",

		[this](Button& button)
		{
			button.isPressed = true;
			clock.startTimer();
			bgColor.black();
		}

	);

	// create button that can test sounds
	buttons.emplace_back
	(
		SDL_Rect{ wsize.x - wsize.x / 6, wsize.y / 8, bsize * 4, bsize },
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

	// main update loop
	while(!close)
	{
		update();
	}
}

void Program::update()
{
	eventHandler();

	// update clock
	switch(clock.state)
	{
		case ClockState::RINGING: clock.setClockToCurrentTime(); break;
		case ClockState::RUNNING_TIMER: clock.timerLoop(); break;
		case ClockState::NOT_SET: break;
		case ClockState::SHOW_CLOCK: break;
	}

	clock.updateTexture();

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

		// Handle key press event
		else if (e.type == SDL_KEYDOWN) 
		{
			switch (e.key.keysym.sym) 
			{
				case SDLK_RETURN:
				{
					if(clock.state == ClockState::RINGING)
					{
						clock.state = ClockState::NOT_SET;
						SDL_Log("Return key pressed, clockstate is back to normal(NOT_SET)");
					}
				}
				break;
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


