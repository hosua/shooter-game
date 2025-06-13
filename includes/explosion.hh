#pragma once

#include <vector>
#include <memory>
#include "sprite.hh"

class Explosion {
    public:
        Explosion(float x, float y);
        ~Explosion();
        void update(float dt); 
        void render(float dt) const;
    private:
        float _x, _y;
        int _ttl = 90;
        Sprite* _sprite;
        friend class ExplosionFactory;
};

class ExplosionFactory {
    public:
        ExplosionFactory();
        ~ExplosionFactory();
        void spawn(float x, float y);
        void update_all(float dt);
        void cleanup_all();
        void render_all(float dt) const;
    private:
        std::vector<std::unique_ptr<Explosion>> _pool;
};
