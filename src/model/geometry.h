#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Geometry {
public:
    bool has_texture;

    GLuint texture;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::ivec4> bone_ids;
    std::vector<glm::vec4> weights;
    std::vector<GLuint> indices;

    GLenum draw_type = GL_TRIANGLES;
    GLint wrap_type = GL_REPEAT;
    GLint filter_type = GL_NEAREST_MIPMAP_LINEAR;

    Geometry(int num_instances = 1,
             GLenum draw = GL_TRIANGLES,
             GLint wrap = GL_REPEAT,
             GLint filter = GL_NEAREST_MIPMAP_LINEAR);

    ~Geometry();

    void populate_buffers();

    void attach_texture(GLuint tex_id);

    GLuint get_texture();

    void draw();

    void bind();

    void bind_instance_matrix(std::vector<glm::mat4> &instance_matrix);

    void set_num_instances(int num) {num_instances = num;};

private:
    GLuint VAO, VBO, NBO, TBO, EBO, BBO, WBO;
    int num_instances;
    GLuint IVBO;
};
