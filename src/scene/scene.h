#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_objects/game_object.h"
#include "shaders/shader.h"
#include "scene_info.h"

// These constants should be equivalent to those in the shaders!
#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 10

/*
    Contains all the information for a single scene
*/
class Scene {
public:
    SceneInfo info;
    //std::vector<Camera*> cameras;
    std::vector<GameObject*> objs;

    virtual ~Scene();

    // Draw using the passed in shader.
    // Default is NULL so that the shader attached to each mesh is used instead.
    void draw(Shader *shader = NULL);
    void draw_skybox();
    Camera &get_cam();

    // Add lights to the scene, in accordance with max number of respective light
    void add_light(DirectionalLight *l);
    void add_light(PointLight *l);
    void add_light(SpotLight *l);

    virtual void s_update();
    virtual void c_update();

    // Sets up scene but not its models
    virtual void setup_scene() = 0;

    // Sets up scene's models (please call after setup_scene). Do not call on server
    virtual void graphical_setup() = 0;
};