#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <assimp/scene.h>

#include "geometry.h"
#include "material.h"
#include "model_filter.h"

class Mesh {
public:
    Mesh(Geometry *geo = NULL,
         Material *mat = NULL,
         glm::mat4 m = glm::mat4(1.f))
            : geometry(geo), material(mat),
              local_transform(m) {}

    Geometry *geometry;
    Material *material;
    ModelFilter model_filter;
    glm::mat4 local_transform;

    // map<string, Anim*> animations();
    aiMatrix4x4 inverseBoneMat;
};
