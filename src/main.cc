#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "SDL_keyboard.h"
#include "SDL_keycode.h"

#include "SDL_video.h"
#include "bullet.hh"
#include "enemy.hh"
#include "explosion.hh"
#include "graphics.hh"
#include "globals.hh"
#include "input.hh"
#include "player.hh"
#include "sounds.hh"
#include "text.hh"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

namespace gfx = Graphics;
namespace global_vars = Global::Vars;

float fps = SDL_GetTicks();
uint32_t fps_last_time = 0.f;
uint32_t fps_frames = 0;

void update_camera(const Player& player, float screen_w, float screen_h) {
    Global::camera.x = player._x - screen_w / 2.f;
    Global::camera.y = player._y - screen_h / 2.f;
}

void run_all_collision_checks(Player& player, EnemyFactory& enemy_factory, BulletFactory& bullet_factory, ExplosionFactory& explosion_factory) {
    for (const std::unique_ptr<Bullet>& bullet : bullet_factory._pool) {
        const auto& [bx, by] = bullet->get_rel_pos();
        for (const std::unique_ptr<Enemy>& enemy : enemy_factory._pool) {
            SDL_FRect enemy_world_rect = { enemy->_x, enemy->_y, enemy->_w, enemy->_h };
            const auto& [ex, ey] = get_rel_pos(enemy_world_rect);
            if (check_collision(bullet->_hitbox, enemy->_hitbox)) {
                bullet->_is_destroyed = true;
                enemy->_is_alive = false; 
                explosion_factory.spawn(enemy->_x, enemy->_y);
            }
        }
    }
}

