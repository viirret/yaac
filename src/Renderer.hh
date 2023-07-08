#ifndef YAAC_RENDERER_HH
#define YAAC_RENDERER_HH

#include <SDL2/SDL.h>

#include "Color.hh"

namespace Renderer
{

// Static SDL_Renderer pointer.
SDL_Renderer* get();
void set(SDL_Renderer* ren);

void render();
void clear();
void setColor(Color color);

} // namespace Renderer

#endif
