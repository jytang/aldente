#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "util/util.h"

// TODO : Work on more efficient algorithms for rotation and scaling.

Transform::Transform() {
    world_mat = glm::mat4(1.0f);
}

void Transform::set_world_mat(glm::mat4 mat){
    world_mat = mat;
}

void Transform::set_position(glm::vec3 position) {
    set_position(position.x, position.y, position.z);
}

// Moves to the given position / coordinates.
void Transform::set_position(float x, float y, float z) {
    world_mat[3][0] = x;
    world_mat[3][1] = y;
    world_mat[3][2] = z;
}

void Transform::set_rotation(glm::vec3 angles) {
    set_rotation(angles.x, angles.y, angles.z);
}

// Starting from the object's original orientatin, rotates around the x-axis,
// then y-axis, then z-axis. (x, y, z in degrees)
void Transform::set_rotation(float x, float y, float z) {
    glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), glm::radians(z), glm::vec3(0.f, 0.f, 1.f));

    glm::mat4 rotation = rotate_z * rotate_y * rotate_x;

    world_mat = get_translation_mat() * rotation * get_scale_mat();
}

void Transform::set_scale(glm::vec3 scale) {
    set_scale(scale.x, scale.y, scale.z);
}

// Sets the transform's scale by extracting rotation matrix and scaling
void Transform::set_scale(float x, float y, float z) {

    glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));

    world_mat = get_translation_mat() * get_rotation_mat() * scale_mat;
}

void Transform::translate(glm::vec3 position) {
    translate(position.x, position.y, position.z);
}

// Translate along the given vector from the current position.
void Transform::translate(float x, float y, float z) {
    glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

    world_mat = trans_mat * world_mat;
}

void Transform::rotate(glm::vec3 angles, bool local) {
    rotate(angles.x, angles.y, angles.z, local);
}

// Rotates first around x-axis, then y-axis, then z-axis,
// starting from current orientation. (x, y, z in degrees)
void Transform::rotate(float x, float y, float z, bool local) {

    glm::vec3 current_pos = get_position();

    // For Local, move to origin first, then rotate, then move back
    if (local)
        translate(-current_pos);

    glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), glm::radians(z), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 rotation = rotate_z * rotate_y * rotate_x;

    world_mat = rotation * world_mat;

    if (local)
        translate(current_pos);

}

void Transform::look_at(glm::vec3 dir, float a) {
    if (dir.x != 0 || dir.z != 0) {
        float goal_angle = atan2(dir.x, dir.z);
        float orig_angle = atan2(get_forward().x, get_forward().z);
        float angle = Util::lerp(orig_angle, goal_angle, a);
        glm::mat4 new_world = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        new_world = glm::scale(new_world, get_scale());
        new_world[3] = world_mat[3];
        world_mat = new_world;
    }
}

glm::mat4 Transform::get_world_mat() {
    return world_mat;
}

// Position is obtained from the last column vec3
glm::vec3 Transform::get_position() {
    float x = world_mat[3][0];
    float y = world_mat[3][1];
    float z = world_mat[3][2];

    return glm::vec3(x, y, z);
}

// Scale is obtained by taking the length of the first three column vectors
glm::vec3 Transform::get_scale() {
    glm::vec3 col_0 = glm::vec3(world_mat[0][0], world_mat[0][1], world_mat[0][2]);
    glm::vec3 col_1 = glm::vec3(world_mat[1][0], world_mat[1][1], world_mat[1][2]);
    glm::vec3 col_2 = glm::vec3(world_mat[2][0], world_mat[2][1], world_mat[2][2]);

    return glm::vec3(glm::length(col_0), glm::length(col_1), glm::length(col_2));
}


// Helper function for easily getting translation matrix
glm::mat4 Transform::get_translation_mat() {
    float x = world_mat[3][0];
    float y = world_mat[3][1];
    float z = world_mat[3][2];
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

// Helper fucntion for easily getting rotation matrix
glm::mat4 Transform::get_rotation_mat() {
    glm::vec3 col_0 = glm::vec3(world_mat[0][0], world_mat[0][1], world_mat[0][2]);
    glm::vec3 col_1 = glm::vec3(world_mat[1][0], world_mat[1][1], world_mat[1][2]);
    glm::vec3 col_2 = glm::vec3(world_mat[2][0], world_mat[2][1], world_mat[2][2]);
    glm::vec3 scale = glm::vec3(glm::length(col_0), glm::length(col_1), glm::length(col_2));
    if (scale.x == 0 || scale.y == 0 || scale.z == 0) {
        std::cerr << "Error : Scale is 0, cannot get rotation" << std::endl;
        return glm::mat4(1.0f);
    }
    col_0 = (1 / scale.x) * col_0;
    col_1 = (1 / scale.y) * col_1;
    col_2 = (1 / scale.z) * col_2;

    return glm::mat4(glm::mat3(col_0, col_1, col_2));
}

// Helper function for easily getting scale matrix
glm::mat4 Transform::get_scale_mat() {
    return glm::scale(glm::mat4(1.f), get_scale());
}

// Get basis vectors from world matrix
glm::vec3 Transform::get_forward() {
    return glm::normalize(glm::vec3(world_mat[2][0], world_mat[2][1], world_mat[2][2]));
}
glm::vec3 Transform::get_up() {
    return glm::normalize(glm::vec3(world_mat[1][0], world_mat[1][1], world_mat[1][2]));
}
glm::vec3 Transform::get_right() {
    return glm::normalize(glm::vec3(world_mat[0][0], world_mat[0][1], world_mat[0][2]));
}