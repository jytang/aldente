#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "scene_group.h"
#include "shaders/shader.h"

class SceneTransform :
	public SceneGroup
{
public:
	glm::mat4 transformation;

	SceneTransform();
	SceneTransform(Scene *, glm::mat4 m);
	~SceneTransform();
	void draw(glm::mat4 m);
	void update();
	void pass(glm::mat4 m, Shader *s);
};

