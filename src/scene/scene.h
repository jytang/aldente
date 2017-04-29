#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "shaders/shader.h"
#include "model/plane.h"
#include "grid.h"
#include "boost/signals2.hpp"

/*
    Contains all the information for a single scene
*/
class Scene {
public:
    SceneCamera camera;
    //std::vector<SceneCamera*> cameras;
    std::vector<GameObject*> objs;
    glm::vec3 light_pos;
    std::vector<btRigidBody*> rigids;
    boost::signals2::signal<void(std::pair<bool,btRigidBody*>)> rigidSignal;

    virtual ~Scene() {}

    // Draw using the passed in shader.
    // Default is NULL so that the shader attached to each mesh is used instead.
    void draw(Shader *shader = NULL);

    virtual void update();

    void addRigid(btRigidBody* toAdd);
};

/*
Main Scene for having stuff in our game, especially for testing.
Anything that you would want to add to the game scene that is only
used for the specific scene should be put here.
In the future, the "Dungeon/Build Phase" would be its own scene, and
then each mini game would likely all be its own scene. The title menu
can also be its own scene.
*/
class MainScene : public Scene {
private:
    Grid *grid;
    Tile *hover;
public:
    MainScene();
    void update() override;
};
