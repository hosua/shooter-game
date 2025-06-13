#include <algorithm>
#include <SDL2/SDL.h>
#include <iostream>

#include "bullet.hh"
#include "graphics.hh"
#include "globals.hh"
#include "input.hh"
#include "player.hh"
#include "sounds.hh"

namespace gfx = Graphics;
namespace global_vars = Global::Vars;
constexpr int PW = 48, PH = 60;

Player::Player(float x, float y): _x(x), _y(y), _w(PW), _h(PH) {
    _sprite = std::make_unique<Sprite>(gfx::textures["player"], PW, PH, 2, 10.f);
    _hitbox = { 0, 0, 30, 30 };
}

Player::~Player() {}

void Player::render(float dt) const {
    const float distance = 29.0f;
    SDL_FRect world_rect = { _x, _y, _w, _h };
    const auto& [px, py] = get_rel_pos(world_rect);
    _sprite->render(px, py, _angle);
    _sprite->update(dt);

    if (global_vars::debug) {
        SDL_SetRenderDrawColor(Global::renderer, 0, 255, 68, 255);
        SDL_RenderDrawRectF(Global::renderer, &_hitbox);
    }
}

void Player::move(int dir_x, int dir_y, float dt) {
    dir_x = std::clamp(dir_x, -1, 1);
    dir_y = std::clamp(dir_y, -1, 1);
    const float dx = static_cast<float>(dir_x < 0 ? -1 : 1);
    const float dy = static_cast<float>(dir_y < 0 ? -1 : 1);
    if (dir_x) _x += dx * _move_speed * dt; 
    if (dir_y) _y += dy * _move_speed * dt; 
    const auto& [mx, my] = Input::get_mouse_pos();
    if (mx >= 0 && my >= 0) {
        const auto& [wmx, wmy] = Input::get_mouse_world_pos();
        const float wmdx = wmx - _x;
        const float wmdy = wmy - _y;
        _angle = std::atan2(wmdy, wmdx) * 180.0f / static_cast<float>(M_PI) + 90.f;
    }
    SDL_FRect world_rect = { _x, _y, _w, _h };
    const auto& [px, py] = get_rel_pos(world_rect);
    _hitbox = {
        px + 8.f,
        py + 14.f, 
        _hitbox.w,
        _hitbox.h
    };

}

void Player::shoot(BulletFactory& bullet_factory) const {
    float center_x = _x - _w / 2.0f;
    float center_y = _y - PH / 2.0f;
    float rad = (_angle - 90.0f) * (M_PI / 180.0f);
    float bullet_x = center_x + std::cos(rad);
    float bullet_y = center_y + std::sin(rad);
    bullet_factory.spawn(bullet_x, bullet_y, _angle, _bullet_speed, true);
    Sounds::play("shoot");
}
