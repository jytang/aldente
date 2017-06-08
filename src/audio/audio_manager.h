#pragma once

#include <SFML/Audio.hpp>
#include <string.h>
#include <unordered_map>
#include <vector>
#include "events.h"

class AudioManager {
public:
    static const std::string BUILD_MUSIC;
    static const std::string DUNGEON_MUSIC;

    static const std::string BUILD_CONFIRM_SOUND;
    static const std::string ARROW_SWOOSH_SOUND;
    static const std::string DREAM_ESSENCE_SOUND;
    static const std::string PORTAL_BUZZ_SOUND;
    static const std::string TAKE_DAMAGE_SOUND;
    static const std::string INVALID_BUZZ_SOUND; // Not used yet
    static const std::string DIALOGUE_SOUND; // Not used yet
    static const std::string SLIME_JUMP_SOUND; // Not used yet
    static const std::string DROP_FROM_SKY_SOUND; // Not used yet
    static const std::string SELECT1_SOUND; // Not used yet
    static const std::string SELECT2_SOUND; // Not used yet

    std::unordered_map<std::string, sf::SoundBuffer> sound_buffers;

    AudioManager();
private:
    static const float SFX_DECREASE_COEFFICIENT;

    sf::Music music;
    std::vector<sf::Sound> active_sounds;
    bool muted;
    float max_music_volume, max_sound_effects_volume;

    void loadSounds();
    int firstInactiveSoundIndex();
    float volumeByDistance(float distance);
};
