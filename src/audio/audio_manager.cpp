#include <string>
#include <iostream>
#include "audio_manager.h"

const std::string AudioManager::BUILD_MUSIC = "mikoto.wav";
const std::string AudioManager::DUNGEON_MUSIC = "k_theme.wav";

AudioManager::AudioManager() {
    events::music_event.connect([&](const events::AudioData &d) {
        // Stop the current music from playing, if any
        if (music.getStatus() == sf::SoundSource::Status::Playing) {
            music.stop();
        }
        
        std::string filename = d.filename;
        if (!music.openFromFile(filename)) {
            std::cerr << "AudioManager: Cannot open " << filename << std::endl;;
        }
        music.setLoop(true);
        music.play();
    });

    events::toggle_mute_event.connect([&]() {
        if (music.getStatus() == sf::SoundSource::Status::Paused) {
            music.play();
        } else {
            music.pause();
        }
        music.pause();
    });
}
