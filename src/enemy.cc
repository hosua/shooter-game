#include <algorithm>
#include <cmath>
#include <iostream>

#include "enemy.hh"
#include "SDL_render.h"
#include "globals.hh"
#include "graphics.hh"
#include "sprite.hh"


namespace gfx = Graphics;
namespace global_vars = Global::Vars;
constexpr int EW = 48, EH = 60;

Enemy::Enemy(float x, float y): _x(x), _y(y), 
    _w(EW), _h(EH),
    _angle(0), _speed(0),
    _is_alive(true), 
    _hitbox({ x, y, 32, 32 }) 
{
    _texture = gfx::load_texture("assets/spaceship/ship_5_sheet.png");
    _sprite = std::make_unique<Sprite>(_texture, EW, EH, 2, 10.f);
}

Enemy::~Enemy() {}

void Enemy::render(float dt) const {
    SDL_FRect world_rect = { _x, _y, _w, _h };
    const auto& [ex, ey] = get_rel_pos(world_rect);
    _sprite->render(ex, ey, _angle);
    _sprite->update(dt);
    if (global_vars::debug){
        SDL_SetRenderDrawColor(Global::renderer, 0, 255, 68, 255);
        SDL_RenderDrawRectF(Global::renderer, &_hitbox);
    }
}

void Enemy::update(float dt) {
    SDL_FRect world_rect = { _x, _y, _w, _h };
    const auto& [ex, ey] = get_rel_pos(world_rect);
    _hitbox = { ex + 8, ey + 12, _hitbox.w, _hitbox.h };
    SDL_FRect player_world_rect = {
        global_vars::player_pos.x,
        global_vars::player_pos.y,
        48,
        60,
    };
    const float center_x = static_cast<float>(gfx::VIRTUAL_WIDTH) / 2, 
        center_y = static_cast<float>(gfx::VIRTUAL_HEIGHT) / 2;
    const float dy = ey - center_y, dx = ex - center_x;
    _angle = atan2(dy, dx) * 180 / static_cast<float>(M_PI) - 90;
}

bool Enemy::check_collision_with_bullet(const Bullet& bullet) {
    const bool res = check_collision(_hitbox, bullet._hitbox) && bullet._is_player_bullet;
    if (res) _is_alive = false;
    return res;
}

// Factory

EnemyFactory::EnemyFactory(){}
EnemyFactory::~EnemyFactory(){}

void EnemyFactory::spawn(float x, float y) {
    std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(x, y);
    _pool.push_back(std::move(enemy));
}

void EnemyFactory::render_all(float dt) const {
    for (const std::unique_ptr<Enemy>& enemy : _pool) {
        enemy->render(dt);
    }
}

void EnemyFactory::update_all(float dt) {
    for (const std::unique_ptr<Enemy>& enemy : _pool) {
        enemy->update(dt);
    }
}

static int n = 0;

void EnemyFactory::cleanup_all() {
    _pool.erase(
        std::remove_if(_pool.begin(), _pool.end(), 
            [](std::unique_ptr<Enemy>& enemy){
                return !enemy->_is_alive ||
                        enemy->_x < Global::camera.x - 500 || 
                        enemy->_x > Global::camera.x + Graphics::VIRTUAL_WIDTH + 500 ||
                        enemy->_y < Global::camera.y - 500 || 
                        enemy->_y > Global::camera.y + Graphics::VIRTUAL_HEIGHT + 500;
            }), _pool.end()
    );
}
