#pragma once

#include <SFML/Audio.hpp>
#include <string.h>
#include "events.h"

class AudioManager {
public:
    static const std::string BUILD_MUSIC;
    static const std::string DUNGEON_MUSIC;

    AudioManager();
private:
    sf::Music music;
    bool muted;
};