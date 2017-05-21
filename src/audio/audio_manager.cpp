#include <string>
#include <iostream>
#include "audio_manager.h"

const std::string AudioManager::BUILD_MUSIC = "mikoto.wav";
const std::string AudioManager::DUNGEON_MUSIC = "k_theme.wav";

AudioManager::AudioManager() : muted(true) {
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
        if (!muted)
            music.play();
    });

    events::toggle_mute_event.connect([&]() {
        muted = !muted;
        if (muted) {
            music.pause();
        } else {
            music.play();
        }
    });
}
