#include <algorithm>
#include <iostream>
#include <memory>

#include "explosion.hh"
#include "SDL_render.h"
#include "graphics.hh"
#include "globals.hh"
#include "sprite.hh"
#include "sounds.hh"

namespace gfx = Graphics;

Explosion::Explosion(float x, float y): 
    _x(x), _y(y) {
    _texture = gfx::load_texture("assets/spaceship/Space Ships Explosion.png");
    _sprite = new Sprite(_texture, 48, 48, 6, 10.f);
    Sounds::play("explosion");
}

Explosion::~Explosion() {
    SDL_DestroyTexture(_texture);
}

void Explosion::update(float dt) {
    --_ttl;
}

void Explosion::render(float dt) const {
    SDL_FRect world_rect = { _x, _y, 48, 48 };
    const auto& [ex, ey] = get_rel_pos(world_rect);
    _sprite->render(ex, ey, 0.f);
    _sprite->update(dt);
}

ExplosionFactory::ExplosionFactory() {}
ExplosionFactory::~ExplosionFactory() {}

void ExplosionFactory::spawn(float x, float y) {
    std::unique_ptr<Explosion> explosion = std::make_unique<Explosion>(x, y);
    _pool.push_back(std::move(explosion));
}

void ExplosionFactory::render_all(float dt) const {
    for (const std::unique_ptr<Explosion>& explosion: _pool) {
        explosion->render(dt) ;
    }
}

void ExplosionFactory::update_all(float dt) {
    for (const std::unique_ptr<Explosion>& explosion : _pool) {
        explosion->update(dt);
    }
}

void ExplosionFactory::cleanup_all() {
        _pool.erase(
        std::remove_if(_pool.begin(), _pool.end(), 
            [](std::unique_ptr<Explosion>& explosion){
                return  explosion->_ttl < 0 ||
                        explosion->_x < Global::camera.x - 500 || 
                        explosion->_x > Global::camera.x + Graphics::VIRTUAL_WIDTH + 500 ||
                        explosion->_y < Global::camera.y - 500 || 
                        explosion->_y > Global::camera.y + Graphics::VIRTUAL_HEIGHT + 500;
            }), _pool.end()
    );
}
