#include "bt_debug.h"
#include "GL/glew.h"
#include <iostream>
#include "shaders/shader_manager.h"
#include "scene_manager.h"

btDebug::btDebug(Physics *physics) : physics(physics) {
    setDebugMode(btIDebugDraw::DBG_DrawWireframe);

    lines = new Mesh();
    geo = new Geometry();
    mat = new Material(Color::RED);

    geo->draw_type = GL_LINES;
    mat->shadows = false;
    lines->material = mat;
    lines->geometry = geo;
}

void btDebug::set_enable(bool b) {
    enabled = b;
}

void btDebug::clear() {
    geo->vertices.clear();
    geo->normals.clear();
    geo->indices.clear();
}

void btDebug::draw(SceneInfo &scene_info) {
    if (!enabled)
        return;

    if (physics->dynamicsWorld)
        physics->dynamicsWorld->setDebugDrawer(this);

    clear();
    physics->dynamicsWorld->debugDrawWorld();

    for (int i = 0; i < geo->vertices.size(); i++)
        geo->indices.push_back(i);

    geo->populate_buffers();

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