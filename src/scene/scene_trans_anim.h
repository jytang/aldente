#pragma once
#include "scene_group.h"
class SceneTransAnim :
	public SceneGroup
{
private:
	glm::vec3 start, dir;
	GLfloat curr_percent;
	bool play;
	double prev_time;
	glm::mat4 transformation;
public:
	SceneTransAnim(Scene * scene, glm::vec3 start, glm::vec3 dir, bool play);
	SceneTransAnim();
	~SceneTransAnim();
	void play_anim();
	void reset();
	void draw(glm::mat4 m);
	void pass(glm::mat4 m, Shader *s);
};

