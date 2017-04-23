#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>

#include "scene/main_scene.h"
#include "scene/scene_camera.h"

/*
	Main Game Function.
	Game loop and scene setup happens here
*/
class Aldente {
private:
    Aldente();

    // Scenes
    std::vector<Scene *> scenes;
    MainScene *main_scene;

    GLFWwindow *window;
    Scene *scene;
    SceneCamera *camera;

    void setup_scenes();

    void destroy();

public:
    static Aldente *aldente;

    ~Aldente();

    bool shadows_on = true;
    bool debug_shadows = false;

    void start_game_loop();    // Main Game Loop

    std::vector<Scene *> get_scenes(); // Gets list of all scenes in the game
    Scene *get_scene(); // Gets currently active scene
    SceneCamera *get_camera(); // Gets currently active camera
    GLFWwindow *get_window(); // Gets currently active window
};
