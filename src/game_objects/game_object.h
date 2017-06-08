#pragma once

#include "scene/scene_info.h"
#include "components/transform.h"
#include "shaders/shader.h"
#include "animation/animation_player.h"
#include "btBulletDynamicsCommon.h"
#include "util/color.h"
#include "model/model_filter.h"
#include "collision_groups.h"

#include <unordered_map>
#include <string>

#define CLIENT_ONLY_ID -1
#define REQUIRE_ID 0
#define STARTING_ID 1
#define ON_SERVER 0

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
    bool use_rb_transform;

    float height_offset;
public:
    static std::unordered_map<int, GameObject*> game_objects;
    std::vector<GameObject *> children;

    Transform transform; // World matrix now controlled using the Transform Component
    Transform initial_transform; // Initial transform to reset to at beginning of build phase
    std::string tag; // Identify this GameObject by a human-readable tag.
    bool notify_on_collision = false; // Physics engine will only call on_collision if this flag is set.

    short collision_group = COLLISION_DEFAULT; // Group is belongs to
    short collision_mask = 0; // collides with everything by default.
                              // OR (| operator) with groups to not collide with them

    GameObject(int id = 0);
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
    void c_update(); // Updates this object and all children
    virtual void c_update_this() {}; // Update function for this particular object. Use this instead of update()
    virtual void c_update_state(glm::mat4 mat, bool enab); // Updates this object's world matrix on client side.
    virtual void c_on_collision(GameObject *other) {} // Client: this obj has collided, perform graphical update.
    virtual void c_interact_trigger(GameObject *other) {}; // Callback for when a player presses A on this object. Graphical update.

    // Model setup and related functions
    virtual void setup_model() {};
    virtual void setup_instanced_model(int num_instances,
                                       std::vector<glm::mat4> instance_matrix = std::vector<glm::mat4>()) {};

    // Resets to Initial Parameters
    virtual void s_reset() {};
    virtual void c_reset() {};

    // Model Stuff
    Model* get_model() { return model; };
    void attach_model(Model *m);

    // Changing game obejcts material
    void disable_filter();  // Turns off any filter
    void set_filter_color(Color color); // Filter is to apply temporarily to entire object
    void set_filter_alpha(float alpha);
    void set_filter_shadows(bool enable);
    void set_color(Color color);    // These are to apply permanently to entire object
    void set_alpha(float alpha);
    void set_shadows(bool enable);

    // Rigid Body
    btRigidBody *get_rigid() { return rigidbody; };
    void set_rigid(btRigidBody *to_add) { rigidbody = to_add; };

    int get_id() { return id; };

    // Sets both transform and rigid body position at the same time
    void set_position(glm::vec3 pos);

    // Sets both initial scale and transform's scale at the same time
    void set_scale(glm::vec3 scale);

    // Sets only rigidbody's position
    void set_rb_transform();

    // Updates Transform using Rigid Body's Position
    void sync_position();

    // Resets transform to use Initial Transform
    void reset_to_initial_transform();

    void set_ghost(bool b);
    
    void disable();
    void enable();
    void disable_all();
    void enable_all();

    bool is_enabled() { return enabled; };

    // Set's initial transform's rotation
    void set_initial_direction(glm::vec3);
};
