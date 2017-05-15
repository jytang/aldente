#include "geometry.h"
#include "SOIL.h"

Geometry::Geometry(int num_instances, GLenum draw, GLint wrap, GLint filter) :
        num_instances(num_instances),
        draw_type(draw),
        wrap_type(wrap),
        filter_type(filter){

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &BBO);
    glGenBuffers(1, &WBO);
    if (num_instances > 1) glGenBuffers(1, &IVBO); // only generate buffer if more than one instance
}

Geometry::~Geometry() {}

void Geometry::populate_buffers() {
    glBindVertexArray(VAO);

    if (vertices.size() != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (normals.size() != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Required to prevent MacOS segfault
    if (tex_coords.size() != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tex_coords.size(), tex_coords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (indices.size() != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    }

    if (bone_ids.size() != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, BBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::ivec4) * bone_ids.size(), bone_ids.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 4, GL_INT, 0, 0);
    }

    if (weights.size() != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, WBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * weights.size(), weights.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Bind instanced VBO if more than one instance
    if (num_instances > 1) {
        glBindBuffer(GL_ARRAY_BUFFER, IVBO);
        glBufferData(GL_ARRAY_BUFFER, num_instances * sizeof(glm::mat4),
                     NULL, // no instance data as of yet, to be binded at render time
                     GL_DYNAMIC_DRAW); // instances can change their positions on the fly

        // Setup 4 vertex attributes for a mat4
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)0);
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));

        // Setup defaults to be identity matrix.
        glVertexAttrib4f(5, 1.f, 0.f, 0.f, 0.f);
        glVertexAttrib4f(6, 0.f, 1.f, 0.f, 0.f);
        glVertexAttrib4f(7, 0.f, 0.f, 1.f, 0.f);
        glVertexAttrib4f(8, 0.f, 0.f, 0.f, 1.f);

        // Step forward by 1 for every instance
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
    }

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
    if (num_instances == 1) {
        glDrawElements(draw_type, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawElementsInstanced(draw_type, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0,
                                num_instances); // same as above except pass in number of instances to OpenGL
    }
}

void Geometry::bind() {
    glBindVertexArray(VAO);
}

// Fill the IVBO buffer with positions of each instance via instance_matrix.
void Geometry::bind_instance_matrix(std::vector<glm::mat4> &instance_matrix) {
    glBindBuffer(GL_ARRAY_BUFFER, IVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(glm::mat4) * instance_matrix.size(),
                    &instance_matrix[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}