#pragma once

#include <SDL_mixer.h>
#include <string>
#include <map>

namespace Sounds {
    void load(const std::string& key, const std::string& path);
    void play(const std::string& key);
    void set_volume(int volume);
}
