#include "scene_animation.h"
#include <GLFW/glfw3.h>
#include <cfloat>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "util/util.h"

SceneAnimation::SceneAnimation(Scene * scene, GLfloat min, GLfloat max, GLfloat start, GLfloat step, glm::vec3 axis, glm::vec3 pivot) :
    min(min), max(max), curr(start), step(step), axis(axis), pivot(pivot)
{
    this->scene = scene;
    prev_time = glfwGetTime();
}

SceneAnimation::SceneAnimation()
{
}

SceneAnimation::~SceneAnimation()
{
}

void SceneAnimation::draw(glm::mat4 m)
{
    transformation = glm::translate(glm::mat4(1.f), pivot);
    transformation = glm::rotate(glm::mat4(1.f), curr / 180.f*glm::pi<float>(), axis)
        * transformation;
    transformation = glm::translate(glm::mat4(1.f), -pivot) * transformation;

    // animation step every 1/60th of a second
    double curr_time = glfwGetTime();
    if ((curr_time - prev_time) > 1.f / 60.f)
    {
        curr += Util::random(1, 2) * step;
        // reverse step when hitting min/max
        if (curr >= max || curr <= min)
            step = -step;
        prev_time = curr_time;
    }

    glm::mat4 new_matrix = m * transformation;
    SceneGroup::draw(new_matrix);
}

void SceneAnimation::pass(glm::mat4 m, Shader *s)
{
    glm::mat4 new_mat = m * transformation;
    SceneGroup::pass(new_mat, s);
}
