#include "geometry.h"
#include "SOIL.h"

Geometry::Geometry(GLenum draw, GLint wrap, GLint filter) :
        draw_type(draw),
        wrap_type(wrap),
        filter_type(filter){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    glGenBuffers(1, &EBO);
}

Geometry::~Geometry() {}

void Geometry::populate_buffers() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tex_coords.size(), tex_coords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Geometry::attach_texture(GLuint tex_id) {
    texture = tex_id;
    has_texture = true;
}

GLuint Geometry::get_texture() {
    return texture;
}

void Geometry::draw() {
    glDrawElements(draw_type, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
}

void Geometry::bind() {
    glBindVertexArray(VAO);
}
