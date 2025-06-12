#pragma once

#include <SDL2/SDL.h>
#include "text.hh"

namespace Global {
    extern TextRenderer text_renderer;
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern SDL_FRect letter_box_rect;
    extern SDL_FRect camera; // this should probably go into its own class at some point

    namespace Vars {
        extern bool paused;
        extern float volume;
        extern bool volume_muted;
        extern bool debug;
        extern bool spawn_mode;
    }
}

float random_float(float min, float max);
float random_int(int min, int max);
SDL_Color random_color();

bool check_collision(const SDL_Rect& a, const SDL_Rect& b);
bool check_collision(const SDL_FRect& a, const SDL_Rect& b);
bool check_collision(const SDL_Rect& a, const SDL_FRect& b);
bool check_collision(const SDL_FRect& a, const SDL_FRect& b);

std::pair<float, float> get_rel_pos(const SDL_FRect& rect);
std::pair<int, int> get_rel_pos(const SDL_Rect& rect);

// Not defined in SDL2
#define SDLK_GRAVE '`'
