#pragma once

class Player;
class BulletFactory;

namespace Debug {
    extern bool enabled;
    
    void render_menu(Player& player, const BulletFactory& bullet_factory);
}
