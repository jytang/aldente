#include "audio_manager.h"

#include <boost/range.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <iostream>

#define SOUND_DIR_PATH "assets/audio/sound/"

const std::string AudioManager::BUILD_MUSIC = "assets/audio/music/motif6.wav";
const std::string AudioManager::DUNGEON_MUSIC = "assets/audio/music/motif8.wav";

const std::string AudioManager::BUILD_CONFIRM_SOUND = "assets/audio/sound/build_confirm.wav";
const std::string AudioManager::FOOTSTEPS_SOUND = "assets/audio/sound/footsteps.wav";

AudioManager::AudioManager() : muted(true) {
    loadSounds();

    events::music_event.connect([&](const events::AudioData &d) {
        // Stop the current music from playing, if any
        if (music.getStatus() == sf::SoundSource::Status::Playing) {
            music.stop();
        }
        
        std::string filename = d.filename;
        if (!music.openFromFile(filename)) {
            std::cerr << "AudioManager: Cannot open " << filename << std::endl;;
        }

        music.setVolume(d.volume);
        music.setLoop(d.loop);

        if (muted) return;
        
        music.play();
    });

    events::sound_effects_event.connect([&](const events::AudioData &d) {
        std::string filename = d.filename;

        sounds[filename].setVolume(d.volume);
        sounds[filename].setLoop(d.loop);

        if (d.loop) {
            sounds[filename].play();
            sounds[filename].pause();
        }

        if (muted) return;

        sounds[filename].play();
    });

    events::stop_sound_effects_event.connect([&](const std::string filename) {
        sounds[filename].stop();
    });

    events::toggle_mute_event.connect([&]() {
        muted = !muted;
        if (muted) {
            music.pause();

            // Mute all sound effects
            for (auto &sound : sounds) {
                sound.second.pause();
            }
        } else {
            music.play();

            // Play looped sound effects that has been paused
            for (auto &sound : sounds) {
                if (sound.second.getStatus() == sf::SoundSource::Status::Paused) {
                    sound.second.play();
                }
            }
        }
    });
}

void AudioManager::loadSounds() {
    // Load sound effects into memory
    boost::filesystem::path path = boost::filesystem::path(SOUND_DIR_PATH);
    
    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {
        std::string filename = SOUND_DIR_PATH + entry.path().filename().string();
        if (!sound_buffers[filename].loadFromFile(filename)) {
            std::cerr << "AudioManager: Cannot open" << filename << std::endl;
        }
        sounds[filename] = sf::Sound(sound_buffers[filename]);
    }
}
