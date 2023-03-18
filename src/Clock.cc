#include "Clock.hh"
#include "Color.hh"
#include "Opts.hh"
#include "Renderer.hh"
#include "Settings.hh"
#include "Util.hh"

#include <SDL2/SDL_mixer.h>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

#define clockColorDefault Color(122, 122, 122, 255)

Clock::Clock(TTF_Font* font, Vec2i screenSize, Color* bgColor, const Config& config, const Opts& opts)
    : state(ClockState::NOT_SET),
      font(font), bgColor(bgColor), originalBgColor(*bgColor),
      clockColor(Util::readHexColor(config.get("clockColor"), clockColorDefault)),
      opts(opts)
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

    // load sound from config
    if (config.get("song") != "")
    {
        sound = Util::loadMusic(config.get("song"));
    }

    if (!sound)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound from config %s", Mix_GetError());
    }
    else
    {
        SDL_Log("Using config sound %s", config.get("song").c_str());
    }

    // load wakeup time from config
    if (config.get("wakeup") != "")
    {
        std::stringstream ss(config.get("wakeup"));
        std::string token;

        std::getline(ss, token, ':');
        hours = std::stoi(token);

        std::getline(ss, token);
        minutes = std::stoi(token);
    }
    else
    {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* timeinfo = std::localtime(&currentTime);

        hours = timeinfo->tm_hour;
        minutes = timeinfo->tm_min;
    }

    updateTexture();
}

Clock::~Clock()
{
    if (texture)
        SDL_DestroyTexture(texture);
    if (timeLeftTex)
        SDL_DestroyTexture(timeLeftTex);
}

void Clock::render()
{
    if (texture)
        SDL_RenderCopy(Renderer::get(), texture, nullptr, &rect);

    if (timeLeftTex)
        SDL_RenderCopy(Renderer::get(), timeLeftTex, nullptr, &timeLeftRect);
}

void Clock::showTime()
{
    if (Mix_PlayingMusic())
    {
        // stop playing music
        Mix_HaltMusic();

        // change backgroundcolor back to normal
        bgColor->change(originalBgColor);
    }

    // update clock
    static std::time_t lastTime = 0;
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // time has changed
    if (currentTime != lastTime)
    {
        std::tm* timeinfo = std::localtime(&currentTime);

        hours = timeinfo->tm_hour;
        minutes = timeinfo->tm_min;
        seconds = timeinfo->tm_sec;

        lastTime = currentTime;
        updateTexture();
    }
}

std::string Clock::timeToText()
{
    std::stringstream ss;

    // convert hours
    if (hours < 10)
        ss << "0" << hours;
    else
        ss << hours;

    // add separator
    ss << ":";

    // convert minutes
    if (minutes < 10)
        ss << "0" << minutes;
    else
        ss << minutes;

    if (state == ClockState::SHOW_CLOCK)
        ss << ":" << seconds;

    return ss.str();
}

void Clock::updateTexture()
{
    // create surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, timeToText().c_str(), clockColor.getSDLColor());

    // update texture
    texture = SDL_CreateTextureFromSurface(Renderer::get(), surface);

    SDL_FreeSurface(surface);
}

void Clock::loopSong()
{
    if (!Mix_PlayingMusic())
        Mix_PlayMusic(sound, -1);
}

void Clock::startTimer()
{
    state = ClockState::RUNNING_TIMER;

    if (!sound)
    {
        // load default sound
        sound = Mix_LoadMUS((Settings::DEEPINSONGDIR + "system-shutdown.wav").c_str());
        if (!sound)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "NO SOUND: %s", Mix_GetError());
            bgColor->red();
        }
        else
        {
            SDL_Log("Using default deepin sounds!");
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
    if (timeLeft <= std::chrono::seconds(0))
        timeLeft += std::chrono::hours(24);

    // update variables that tell how much time is remaining
    updateClockValues();

    // update texture
    updateTimerTexture();
}

void Clock::updateTimer()
{
    static std::time_t lastSystemTime = std::time(nullptr);
    auto currentSystemTime = std::time(nullptr);

    // newtime is different than old time
    if (std::difftime(currentSystemTime, lastSystemTime) != 0)
    {
        // get offset off new system time
        std::tm localtm = *std::localtime(&currentSystemTime);
        int currentOffset = localtm.tm_gmtoff;
        int lastOffset = localtm.tm_gmtoff - std::difftime(currentSystemTime, lastSystemTime);

        // remove offset from timeLeft (normally 1 second)
        timeLeft -= std::chrono::seconds(currentOffset - lastOffset);

        // update variables and texture
        updateClockValues();
        updateTimerTexture();

        lastSystemTime = currentSystemTime;
    }

    // timer has been reached
    if (std::chrono::system_clock::now() >= std::chrono::system_clock::now() + timeLeft)
    {
        if (opts.closeImmediately)
            exit(0);

        // update state
        state = ClockState::RINGING;

        // hide the time left texture
        timeLeftTex = nullptr;

        // make the backgroundColor white
        bgColor->white();
    }
}

void Clock::updateTimerTexture()
{
    // update timer
    SDL_Surface* surface = TTF_RenderText_Solid(font, createTimeString().c_str(), clockColor.getSDLColor());
    timeLeftTex = SDL_CreateTextureFromSurface(Renderer::get(), surface);
    SDL_FreeSurface(surface);
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

    if (minutesLeft < 10)
        ss << "0" << minutesLeft;
    else
        ss << minutesLeft;

    ss << ":";

    if (secondsLeft < 10)
        ss << "0" << secondsLeft;
    else
        ss << secondsLeft;

    return ss.str();
}

void Clock::addHour()
{
    if (state != ClockState::NOT_SET)
        return;

    if (hours >= 23)
        hours = 0;
    else
        hours++;

    updateTexture();
}

void Clock::removeHour()
{
    if (state != ClockState::NOT_SET)
        return;

    if (hours <= 0)
        hours = 23;
    else
        hours--;

    updateTexture();
}

void Clock::addMinute()
{
    if (state != ClockState::NOT_SET)
        return;

    if (minutes >= 59)
        minutes = 0;
    else
        minutes++;

    updateTexture();
}

void Clock::removeMinute()
{
    if (state != ClockState::NOT_SET)
        return;

    if (minutes <= 0)
        minutes = 59;
    else
        minutes--;

    updateTexture();
}
