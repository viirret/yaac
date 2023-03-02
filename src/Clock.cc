#include "Clock.hh"
#include "Renderer.hh"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mixer.h>
#include <chrono>
#include <sstream>
#include <string>
#include <ctime>

Clock::Clock(TTF_Font* font, Vec2i screenSize, Color* bgColor)
	: 	state(ClockState::NOT_SET),
		font(font), bgColor(bgColor),
		clockColor(122, 122, 122, 122)
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

	if(state == ClockState::RINGING)
		ss << ":" << seconds;

	return ss.str();
}

void Clock::updateTexture()
{
	// create surface
	SDL_Surface* surface = TTF_RenderText_Solid(font, timeToText().c_str(), clockColor.getSDLColor());

	// update texture
	texture = SDL_CreateTextureFromSurface(Renderer::get(), surface);
}

void Clock::setClockToCurrentTime()
{
}

void Clock::startTimer()
{
	state = ClockState::RUNNING_TIMER;

	// create sound
	sound = Mix_LoadMUS(filePath);

	if(!sound)
	{
		SDL_Log("CANNOT FIND SOUND %s", Mix_GetError());

		// LOAD CONFIG HERE
		
		SDL_Log("CANNOT FIND CONFIG %s", Mix_GetError());

		SDL_Log("USING DEFAULT SOUND");
		sound = Mix_LoadMUS("../assets/default.mp3");
		if(!sound)
		{
			SDL_Log("NO SOUND!!! %s %s", Mix_GetError(), SDL_GetError());
		}
	}

	// get current time in the local timezone
	auto now = std::chrono::system_clock::now();
	auto nowLocal = std::chrono::system_clock::to_time_t(now);
	auto nowLocalTime = std::localtime(&nowLocal);

	// start to construct a new time point from the current time
	std::tm targetTime = *nowLocalTime;

	// create new timepoint
	targetTime.tm_hour = hours;
	targetTime.tm_min = minutes;
	targetTime.tm_sec = seconds;

	// get chrono::time_point
	auto targetTimeT = std::mktime(&targetTime);
	auto targetTimePoint = std::chrono::system_clock::from_time_t(targetTimeT);

	// calculate the time difference between the current time and the target time
	timeLeft = targetTimePoint - now;

	// target is smaller than time (target is next day)
	if(timeLeft <= std::chrono::seconds(0))
		timeLeft += std::chrono::hours(24);

	// update variables that tell how much time is remaining
	updateClockValues();

	// create surface
	SDL_Surface* surface = TTF_RenderText_Solid(font, createTimeString().c_str(), clockColor.getSDLColor());

	// update texture that tells how much time is left
	timeLeftTex = SDL_CreateTextureFromSurface(Renderer::get(), surface);
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

		// update time
		timeLeft -= std::chrono::seconds(1);
		updateClockValues();

		// update timer
		SDL_Surface* surface = TTF_RenderText_Solid(font, createTimeString().c_str(), clockColor.getSDLColor());
		timeLeftTex = SDL_CreateTextureFromSurface(Renderer::get(), surface);
   	}

	// timer has been reached
	if(std::chrono::system_clock::now() >= std::chrono::system_clock::now() + timeLeft)
	{
		state = ClockState::RINGING;

    	// hide the time left texture
		timeLeftTex = nullptr;

		// make the backgroundColor white
		bgColor->white();

		// change color of the clock numbers
		clockColor.black();

		// play sound after timer is finished
    	if(Mix_PlayMusic(sound, 0) == -1) 
		{
			SDL_Log("Cannot play clicksound %s", Mix_GetError());
    	}
	}
}

void Clock::updateClockValues()
{
    hoursLeft = std::chrono::duration_cast<std::chrono::hours>(timeLeft).count();
    minutesLeft = std::chrono::duration_cast<std::chrono::minutes>(timeLeft).count() % 60;
    secondsLeft = std::chrono::duration_cast<std::chrono::seconds>(timeLeft).count() % 60;
}

// oddly similar to function timeToText
// could be changed but three parameters might make this more confusing
std::string Clock::createTimeString()
{
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

	return ss.str();
}

void Clock::addHour()
{
	if(state != ClockState::NOT_SET)
		return;

	if(hours >= 23)
		hours = 0;
	else 
		hours++;
	
	updateTexture();
}

void Clock::removeHour()
{
	if(state != ClockState::NOT_SET)
		return;

	if(hours <= 0)
		hours = 23;
	else
		hours--;

	updateTexture();
}

void Clock::addMinute()
{	
	if(state != ClockState::NOT_SET)
		return;

	if(minutes >= 59)
		minutes = 0;
	else
		minutes++;

	updateTexture();
}

void Clock::removeMinute()
{	
	if(state != ClockState::NOT_SET)
		return;

	if(minutes <= 0)
		minutes = 59;
	else
	 	minutes--;
	
	updateTexture();
}

