#include "Clock.hh"
#include "Renderer.hh"

#include <chrono>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>

Clock::Clock(TTF_Font* font, Vec2i screenSize)
	: font(font)
{
	// define clock size and position
	rect.w = 200;
	rect.h = 200;
	rect.x = screenSize.x / 2 - rect.w / 2;
	rect.y = screenSize.y / 2 - rect.h * 2;
	
	// same for texture that tells how much time is left
	timeLeftRect.w = 200;
	timeLeftRect.h = 50;
	timeLeftRect.x = screenSize.x / 2 - timeLeftRect.w / 2;
	timeLeftRect.y = screenSize.y / 2 - timeLeftRect.h * 2;

	updateTexture();
}

Clock::~Clock()
{
	if(texture)
		SDL_DestroyTexture(texture);
	if(timeLeftTex)
		SDL_DestroyTexture(timeLeftTex);
}

void Clock::render()
{
	SDL_RenderCopy(Renderer::get(), texture, nullptr, &rect);

	if(timeLeftTex)
		SDL_RenderCopy(Renderer::get(), timeLeftTex, nullptr, &timeLeftRect);
}

std::string Clock::timeToText()
{
	std::stringstream ss;	

	// convert hours
	if(hours < 10)
		ss << "0" << hours;
	else
		ss << hours;

	// add separator
	ss << ":";

	// convert minutes
	if(minutes < 10)
		ss << "0" << minutes;
	else
		ss << minutes;

	return ss.str();
}

void Clock::updateTexture()
{
	// create surface
	SDL_Surface* surface = TTF_RenderText_Solid(font, timeToText().c_str(), white);

	// update texture
	texture = SDL_CreateTextureFromSurface(Renderer::get(), surface);
}

void Clock::startTimer()
{
	// create sound (this will be replaced with file picker)
	sound = Mix_LoadMUS("../assets/click.mp3");

	if(!sound)
	{
		std::cout << "Cannot find clicksound " << Mix_GetError() << std::endl;
	}

	// get current time in the local timezone
	auto now = std::chrono::system_clock::now();
    auto nowLocal = std::chrono::system_clock::to_time_t(now);
    auto nowLocalTime = std::localtime(&nowLocal);

    // Construct a new time point for the target time of day
    std::tm targetTime = *nowLocalTime;
    targetTime.tm_hour = hours;
    targetTime.tm_min = minutes;
    targetTime.tm_sec = 0;
	auto targetTimeT = std::mktime(&targetTime);
    auto targetTimePoint = std::chrono::system_clock::from_time_t(targetTimeT);

    // calculate the time difference between the current time and the target time
	auto timeLeft = targetTimePoint - now;

	// target is smaller than time (target is next day)
	if(timeLeft <= std::chrono::seconds(0))
		timeLeft += std::chrono::hours(24);

	// convert the time difference to hours, minutes, and seconds
    hoursLeft = std::chrono::duration_cast<std::chrono::hours>(timeLeft).count();
    minutesLeft = std::chrono::duration_cast<std::chrono::minutes>(timeLeft).count() % 60;
    secondsLeft = std::chrono::duration_cast<std::chrono::seconds>(timeLeft).count() % 60;

	// create surface
	SDL_Surface* surface = TTF_RenderText_Solid(font, "", white);

	// update texture that tells how much time is left
	timeLeftTex = SDL_CreateTextureFromSurface(Renderer::get(), surface);

	// do we update timer from main program
	timerOn = true;
}

void Clock::timerLoop()
{
    static auto lastPrintTime = std::chrono::system_clock::now();

   	// Check if one second has elapsed since last message
   	auto now = std::chrono::system_clock::now();
   	auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastPrintTime).count();
   	if(elapsedSeconds >= 1)
	{
       	lastPrintTime = now;

		secondsLeft -= 1;

		// TODO update all "left" variables

		std::stringstream ss;

		ss << hoursLeft << ":";

		if(minutesLeft < 10)
			ss << "0" << minutesLeft; 
		else
		 	ss << minutesLeft;

		ss << ":";

		if(secondsLeft < 10)
			ss << "0" << secondsLeft;
		else
			ss << secondsLeft;

		// update timer
		SDL_Surface* surface = TTF_RenderText_Solid(font, ss.str().c_str(), white);
		timeLeftTex = SDL_CreateTextureFromSurface(Renderer::get(), surface);
   	}

	/*
	 * TODO update if sentence
	// timer has been reached
	if(std::chrono::system_clock::now() >= targetTime)
	{
		// play sound after timer is finished
    	if(Mix_PlayMusic(sound, 0) == -1) 
		{
        	std::cout << "Cannot play clicksound " << Mix_GetError() << std::endl;
    	}

    	// hide the time left texture
		timeLeftTex = nullptr;

		// stop executing this loop
		timerOn = false;
	}
	*/
}

void Clock::addHour()
{
	if(hours >= 23)
		hours = 0;
	else 
		hours++;
	
	updateTexture();
}

void Clock::removeHour()
{
	if(hours <= 0)
		hours = 23;
	else
		hours--;

	updateTexture();
}

void Clock::addMinute()
{
	if(minutes >= 59)
		minutes = 0;
	else
		minutes++;
	
	updateTexture();
}

void Clock::removeMinute()
{
	if(minutes <= 0)
		minutes = 59;
	else
	 	minutes--;
	
	updateTexture();
}

