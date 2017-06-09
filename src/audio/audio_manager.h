#pragma once

#include <SFML/Audio.hpp>
#include <string.h>
#include <unordered_map>
#include <vector>
#include "events.h"

class AudioManager {
public:
    static const std::string MAIN_MENU_MUSIC;
    static const std::string BUILD_MUSIC;
    static const std::string DUNGEON_MUSIC;
    static const std::string MINIGAME_MUSIC;

    static const std::string BUILD_CONFIRM_SOUND;
    static const std::string ARROW_SWOOSH_SOUND;
    static const std::string DREAM_ESSENCE_SOUND;
    static const std::string PORTAL_BUZZ_SOUND;
    static const std::string TAKE_DAMAGE_SOUND;
    static const std::string INVALID_BUZZ_SOUND;
    static const std::string DIALOGUE_SOUND;
    static const std::string SLIME_JUMP_SOUND; // Not used yet
    static const std::string WHOOSH_SOUND;
    static const std::string SELECT1_SOUND;
    static const std::string SELECT2_SOUND;
    static const std::string FANFARE_SOUND;
    static const std::string CHAR_SELECTION_READY_SOUND;
    static const std::string CHAR_SELECTION_UNREADY_SOUND;
    static const std::string MAIN_MENU_START_SOUND;
    static const std::string CONSTRUCT_SELECTED_SOUND;

    static const std::string LEADERBOARD_SOUND;
    static const std::string BUILD_PHASE_READY_SOUND;
    static const std::string CHEST_OPENING_SOUND;
    static const std::string COUNTDOWN_SOUND;


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
