#include "bullet.hh"
#include "graphics.hh"
#include "globals.hh"

#include <algorithm>
#include <iostream>

namespace gfx = Graphics;

namespace global_vars = Global::Vars;

constexpr int BW = 48, BH = 48;

Bullet::Bullet(float x, float y, float angle, float speed, bool is_player_bullet): 
    _x(x), _y(y), _angle(angle), _speed(speed), _hitbox({x, y, 10 ,10}),
    _is_destroyed(false) {
    _sprite = std::make_unique<Sprite>(gfx::textures["bullet"], BW, BH, 2, 10.f);
}

Bullet::~Bullet(){}
            
void Bullet::render(float dt) const {
    _sprite->update(dt); 
    
    const auto& [bx, by] = get_rel_pos();
    _sprite->render(bx, by, _angle);

    if (global_vars::debug) {
        SDL_SetRenderDrawColor(Global::renderer, 0, 255, 68, 255);
        SDL_RenderDrawRectF(Global::renderer, &_hitbox);
    }
}

// Needed to do some different calculation for the bullet, so we're not using
// the global one
std::pair<float, float> Bullet::get_rel_pos() const {
    return { _x - Global::camera.x, _y - Global::camera.y + 8.f};
}

void Bullet::update(float dt) {
    float radians = (_angle - 90.f) * M_PI / 180.0f;
    _x += std::cos(radians) * _speed * dt;
    _y += std::sin(radians) * _speed * dt;
    const auto& [bx, by] = get_rel_pos();
    _hitbox = {
        bx + 18.f,
        by + 18.f,
        _hitbox.w,
        _hitbox.h
    };
}

// Factory

BulletFactory::BulletFactory() {}
BulletFactory::~BulletFactory() {}

void BulletFactory::spawn(float x, float y, float angle, float speed, bool is_player_bullet) {
    std::unique_ptr<Bullet> bullet = 
        std::make_unique<Bullet>(x, y, angle, speed, is_player_bullet);
    _pool.push_back(std::move(bullet));
}

void BulletFactory::render_all(float dt) const {
    for (const std::unique_ptr<Bullet>& bullet: _pool) {
        bullet->render(dt);
    }
}

void BulletFactory::update_all(float dt) const {
    for (const std::unique_ptr<Bullet>& bullet: _pool) {
        bullet->update(dt);
    }
}

void BulletFactory::cleanup_all() {
    _pool.erase(
        std::remove_if(_pool.begin(), _pool.end(), 
                        [](std::unique_ptr<Bullet>& bullet){
                            return bullet->_is_destroyed || 
                                    bullet->_x < Global::camera.x - 500 || 
                                    bullet->_x > Global::camera.x + Graphics::VIRTUAL_WIDTH + 500 ||
                                    bullet->_y < Global::camera.y - 500 || 
                                    bullet->_y > Global::camera.y + Graphics::VIRTUAL_HEIGHT + 500;
                        }), _pool.end()
    );
}

size_t BulletFactory::get_active_bullet_count() const {
    return _pool.size();
}
