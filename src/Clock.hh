#ifndef YAAC_CLOCK_HH
#define YAAC_CLOCK_HH

#include "Vector2.hh"
#include "Color.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <string>

enum class ClockState
{
	NOT_SET,
	RUNNING_TIMER,
	RINGING,
	SHOW_CLOCK
};

class Clock
{
public:
	Clock(TTF_Font* font, Vec2i screenSize, Color* bgColor);
	~Clock();

	// current state of this clock
	ClockState state;

	void render();
	
	// clock modifier functions
	void addHour();
	void removeHour();
	void addMinute();
	void removeMinute();

	// start timer
	void startTimer();

	// update timer
	void timerLoop();

	// update main texture
	void updateTexture();

	// update normal mode
	void setClockToCurrentTime();

	// store selected file path
	const char* filePath = nullptr;
	bool fileSelected = false;

	// sound that timer makes when done
	Mix_Music* sound;

private:
	TTF_Font* font;

	// texture for the main clock
	SDL_Texture* texture;
	
	// how much time is left until timer is done
	SDL_Texture* timeLeftTex;

	// rect for the clock and timer
	SDL_Rect rect, timeLeftRect;

	// clock color (2 textures)
	Color clockColor;

	// backgroundColor in Program
	Color* bgColor;

	// how much time is left
	std::chrono::duration<double> timeLeft;
	std::chrono::hours::rep hoursLeft;
	std::chrono::minutes::rep minutesLeft;
	std::chrono::seconds::rep secondsLeft;
	
	// main texture:
	// values
	int hours = 0;
	int minutes = 0;
	int seconds = 0;

	// timer texture:
	std::string timeToText();
	std::string createTimeString();
	void updateClockValues();
};

#endif
