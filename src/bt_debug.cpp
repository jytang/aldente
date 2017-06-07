#include "bt_debug.h"
#include "GL/glew.h"
#include "shaders/shader_manager.h"
#include "game/game_state.h"
#include "events.h"

#include <iostream>

btDebug::btDebug() {
    setDebugMode(btIDebugDraw::DBG_DrawWireframe);

    lines = new Mesh();
    geo = new Geometry();
    mat = new Material(Color::RED);

    geo->draw_type = GL_LINES;
    mat->shadows = false;
    lines->material = mat;
    lines->geometry = geo;

    // Connect to debug toggle event.
    events::debug::toggle_bt_debug_drawer_event.connect([&](void) {
        enabled = !enabled;
    });
}

void btDebug::set_enable(bool b) {
    enabled = b;
}

void btDebug::clear() {
    geo->vertices.clear();
    geo->normals.clear();
    geo->indices.clear();
}

void btDebug::draw() {
    if (!enabled)
        return;

    Physics *physics = &GameState::physics;
    SceneInfo scene_info = GameState::scene_manager.get_current_scene()->info;

    if (physics->dynamicsWorld == NULL)
        return;
    
    physics->dynamicsWorld->setDebugDrawer(this);

    // Clears past buffers
    clear();

    // Calls "drawLine" for all box colliders registered in the world
    // Note: This requires the rigid bodies to exist on client
    physics->dynamicsWorld->debugDrawWorld();

    for (int i = 0; i < geo->vertices.size(); i++)
        geo->indices.push_back(i);

    geo->populate_buffers();

    // Draws all lines
    ShaderManager::unlit.use();
    ShaderManager::unlit.draw(lines, scene_info, glm::mat4(1.0f));
}

void btDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    geo->vertices.push_back({ from.getX(), from.getY(), from.getZ() });
    geo->vertices.push_back({ to.getX(), to.getY(), to.getZ() });
    geo->normals.push_back({ 1, 1, 1 });
    geo->normals.push_back({ 1, 1, 1 });
}

void btDebug::setDebugMode(int mode) { 
    debug_mode = (DebugDrawModes)mode; 
}

int btDebug::getDebugMode() const {
    return debug_mode;
}