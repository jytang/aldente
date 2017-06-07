#pragma once

#include "shadows.h"
#include "window.h"
#include "scene_manager.h"
#include "render/hdr.h"
#include "bt_debug.h"

class Render {
private:
    Shadows shadows;

    HDR hdr;

    Window &window;

    SceneManager &scene_manager;

    btDebug *bt_debug;

public:
    Render(Window &window, SceneManager &scene_manager);

    void update();

};
