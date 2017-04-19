#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "scene_camera.h"
#include "scene_group.h"
#include "shaders/shader.h"
#include "geometry/plane.h"
#include "scene_trans_anim.h"
#include "bounding_sphere.h"

class Scene
{
public:
	SceneGroup *root;
	SceneCamera *camera;
	glm::mat4 P;
	glm::vec3 light_pos;
	Plane frustum_planes[6];
	glm::vec3 frustum_corners[8];
	std::vector< std::vector<GLfloat> > height_map;
	std::vector<BoundingSphere *> interactable_objects;

	// portals
	SceneGroup* in_house;
	SceneTransAnim* out_house;
	float in_height, out_height;
	glm::vec2 in_area[2];
	glm::vec2 in_point, out_point;

	Scene();
	~Scene();
	void render();
	void pass(Shader * s);
	void update_frustum_corners(int width, int height, GLfloat);
	void update_frustum_planes();
	glm::mat4 frustum_ortho();
	void displace_cam(glm::vec3 displacement);

	virtual void setup() {}
	virtual GLfloat get_size() { return 0; }
};