#pragma once

#include "scene/scene_info.h"
#include "model/model.h"
#include "transform.h"
#include "shaders/shader.h"
#include "animation/animation_player.h"
#include "btBulletDynamicsCommon.h"

#include <string>
#include <iostream>

/*
    This represents an abstract object in the scene.
    Contains all the essential components required to
    render the model in the scene.
*/
class GameObject {
protected:
    Model *model;
    AnimationPlayer anim_player;
    btRigidBody *rigidbody;
public:
    Transform transform; // World matrix now controlled using the Transform Component
    std::string tag;

    GameObject();
    
    virtual void draw(Shader *shader, SceneInfo &scene_info);
    virtual void update();
    virtual void on_collision(GameObject *other) { std::cerr << tag << " collided with: " << other->tag << std::endl; }

    Model* get_model() { return model; };
    void attach_model(Model *m);
    void set_color(glm::vec3 color);   
    void set_rigid(btRigidBody *to_add) { rigidbody = to_add; };
    btRigidBody *get_rigid() { return rigidbody; };
};

