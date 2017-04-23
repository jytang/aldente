#include "mesh.h"


Mesh::Mesh(Geometry *geo,
           Material *mat,
           Shader *shader,
           glm::mat4 m,
           bool no_cull) :
        geometry(geo),
        material(mat),
        shader(shader),
        to_world(m),
        no_culling(no_cull) {}


