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
    ModelFilter model_filter; // For passing parameters from game object down to mesh level
    AnimationPlayer anim_player;
    btRigidBody *rigidbody;

    int id;
    bool enabled;

    float height_offset;
public:
    static std::unordered_map<int, GameObject*> game_objects;
    std::vector<GameObject *> children;

    Transform transform; // World matrix now controlled using the Transform Component
    glm::vec3 direction; // Direction the object is facing
    std::string tag; // Identify this GameObject by a human-readable tag.
    bool notify_on_collision = false; // Physics engine will only call on_collision if this flag is set.

    GameObject();
    GameObject(int id);
    ~GameObject();

    // Parenting Stuff
    void add_child(GameObject *obj);
    void remove_child(GameObject *obj);
    void remove_all();

    // Server-Specific Functions
    void s_update(); // Updates this object and all children
    virtual void s_update_this() {}; // Update function for this particular object. Use this instead of update()
    virtual void s_on_collision(GameObject *other) {} // Server: physics has detected a collision with this and other.
    virtual void s_interact_trigger(GameObject *other) {}; // Callback for when a player presses A on this object. Server update.

    // Client-Specific Functions
    virtual void c_draw(Shader *shader, SceneInfo &scene_info);
    virtual void c_draw_instanced(Shader *shader, SceneInfo &scene_info,
                                std::vector<glm::mat4> instance_matrix = std::vector<glm::mat4>());
    virtual void c_update_state(float x, float z, float wx, float wz, bool enab); // Updates this object's position and orientation on client side.
    virtual void c_on_collision() {} // Client: this obj has collided, perform graphical update.
    virtual void c_interact_trigger() {}; // Callback for when a player presses A on this object. Graphical update.

    // Model setup and related functions
    virtual void setup_model() {};
    virtual void setup_instanced_model(int num_instances,
                                       std::vector<glm::mat4> instance_matrix = std::vector<glm::mat4>()) {};

    Model* get_model() { return model; };
    void attach_model(Model *m);

    // Changing game obejcts material
    void set_filter_color(Color color); // Filter is to apply temporarily to entire object
    void set_filter_alpha(float alpha);
    void set_filter_shadows(bool enable);
    void set_color(Color color);    // These are to apply permanently to entire object
    void set_alpha(float alpha);
    void set_shadows(bool enable);

    btRigidBody *get_rigid() { return rigidbody; };
    void set_rigid(btRigidBody *to_add) { rigidbody = to_add; };

    int get_id() { return id; };

    // Sets both transform and rigid body position at the same time
    void set_position(glm::vec3 pos);

    void disable();
    void enable();
    bool is_enabled() { return enabled; };
};
