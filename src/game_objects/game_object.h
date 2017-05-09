#pragma once

#include "scene/scene_info.h"
#include "model/model.h"
#include "transform.h"
#include "shaders/shader.h"
#include "animation/animation_player.h"
#include "btBulletDynamicsCommon.h"
#include "util/color.h"

#include <string>

/*
    This represents an abstract object in the scene.
    Contains all the essential components required to
    render the model in the scene.
*/
class GameObject {
private:
    static int id_counter;
protected:
    Model *model;
    AnimationPlayer anim_player;
    btRigidBody *rigidbody;
    int id;
public:
    Transform transform; // World matrix now controlled using the Transform Component
    std::string tag; // Identify this GameObject by a human-readable tag.
    bool notify_on_collision = false; // Physics engine will only call on_collision if this flag is set.

    GameObject();
    
    virtual void draw(Shader *shader, SceneInfo &scene_info);
    virtual void update();
    virtual void on_collision(GameObject *other) {}
    virtual void setup_model() = 0;

    Model* get_model() { return model; };
    void attach_model(Model *m);
    void set_color(Color color);
    void set_rigid(btRigidBody *to_add) { rigidbody = to_add; };
    btRigidBody *get_rigid() { return rigidbody; };
    int get_id() { return id; };
    void set_id(int to_set) { id = to_set; };
};

