#include "scene_trans_anim.h"
#include <GLFW/glfw3.h>
#include <cfloat>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "util/util.h"

SceneTransAnim::SceneTransAnim(Scene * scene, glm::vec3 start, glm::vec3 dir, bool play) :
    start(start), dir(dir), play(play)
{
    this->scene = scene;
    curr_percent = 0.f; // runs til 100
    prev_time = glfwGetTime();
    transformation = glm::mat4(1.f);
}

SceneTransAnim::SceneTransAnim()
{
}

SceneTransAnim::~SceneTransAnim()
{
}

void SceneTransAnim::play_anim()
{
    play = true;
    curr_percent = 0.f;
    prev_time = glfwGetTime();
}

void SceneTransAnim::reset()
{
    transformation = glm::mat4(1.f);
    play = false;
    curr_percent = 0.f;
}

void SceneTransAnim::draw(glm::mat4 m)
{
    if (play && curr_percent <= 100.f)
    {
        // animation step every 1/60th of a second
        double curr_time = glfwGetTime();
        if ((curr_time - prev_time) > 1.f / 60.f)
        {
            transformation = glm::translate(glm::mat4(1.f), dir*curr_percent);
            curr_percent += 0.1f;
            prev_time = curr_time;
        }
    }

    glm::mat4 new_matrix = m * transformation;
    SceneGroup::draw(new_matrix);
}

void SceneTransAnim::pass(glm::mat4 m, Shader *s)
{
    glm::mat4 new_mat = m * transformation;
    SceneGroup::pass(new_mat, s);
}
