#pragma once

#include <SDL2/SDL.h>
#include <string>

#include <unordered_map>
#include <string>

#include "text.hh"

namespace Graphics {
    extern TextRenderer text_renderer;

    constexpr int VIRTUAL_WIDTH = 1280;
    constexpr int VIRTUAL_HEIGHT = 720;
    
    extern float window_aspect_ratio;
    extern float target_aspect_ratio; 
    extern std::unordered_map<std::string, SDL_Texture*> textures;

    SDL_FRect get_letterbox_rect(float win_w, float win_h);
    void render_gridlines(int tile_w, int tile_h, float scale);
    SDL_Texture* load_texture(const std::string& key, const std::string& file);
}
