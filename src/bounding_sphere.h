#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "scene/scene_transform.h"

#define GRAB 0
#define GENERATE_VILLAGE 1
#define GENERATE_FOREST 2
#define GENERATE_TERRAIN 3
#define TOGGLE_SHADOWS 4
#define CHANGE_SCENE 5

class BoundingSphere
{
public:
	BoundingSphere(SceneTransform *translation_mat, float radius, int interact_type);
	~BoundingSphere();

	SceneTransform *translation_mat;
	float radius;
	int interact_type;
	bool check_interact;
};