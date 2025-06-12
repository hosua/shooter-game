#pragma once

#include <SDL2/SDL_ttf.h>
#include <array>
#include <string>
#include <unordered_map>
#include <utility>

class TextRenderer {
public:

    static constexpr int FONT_MAX_SIZE = 100;
    TextRenderer();
    ~TextRenderer();
    
    bool load_font(const std::string& id, const std::string& path);
    void render_text(const std::string& font_id, const std::string& text,
                     int x, int y, SDL_Color color, size_t size);
    void clear_fonts();
    std::pair<int, int> get_text_size(const std::string& font_id, const std::string& text, size_t size);
private:
    std::unordered_map<std::string, std::array<TTF_Font*, 100>> _fonts;
};
