#include <algorithm>
#include <iostream>

#include "enemy.hh"
#include "globals.hh"
#include "graphics.hh"

constexpr float DIMS = 25.f;

Enemy::Enemy(float x, float y): _x(x), _y(y), 
    _w(DIMS), _h(DIMS),
    _angle(0), _speed(0),
    _is_alive(true), 
    _hitbox({ x, y, DIMS, DIMS }) 
{}

Enemy::~Enemy() {}

void Enemy::render(float dt) const {
    SDL_SetRenderDrawColor(Global::renderer, 128, 128, 128, 255);
    SDL_FRect world_rect = { _x, _y, _w, _h };
    const auto& [ex, ey] = get_rel_pos(world_rect);
    SDL_FRect rel_rect = { ex, ey, _w, _h };
    SDL_RenderFillRectF(Global::renderer, &rel_rect);
}

void Enemy::update(float dt) {
    SDL_FRect world_rect = { _x, _y, _w, _h };
    const auto& [ex, ey] = get_rel_pos(world_rect);
    _hitbox = { ex, ey, _hitbox.w, _hitbox.h };
}

bool Enemy::check_collision_with_bullet(const Bullet& bullet) {
    const bool res = check_collision(_hitbox, bullet._hitbox);
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
