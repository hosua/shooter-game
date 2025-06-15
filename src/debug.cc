#include <SDL2/SDL_mixer.h>

#include "debug.hh"
#include "imgui.h"

#include "player.hh"
#include "bullet.hh"
#include "globals.hh"
#include "input.hh"
#include "sounds.hh"

namespace global_vars = Global::Vars;

namespace Debug {
    bool enabled = true;

    void render_menu(Player& player, const BulletFactory& bullet_factory) {
        static int win_w, win_h;
        ImGui::Begin("Debug");
            const auto& [mx, my] = Input::get_mouse_pos();
            const auto& [wmx, wmy] = Input::get_mouse_world_pos();
            ImGui::Text("Mouse pos: (%i, %i)", mx, my);
            ImGui::Text("Mouse world pos: (%i, %i)", wmx, wmy);
            ImGui::Text("Camera pos: (%.1f, %.1f)", Global::camera.x, Global::camera.y);
            ImGui::Text("Letterbox: (%.1f, %.1f, %.1f, %.1f)", 
                        Global::letter_box_rect.x, Global::letter_box_rect.y,
                        Global::letter_box_rect.w, Global::letter_box_rect.h
            );
            SDL_GetWindowSize(Global::window, &win_w, &win_h);
            ImGui::Text("Window size: (%i, %i)", win_w, win_h);

            ImGui::NewLine();

            ImGui::Text("Player");
            ImGui::Text("pos: (%.1f, %.1f)", player._x, player._y);
            ImGui::Text("angle: %.1f", player._angle);
            ImGui::SliderFloat("move speed", &player._move_speed, 1.f, 1000.f);
            ImGui::SliderFloat("bullet speed", &player._bullet_speed, 1.f, 2000.f);

            if (ImGui::Button("Respawn##Player")) {
                player._x = 0.f, player._y = 0.f;
            }

            ImGui::NewLine();

            ImGui::Text("Active bullets: %zu", bullet_factory.get_active_bullet_count());

            ImGui::Checkbox("Pause##Game", &global_vars::paused);

            ImGui::NewLine();
            if ((ImGui::SliderFloat("Volume", &global_vars::volume, 0, MIX_MAX_VOLUME))) {
                Sounds::set_volume(global_vars::volume);
            }
            ImGui::Checkbox("Mute Volume", &global_vars::volume_muted);

            ImGui::NewLine();
            ImGui::Checkbox("Spawn Mode (F1)", &global_vars::spawn_mode);
            ImGui::Checkbox("Debug Mode (`)", &Debug::enabled);
        ImGui::End();
    }
}


