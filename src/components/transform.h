#pragma once

#include <glm/glm.hpp>

/*
    Handles all matrix transformations for a game object. Gives a (hopefully) simple
    interface for interacting with the game object's transformation, including
    translations, rotations, and scaling.
*/
class Transform {
public:
    Transform();

    void set_world_mat(glm::mat4 mat);

    void set_position(glm::vec3 position);
    void set_position(float x, float y, float z);
    void set_rotation(glm::vec3 angles);
    void set_rotation(float x, float y, float z);
    void set_scale(glm::vec3 scale);
    void set_scale(float x, float y, float z);

    void translate(glm::vec3 position);
    void translate(float x, float y, float z);
    void rotate(glm::vec3 angles, bool local = true);
    void rotate(float x, float y, float z, bool local = true);

    // Makes the model face a direction
    void look_at(glm::vec3 dir);

    glm::mat4 get_world_mat();
    glm::vec3 get_position();
    glm::vec3 get_scale();

    glm::vec3 get_forward();
    glm::vec3 get_up();
    glm::vec3 get_right();

private:
    glm::mat4 world_mat;

    glm::mat4 get_translation_mat();
    glm::mat4 get_rotation_mat();
    glm::mat4 get_scale_mat();
};