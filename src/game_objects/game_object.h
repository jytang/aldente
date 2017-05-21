#pragma once

#include "scene/scene_info.h"
#include "transform.h"
#include "shaders/shader.h"
#include "animation/animation_player.h"
#include "btBulletDynamicsCommon.h"
#include "util/color.h"
#include "model/model_filter.h"

#include <unordered_map>
#include <string>

/*
    This represents an abstract object in the scene.
    Contains all the essential components required to
    render the model in the scene.
*/
class GameObject {
private:
    static int id_counter;

    void connect_model_filter();
protected:
    Model *model;
    ModelFilter model_filter;
    AnimationPlayer anim_player;
    btRigidBody *rigidbody;
    int id;
    bool enabled;
public:
    static std::unordered_map<int, GameObject*> game_objects;
    std::vector<GameObject *> children;

    Transform transform; // World matrix now controlled using the Transform Component
    glm::vec3 direction;
    std::string tag; // Identify this GameObject by a human-readable tag.
    bool notify_on_collision = false; // Physics engine will only call on_collision if this flag is set.

    GameObject();
    GameObject(int id);
    ~GameObject();

    // Parenting Stuff
    void add_child(GameObject *obj);
    void remove_child(GameObject *obj);
    void remove_all();

    virtual void draw(Shader *shader, SceneInfo &scene_info);
    virtual void draw_instanced(Shader *shader, SceneInfo &scene_info,
                                std::vector<glm::mat4> instance_matrix = std::vector<glm::mat4>());
    void update(); // Updates this object and all children

    virtual void update_this() {}; // Update function for this particular object. Use this instead of update()
    virtual void update_state(float x, float z, float wx, float wz); // Updates this object's position and orientation.
    virtual void on_collision(GameObject *other) {} // Server: physics has detected a collision with this and other.
    virtual void on_collision_graphical() {} // Client: this obj has collided, perform graphical update.
    virtual void interact_trigger() {}; // Callback for when a player presses A on this object. Graphical update.
    virtual void setup_model() {};
    virtual void setup_instanced_model(int num_instances,
                                       std::vector<glm::mat4> instance_matrix = std::vector<glm::mat4>()) {};

    void set_filter_color(Color color);
    void set_filter_alpha(float alpha);
    void set_filter_shadows(bool enable);
    void set_color(Color color);
    void set_alpha(float alpha);
    void set_shadows(bool enable);

    Model* get_model() { return model; };
    void attach_model(Model *m);

    btRigidBody *get_rigid() { return rigidbody; };
    void set_rigid(btRigidBody *to_add) { rigidbody = to_add; };
    int get_id() { return id; };

    void set_position(glm::vec3 pos);

    void disable();
    void enable();
    bool is_enabled() { return enabled; };
};
