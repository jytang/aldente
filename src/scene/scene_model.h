#pragma once
#include "scene_node.h"
#include "mesh.h"

#include <vector>

#include "scene.h"
#include "shaders/shader.h"

class SceneModel :
	public SceneNode
{
public:
	std::vector<Mesh*> meshes;
	glm::mat4 model_mat;

	SceneModel(Scene *);
	SceneModel() {};
	~SceneModel();
	void add_mesh(Mesh* m);
	void draw(glm::mat4);
	void update();
	void combine_meshes();
	void pass(glm::mat4 m, Shader *s);

	void setScene(Scene* scene);
};

