#include <iostream>
#include <array>
#include <utility>
#include <string>
#include "text.hh"
#include "SDL_ttf.h"
#include "globals.hh"

TextRenderer::TextRenderer(){}

TextRenderer::~TextRenderer() {
    clear_fonts();
}

bool TextRenderer::load_font(const std::string& id, const std::string& path){

    _fonts[id] = {};
    for (int i = 0; i < FONT_MAX_SIZE; ++i) {
        TTF_Font* font = TTF_OpenFont(path.c_str(), i+1);
        if (!font) {
            std::cerr << "Failed to load font '" << path << "': " << TTF_GetError() << "\n";
            return false;
        }
        _fonts[id][i] = font;
    }
    return true;
}

void TextRenderer::render_text(const std::string& font_id, const std::string& text,
                               int x, int y, SDL_Color color, size_t size) {
    std::unordered_map<std::string, std::array<TTF_Font*, 100>>::iterator it = _fonts.find(font_id);
    if (it == _fonts.end()) {
        std::cerr << "Font '" << font_id << "' not loaded.\n";
        return;
    }
    std::array<TTF_Font*, FONT_MAX_SIZE> font_sizes = it->second;
    if (size > FONT_MAX_SIZE) {
        std::cerr << "Font size: " << size << " not found for font " << font_id << "\n";
        return;
    }
    TTF_Font* font = font_sizes[size];
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Text render failed: " << TTF_GetError() << "\n";
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Global::renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << "\n";
        return;
    }

    SDL_Rect dstRect = { x, y, 0, 0 };
    SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
    SDL_RenderCopy(Global::renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
}

void TextRenderer::clear_fonts() {

}

std::pair<int,int> TextRenderer::get_text_size(const std::string& font_id, const std::string& text, size_t font_size) {
    if (font_size < 0 || font_size > FONT_MAX_SIZE) {
        std::cerr << "Invalid font size: " << font_size << "\n";
        return { -1, -1 };
    }
    
    TTF_Font* font = _fonts[font_id][font_size];
    int w, h;
    if (TTF_SizeText(font, text.c_str(), &w, &h) != 0) {
        std::cerr << "TTF_SizeText failed: " << TTF_GetError() << "\n";
        return { -1, -1 };
    }
    return { w, h };
}
