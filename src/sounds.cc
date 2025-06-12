#include <algorithm>

#include "sounds.hh"
#include "globals.hh"

namespace global_vars = Global::Vars;

namespace Sounds {
    static std::map<std::string, Mix_Chunk*> bank;
    void load(const std::string& key, const std::string& path) {
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        if (!sound) {
            fprintf(stderr, "Failed to load sound: %s", path.c_str());
            Mix_CloseAudio();
        }
        Mix_VolumeChunk(sound, global_vars::volume);
        bank.insert({ key, sound });
    }

    void play(const std::string& key) {
        if (!global_vars::volume_muted)
            Mix_PlayChannel(-1, bank[key], 0);
    }
    void set_volume(int volume) {
        global_vars::volume = std::clamp(volume, 0, MIX_MAX_VOLUME);
        for (auto& [_, chunk] : bank) {
            Mix_VolumeChunk(chunk, global_vars::volume);
        }
    }
}
