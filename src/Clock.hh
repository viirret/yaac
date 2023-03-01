#ifndef YAAC_CLOCK_HH
#define YAAC_CLOCK_HH

#include "Vector2.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <string>

class Clock
{
public:
	Clock(TTF_Font* font, Vec2i screenSize);
	~Clock();
	void render();
	
	// clock modifier functions
	void addHour();
	void removeHour();
	void addMinute();
	void removeMinute();

	// start timer
	void startTimer();

	// update timer texture
	void timerLoop();

	bool timerOn = false;

private:
	TTF_Font* font;

	// texture for the main clock
	SDL_Texture* texture;
	
	// how much time is left until timer is done
	SDL_Texture* timeLeftTex = nullptr;

	// rect for the clock and timer
	SDL_Rect rect, timeLeftRect;

	// sound that timer makes when done
	Mix_Music* sound;

	// clock text is white, for now atleast
	SDL_Color white = { 255, 255, 255, 255 };

	std::chrono::hours::rep hoursLeft;
	std::chrono::minutes::rep minutesLeft;
	std::chrono::seconds::rep secondsLeft;

	std::chrono::duration<double> timeLeft;

	std::string timeToText();
	void updateTexture();
	void updateTime();
	std::string createTimeString();

	int hours = 0;
	int minutes = 0;
};

#endif
