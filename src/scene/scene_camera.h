#pragma once

#include "scene_node.h"
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/shader.h"

class SceneCamera :
	public SceneNode
{
private:
	const glm::vec3 DEFAULT_CAM_POS = glm::vec3(0.0f, 0.0f, 20.0f);
	const glm::vec3 DEFAULT_CAM_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 DEFAULT_CAM_UP = glm::vec3(0.0f, 1.0f, 0.0f);

public:
	glm::vec3 cam_pos;
	glm::vec3 cam_front;
	glm::vec3 cam_up;
	glm::mat4 V;
	GLfloat pitch;
	GLfloat yaw = -90.f;

	SceneCamera(Scene *);
	~SceneCamera();
	void draw(glm::mat4 m);
	void pass(glm::mat4 m, Shader *s);
	void update();
	void recalculate();
	void reset();
};