int main () {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        printf("SDL_mixer could not initialize! Mix_OpenAudio: %s\n", Mix_GetError());
        return 1;
    }

    Sounds::load("explosion", "assets/sounds/explosion.wav");
    Sounds::load("shoot", "assets/sounds/shoot.wav");

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    Global::window = SDL_CreateWindow(
        "Shooter Game",                    
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,           
        gfx::VIRTUAL_WIDTH, gfx::VIRTUAL_HEIGHT,                          
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE                  
    );

    if (!Global::window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    Global::renderer = SDL_CreateRenderer(
        Global::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_Texture* canvas = SDL_CreateTexture(
    Global::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
    gfx::VIRTUAL_WIDTH, gfx::VIRTUAL_HEIGHT);

    if (!Global::renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(Global::window);
        SDL_Quit();
        return 1;
    }

    gfx::text_renderer.load_font("pixel_emulator", "assets/ttf/PixelEmulator-xq08.ttf");
    gfx::load_texture("player", "assets/spaceship/ship_6_sheet.png");
    gfx::load_texture("enemy", "assets/spaceship/ship_5_sheet.png");
    gfx::load_texture("explosion", "assets/spaceship/Space Ships Explosion.png");
    gfx::load_texture("bullet", "assets/spaceship/shot.png");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();              
    ImGuiIO& io = ImGui::GetIO();        
    (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(Global::window, Global::renderer);
    ImGui_ImplSDLRenderer2_Init(Global::renderer);
    
    BulletFactory bullet_factory;
    EnemyFactory enemy_factory;
    ExplosionFactory explosion_factory;
    
    Player player(0, 0);
    int count = 0;
    bool running = true;
    SDL_Event event;
    SDL_Rect border_rect = { 0, 0, gfx::VIRTUAL_WIDTH, gfx::VIRTUAL_HEIGHT };
    int win_w, win_h;

    bool lmb_clicked = false;

    while (running) {
        uint32_t frame_start = SDL_GetTicks();
        float dt = (frame_start - fps_last_time) / 1000.f;
        fps_last_time = frame_start;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                lmb_clicked = true;
                    if (!ImGui::GetIO().WantCaptureMouse) {
                    if (global_vars::spawn_mode) {
                        const auto& [wmx, wmy] = Input::get_mouse_world_pos();
                        enemy_factory.spawn(wmx, wmy);
                    } else {
                        player.shoot(bullet_factory);
                    }
                }
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                    case SDLK_SPACE:
                        player.shoot(bullet_factory);
                    break;
                    case SDLK_F1:
                        global_vars::spawn_mode = !global_vars::spawn_mode;
                    break;
                    case SDLK_GRAVE:
                        global_vars::debug = !global_vars::debug;
                        global_vars::spawn_mode = false;
                    break;
                }
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_P) {
                global_vars::paused = !global_vars::paused;
            }
        }

        if (!global_vars::paused) {
            // Do not get keyboard state in poll events!
            const uint8_t* key_state = SDL_GetKeyboardState(nullptr);
            int dir_x = 0, dir_y = 0;
            if (key_state[SDL_SCANCODE_UP] || key_state[SDL_SCANCODE_W]) dir_y = -1;
            else if (key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_S]) dir_y = 1;

            if (key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_A]) dir_x = -1;
            else if (key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_D]) dir_x = 1;
            

            /* Hold shoot, this probably should have delay added. */
            // if (key_state[SDL_SCANCODE_SPACE]) player.shoot(bullet_factory);

            player.move(dir_x, dir_y, dt);
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        if (global_vars::debug) {
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
                ImGui::Checkbox("Debug Mode (`)", &global_vars::debug);
            ImGui::End();
        }
        int drawable_w, drawable_h;
        SDL_GetRendererOutputSize(Global::renderer, &drawable_w, &drawable_h);

        Global::letter_box_rect = gfx::get_letterbox_rect(
            static_cast<float>(drawable_w), 
            static_cast<float>(drawable_h)
        );

        update_camera(player, static_cast<float>(gfx::VIRTUAL_WIDTH), static_cast<float>(gfx::VIRTUAL_HEIGHT));

        SDL_SetRenderTarget(Global::renderer, canvas);
        SDL_SetRenderDrawColor(Global::renderer, 12, 12, 12, 255);
        SDL_RenderClear(Global::renderer);
        /* ======= Begin gameplay drawing logic ======= */

        bullet_factory.render_all(dt);
        enemy_factory.render_all(dt);
        player.render(dt);
        explosion_factory.render_all(dt);

        if (global_vars::debug) {
            gfx::render_gridlines(16, 16, 4.f);
        }

        if (!global_vars::paused) {
            // game logic
            explosion_factory.update_all(dt);
            bullet_factory.update_all(dt);
            enemy_factory.update_all(dt);
            
            run_all_collision_checks(player, enemy_factory, bullet_factory, explosion_factory);

            bullet_factory.cleanup_all();
            enemy_factory.cleanup_all();
            explosion_factory.cleanup_all();
        } 

        /* ======= End drawing logic ======= */
        /* ======= Begin UI drawing logic ======= */
        if (global_vars::paused) {
            auto [paused_tw, paused_th] = gfx::text_renderer.get_text_size("pixel_emulator", "paused", 25);
            gfx::text_renderer.render_text("pixel_emulator", "Paused", 
                                            ((gfx::VIRTUAL_WIDTH / 2) - (paused_tw / 2)), 
                                            ((gfx::VIRTUAL_HEIGHT / 2) - (paused_th / 2)), 
                                            { 255, 255, 255, 255 },
                                            36
            );
        }
        /* ======= End UI drawing logic ======= */

        ImGui::Render();
        SDL_SetRenderTarget(Global::renderer, nullptr); 
        SDL_RenderCopyF(Global::renderer, canvas, nullptr, &Global::letter_box_rect); 
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Global::renderer); 
        SDL_RenderPresent(Global::renderer);  
        // calculate fps
        fps_frames++;
        if (frame_start - fps >= 1000) {
            fps = fps_frames * 1000.f / (frame_start - fps);
            fps = frame_start;
            fps_frames = 0;
        }
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_DestroyTexture(canvas);
    SDL_DestroyRenderer(Global::renderer);
    SDL_DestroyWindow(Global::window);
    SDL_Quit();

    return 0;
}
