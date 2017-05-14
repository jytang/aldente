#pragma once

#include "LinearMath/btIDebugDraw.h"
#include "scene/scene_info.h"
#include "model/mesh.h"
#include "physics.h"

/*
    For drawing debug lines for bullet box colliders.
*/
class btDebug : public btIDebugDraw {
public:

    btDebug(Physics *physics);

    void clear();
    void draw(SceneInfo &scene_info);
    void set_enable(bool b);
    
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
    void reportErrorWarning(const char* warningString) override {};
    void draw3dText(const btVector3& location, const char* textString) override {};
    void setDebugMode(int debugMode) override;
    int getDebugMode() const override;

private:
    bool enabled;

    int debug_mode;

    Physics *physics;

    Material *mat;
    Geometry *geo;
    Mesh *lines;
};