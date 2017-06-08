#include "audio_manager.h"

#include <boost/range.hpp>
#include <boost/filesystem.hpp>

#include <math.h>
#include <string>
#include <iostream>

#define SOUND_DIR_PATH "assets/audio/sound/"

#define NUM_OF_ACTIVE_SOUNDS 128

#define SFX_DECREASE_DISTANCE_THRESHOLD 7
#define SFX_DECREASE_RATIO 0.1

const std::string AudioManager::BUILD_MUSIC = "assets/audio/music/motif6.wav";
const std::string AudioManager::DUNGEON_MUSIC = "assets/audio/music/motif8.wav";

const std::string AudioManager::BUILD_CONFIRM_SOUND = "assets/audio/sound/build_confirm.wav";
const std::string AudioManager::INVALID_BUZZ_SOUND = "assets/audio/sound/invalid_buzz.wav";
const std::string AudioManager::ARROW_SWOOSH_SOUND = "assets/audio/sound/arrow_swoosh.wav";
const std::string AudioManager::DREAM_ESSENCE_SOUND = "assets/audio/sound/dream_essence.wav";
const std::string AudioManager::PORTAL_BUZZ_SOUND = "assets/audio/sound/portal_buzz.wav";
const std::string AudioManager::TAKE_DAMAGE_SOUND = "assets/audio/sound/take_damage.wav";
const std::string AudioManager::DIALOGUE_SOUND = "assets/audio/sound/dialogue.wav";
const std::string AudioManager::SLIME_JUMP_SOUND = "assets/audio/sound/slime_jump.wav";
const std::string AudioManager::DROP_FROM_SKY_SOUND = "assets/audio/sound/drop_from_sky.wav";
const std::string AudioManager::SELECT1_SOUND = "assets/audio/sound/select1.wav";
const std::string AudioManager::SELECT2_SOUND = "assets/audio/sound/select2.wav";


const float AudioManager::SFX_DECREASE_COEFFICIENT = log(SFX_DECREASE_RATIO) / SFX_DECREASE_DISTANCE_THRESHOLD;

AudioManager::AudioManager() : muted(true), max_music_volume(100.0), max_sound_effects_volume(100.0) {
    // Fill active sounds
    for (int i = 0; i < NUM_OF_ACTIVE_SOUNDS; i++) {
        active_sounds.push_back(sf::Sound());
    }
    
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

        music.setVolume(max_music_volume);
        music.setLoop(d.loop);

        if (muted) return;
        
        music.play();
    });

    events::sound_effects_event.connect([&](const events::AudioData &d) {
        std::string filename = d.filename;

        float vol = volumeByDistance(d.distance);
        int inactive_sound_index = firstInactiveSoundIndex();

        // Give up sound effects request if all sounds are active (Very unlikely)
        if (inactive_sound_index == -1) {
            return;
        }

        active_sounds[inactive_sound_index].setBuffer(sound_buffers[filename]);
        active_sounds[inactive_sound_index].setVolume(vol);

        // Need to think of better way to stop looped sound effects if necessary
        active_sounds[inactive_sound_index].setLoop(d.loop);

        if (muted) return;

        // Debug comment
        // std::cerr << "Sound fx: Playing " << filename << " with loop " << d.loop << " and volume " << vol << " with distance " << d.distance << std::endl;

        active_sounds[inactive_sound_index].play();
    });

    events::stop_all_sounds.connect([&]() {
        for (int i = 0; i < active_sounds.size(); i++) {
            active_sounds[i] = sf::Sound();
        }
    });

    events::toggle_mute_event.connect([&]() {
        muted = !muted;
        if (muted) {
            music.pause();

            // Mute all sound effects
            for (sf::Sound sound : active_sounds) {
                sound.pause();
            }
        } else {
            music.play();

            // Play sound effects that has been paused
            for (sf::Sound sound : active_sounds) {
                if (sound.getStatus() == sf::SoundSource::Status::Paused) {
                    sound.play();
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
            std::cerr << "AudioManager: Cannot open " << filename << std::endl;
        }
    }
}

int AudioManager::firstInactiveSoundIndex() {
    int inactive_index = -1;
    for (int i = 0; i < active_sounds.size(); i++) {
        if (active_sounds[i].getStatus() == sf::Sound::Status::Stopped) {
            inactive_index = i;
            break;
        }
    }
    return inactive_index;
}

/*
 * Model is a decreasing exponential function:
 * Volume heard = (Max volume) * e^(decreasing coefficient * distance between objects)
 * where decreasing coefficient = log(SFX_DECREASE_RATIO) / SFX_DECREASE_DISTANCE_THRESHOLD and < 0
 */
float AudioManager::volumeByDistance(float distance) {
    return max_sound_effects_volume * exp(SFX_DECREASE_COEFFICIENT * distance);
}
