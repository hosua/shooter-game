#pragma once

#include <memory>
#include "sprite.hh"
#include "explosion.hh"
#include <utility>

class EnemyFactory;
class BulletFactory;

class Player {
    public:
        float _x, _y, 
            _w = 25.f, _h = 25.f,
            _move_speed = 300.f,
            _angle = 0.f,
            _bullet_speed = 500.f;

        Player(float x, float y);
        Player(float x, float y, SDL_Texture* texture);
        ~Player();
        void move(int dir_x, int dir_y, float dt);
        void render(float dt) const;
        void shoot(BulletFactory& bullet_factory) const;
        friend void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, 
                                             BulletFactory& bullet_factory, ExplosionFactory& explosion_factory);
    private:
        SDL_Texture* _texture;
        SDL_FRect _hitbox;
        std::unique_ptr<Sprite> _sprite; 
};
