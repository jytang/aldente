#pragma once

#include "../../scene/scene.h"
#include "../context.h"

struct MinigameInfo {
    // <button image, text for control>
    // To be used for legend ui
    std::vector<std::pair<std::string, std::string>> input_legend;

    // String describing the minigame's objective
    std::string objective;
};

/*
Contains all the information for a single minigame
*/
class Minigame {
public:
    // Used to setup/teardown the minigame's events and various other things
    virtual void s_setup() {};
    virtual void s_teardown() {};
    virtual void c_setup() {};
    virtual void c_teardown() {};

    // To be called by minigame phase to determine
    // if the minigame should be terminated early
    virtual bool is_finished() {};

    // Used to determine the timer for minigame phase
    std::chrono::seconds get_time() { return time; };

    // Get this minigame's scene
    virtual Scene* get_scene() { return scene; };
protected:
    Scene *scene;
    std::chrono::seconds time;
    MinigameInfo info;
    Context context;
};