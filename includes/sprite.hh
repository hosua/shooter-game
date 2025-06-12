#pragma once

#pragma once

#include <SDL2/SDL.h>

class Sprite {
public:
    int _current_frame;
    Sprite(SDL_Texture* texture, int w, int h, int frame_count, float fps);

    void update(float delta_time);

    // void render(int x, int y) const ;
    void render(int x, int y, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

private:
    SDL_Texture* _texture;
    int _w, _h, _frame_count;
    float _fps;
    float _time_since_last_frame;
    bool _flipped;
};
