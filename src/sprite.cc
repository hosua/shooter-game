#include "sprite.hh"
#include "globals.hh"

Sprite::Sprite(SDL_Texture* texture, int w, int h, int frame_count, float fps)
        : _texture(texture), _w(w), _h(h),
          _frame_count(frame_count), _fps(fps), _current_frame(0), _time_since_last_frame(0.0f) {}

void Sprite::update(float delta_time) {
    _time_since_last_frame += delta_time;
    float time_per_frame = 1.0f / _fps;

    if (_time_since_last_frame>= time_per_frame) {
        _current_frame = (_current_frame + 1) % _frame_count;
        _time_since_last_frame -= time_per_frame;
    }
}

void Sprite::render(int x, int y, float angle, SDL_RendererFlip flip) const {
    SDL_Rect src = { _current_frame * _w, 0, _w, _h };
    SDL_Rect dest = { x, y, _w, _h };
    SDL_Point center = { _w / 2, _h / 2 };
    SDL_RenderCopyEx(Global::renderer, _texture, &src, &dest, angle, &center, flip);
}

