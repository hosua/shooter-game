#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "graphics.hh"
#include "globals.hh"
#include "text.hh"

namespace Graphics {
    TextRenderer text_renderer;

    std::unordered_map<std::string, SDL_Texture*> textures;

    float window_aspect_ratio;
    float target_aspect_ratio;

    SDL_FRect get_letterbox_rect(float win_w, float win_h) { 
        window_aspect_ratio = static_cast<float>(win_w) / win_h;
        target_aspect_ratio = static_cast<float>(VIRTUAL_WIDTH) / VIRTUAL_HEIGHT;
        

        if (window_aspect_ratio > target_aspect_ratio) {
            float w = win_h * target_aspect_ratio;
            return  { (win_w - w) / 2, 0, w, win_h };
        } else {
            float h = win_w / target_aspect_ratio;
            return { 0, (win_h - h) / 2, win_w, h };
        }
    }

    void render_gridlines(int tile_w, int tile_h, float scale) {
        SDL_SetRenderDrawBlendMode(Global::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Global::renderer, 255, 255, 255, 200);

        const int tw = tile_w * scale;
        const int th = tile_h * scale;

        int start_x = static_cast<int>(Global::camera.x) / tw;
        int start_y = static_cast<int>(Global::camera.y) / th;

        int screen_start_x = -(static_cast<int>(Global::camera.x) % tw);
        int screen_start_y = -(static_cast<int>(Global::camera.y) % th);

        for (int x = screen_start_x; x < Graphics::VIRTUAL_WIDTH; x += tw) {
            SDL_RenderDrawLine(Global::renderer, x, 0, x, Graphics::VIRTUAL_HEIGHT);
        }

        for (int y = screen_start_y; y < Graphics::VIRTUAL_HEIGHT; y += th) {
            SDL_RenderDrawLine(Global::renderer, 0, y, Graphics::VIRTUAL_WIDTH, y);
        }
    }

    SDL_Texture* load_texture(const std::string& key, const std::string& file) {
        SDL_Surface* surface = IMG_Load(file.c_str());
        if (!surface) {
            std::cerr << "IMG_Load failed: " << IMG_GetError() << std::endl;
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Global::renderer, surface);
        textures[key] = texture;
        SDL_FreeSurface(surface);
        return texture;
    }
}
