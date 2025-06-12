#include <iostream>

#include "input.hh"
#include "graphics.hh"
#include "globals.hh"

std::pair<int, int> Input::get_mouse_pos() {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Check if mouse is inside the canvas
    if (mouse_x < Global::letter_box_rect.x ||
        mouse_x >= Global::letter_box_rect.x + Global::letter_box_rect.w ||
        mouse_y < Global::letter_box_rect.y || 
        mouse_y >= Global::letter_box_rect.y + Global::letter_box_rect.h) {
        return { -1, -1 }; // Outside of canvas
    }

    float scale_x = static_cast<float>(Graphics::VIRTUAL_WIDTH) / Global::letter_box_rect.w;
    float scale_y = static_cast<float>(Graphics::VIRTUAL_HEIGHT) / Global::letter_box_rect.h;

    int virt_x = static_cast<int>((mouse_x - Global::letter_box_rect.x) * scale_x);
    int virt_y = static_cast<int>((mouse_y - Global::letter_box_rect.y) * scale_y);
    return { virt_x, virt_y };
}

std::pair<int, int> Input::get_mouse_world_pos() {
    const auto& [mx, my] = Input::get_mouse_pos();
    int ww, wh;
    SDL_GetWindowSize(Global::window, &ww, &wh);
    float dx = abs(Global::letter_box_rect.w - ww),
        dy = abs(Global::letter_box_rect.h - wh);
    float world_mouse_x = mx + Global::letter_box_rect.x + Global::camera.x - (dx / 2);
    float world_mouse_y = my + Global::letter_box_rect.y + Global::camera.y - (dy / 2);
    return { world_mouse_x, world_mouse_y };
}

