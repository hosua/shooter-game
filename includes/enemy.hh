#pragma once

#include <memory>
#include <vector>

#include "bullet.hh"
#include "explosion.hh"
#include "sprite.hh"

class Player;
class EnemyFactory;

enum EnemyType {
    ENEMY_RECT,
    // ENEMY_SHIP1,
};

class Enemy {
    public:
        Enemy(float x, float y, float angle = 0, float speed = 0);
        ~Enemy();
        void render(float dt) const;
        void update(float dt, BulletFactory& bullet_factory);
    private:
        std::unique_ptr<Sprite> _sprite; 
        bool _is_alive;
        float _x, _y, _w, _h, _angle, _speed;
        float _fire_rate = 60;
        float _last_fired = 0;
        SDL_FRect _hitbox;
        friend class EnemyFactory;
        friend void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, 
                                             BulletFactory& bullet_factory, ExplosionFactory& explosion_factory);

};

// Factory 

class EnemyFactory {
    public:
        EnemyFactory();
        ~EnemyFactory();
        void spawn(float x, float y);
        void render_all(float dt) const;
        void update_all(float dt, BulletFactory& bullet_factory);
        void cleanup_all();
    private:
        std::vector<std::unique_ptr<Enemy>> _pool;
        friend void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, 
                                             BulletFactory& bullet_factory, ExplosionFactory& explosion_factory);
};
