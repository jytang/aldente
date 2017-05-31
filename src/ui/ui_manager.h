#pragma once

#include "ui.h"

#include "render2d.h"

#include <string>
#include <map>

class UIManager {
public:
    UIManager(float aspect);
    ~UIManager();
    void draw();
private:
    void setup_uis(); // create all ui objects and store them
    void setup_listeners(); // setup listeners to enable/disable particular uis when necessary

    std::map<std::string, UI *> ui_map; // cached uis
    bool all_enabled = true;
    float aspect;
    Render2D renderer_2d;
};