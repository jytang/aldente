#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "scene_node.h"
#include "shaders/shader.h"

class SceneGroup :
	public SceneNode
{
public:
	std::vector<SceneNode *> children;
	SceneGroup();
	SceneGroup(Scene *);
	~SceneGroup();
	void add_child(SceneNode *node);
	void remove_all();
	virtual void draw(glm::mat4 m);
	virtual void update();
	virtual void pass(glm::mat4 m, Shader *s);
};

