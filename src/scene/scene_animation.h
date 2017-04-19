#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "scene_group.h"
#include "shaders/shader.h"

class SceneAnimation :
	public SceneGroup
{
private:
	GLfloat min, max, curr, step;
	glm::vec3 axis, pivot;
	double prev_time;
	glm::mat4 transformation;
public:
	SceneAnimation(Scene * scene, GLfloat min, GLfloat max, GLfloat start, GLfloat step, glm::vec3 axis, glm::vec3 pivot);
	SceneAnimation();
	~SceneAnimation();
	void draw(glm::mat4 m);
	void pass(glm::mat4 m, Shader *s);
};

