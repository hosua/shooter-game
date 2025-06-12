#pragma once

#include <utility>
#include "globals.hh"

namespace Input {
    // Get the mouse position after window resizing
    std::pair<int, int> get_mouse_pos();
    std::pair<int, int> get_mouse_world_pos();
}
