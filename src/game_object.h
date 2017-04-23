#pragma once

#include "scene/scene_node.h"
#include "model/model.h"

/*
	This represents an abstract object in the scene.
	Contains all the essential components required to
	render the model in the scene.
*/
class GameObject : public SceneNode {
private:
    Model *model;

public:
    GameObject(Scene *scene);

    virtual ~GameObject();

    void attach_model(Model *m);

    virtual void draw();

    virtual void update();

    virtual void pass(Shader *s);
};

