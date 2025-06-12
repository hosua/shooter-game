#pragma once

#include <memory>
#include <vector>

#include "bullet.hh"
#include "explosion.hh"

class Player;
class EnemyFactory;

enum EnemyType {
    ENEMY_RECT,
    // ENEMY_SHIP1,
};

class Enemy {
    public:
        Enemy(float x, float y);
        ~Enemy();
        void render(float dt) const;
        void update(float dt);
        bool check_collision_with_bullet(const Bullet& bullet);
    private:
        bool _is_alive;
        float _x, _y, _w, _h, _angle, _speed;
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
        void update_all(float dt);
        void cleanup_all();
    private:
        std::vector<std::unique_ptr<Enemy>> _pool;
        friend void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, 
                                             BulletFactory& bullet_factory, ExplosionFactory& explosion_factory);
};
