#include "globals.hh"
#include <random>

#include <SDL2/SDL_mixer.h>

namespace global_vars = Global::Vars;

bool global_vars::paused = false;
bool global_vars::debug = true;
float global_vars::volume = MIX_MAX_VOLUME;
bool global_vars::volume_muted = false;
bool global_vars::spawn_mode = false;

SDL_Window* Global::window;
SDL_Renderer* Global::renderer;
SDL_FRect Global::letter_box_rect;
TextRenderer Global::text_renderer;
SDL_FRect Global::camera = { 0, 0, 0, 0 };

static std::random_device rd;  
static std::mt19937 gen(rd()); 


float random_float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

float random_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

SDL_Color random_color() {
   return {
    static_cast<uint8_t>(random_int(25, 255)),
    static_cast<uint8_t>(random_int(25, 255)),
    static_cast<uint8_t>(random_int(25, 255)),
    255
   };
}

bool check_collision(const SDL_Rect& a, const SDL_Rect& b) {
    return !((b.x > a.x + a.w) || (b.x + b.w < a.x) ||
             (b.y > a.y + a.h) || (b.y + b.h < a.y));
}

bool check_collision(const SDL_FRect& a, const SDL_Rect& b) {
    return !((b.x > a.x + a.w) || (b.x + b.w < a.x) ||
             (b.y > a.y + a.h) || (b.y + b.h < a.y));
}

bool check_collision(const SDL_Rect& a, const SDL_FRect& b) {
    return !((b.x > a.x + a.w) || (b.x + b.w < a.x) ||
             (b.y > a.y + a.h) || (b.y + b.h < a.y));
}

bool check_collision(const SDL_FRect& a, const SDL_FRect& b) {
    return !((b.x > a.x + a.w) || (b.x + b.w < a.x) ||
             (b.y > a.y + a.h) || (b.y + b.h < a.y));
}

std::pair<float, float> get_rel_pos(const SDL_FRect& rect) {
    return {rect.x - Global::camera.x - (rect.w / 2.f), rect.y - Global::camera.y - (rect.h / 2.f)};
}

std::pair<int, int> get_rel_pos(const SDL_Rect& rect) {
    return {rect.x - Global::camera.x - (rect.w / 2.f), rect.y - Global::camera.y - (rect.h / 2.f)};
}
