#pragma once

#include <SDL2/SDL.h>
#include <utility>
#include <vector>
#include <memory>

#include "sprite.hh"

class Player;
class EnemyFactory;
class BulletFactory;
class ExplosionFactory;

class Bullet {
    public:
        Bullet(float x, float y, float angle, float speed, bool is_player_bullet);
        ~Bullet();
        void render(float dt) const;
        void update(float dt);
    private:
        float _x, _y, _angle, _speed;
        bool _is_player_bullet, _is_destroyed;
        SDL_FRect _hitbox;
        SDL_Texture *_texture;
        std::unique_ptr<Sprite> _sprite;
        std::pair<float,float> get_rel_pos() const; 
        friend class Enemy;
        friend class BulletFactory;
        friend void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, 
                                             BulletFactory& bullet_factory, ExplosionFactory& explosion_factory);
};

class BulletFactory {
    public:
        BulletFactory();
        ~BulletFactory();
        void spawn(float x, float y, float angle, float speed, bool is_player_bullet);
        void render_all(float dt) const;
        void update_all(float dt) const;
        void cleanup_all();
        size_t get_active_bullet_count() const;
    private:
        std::vector<std::unique_ptr<Bullet>> _pool;
        friend void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, 
                                             BulletFactory& bullet_factory, ExplosionFactory& explosion_factory);
};
