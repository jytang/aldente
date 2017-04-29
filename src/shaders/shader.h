#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "scene/scene_info.h"
#include "model/mesh.h"
#include "basic_shader.h"
#include "skybox_shader.h"
#include "shadow_shader.h"
#include "text_shader.h"
#include "debug_shadow_shader.h"

class Shader {
private:
    GLuint shader_id;
public:
    // Single static instances of each shader
    static BasicShader basic;
    static SkyboxShader skybox;
    static ShadowShader shadow;
    static TextShader text;
    static DebugShadowShader debug_shadow;

    // Compiles and initializes all the static shader instances.
    static void init_all() {
        basic.set_id(create_shader_program("basic"));
        basic.init();
        shadow.set_id(create_shader_program("shadow"));
        shadow.init();
        skybox.set_id(create_shader_program("skybox"));
        skybox.init();
        text.set_id(create_shader_program("text"));
        text.init();
        debug_shadow.set_id(create_shader_program("debug_shadow"));
        debug_shadow.init();
    }

    // Utility shader function to load and compile shader program from file.
    // Returns program ID assigned to the shader program.
    static GLuint create_shader_program(const char *type);

    void set_id(GLuint id) { shader_id = id; }
    GLuint get_id() { return shader_id; }

    // Returns location of the uniform variable
    GLint get_uni(const char *uniform) { return glGetUniformLocation(shader_id, uniform); }

    // Convenience for passing glm::vec3 as uniforms.
    void uni_vec3(const char *uniform, glm::vec3 v) { glUniform3f(get_uni(uniform), v.x, v.y, v.z); }

    // Set GL state to use this shader program.
    void use() { glUseProgram(shader_id); }

    // Initialization of shader state. Allocation of buffers and buffer object init done here.
    virtual void init() = 0;

    // Called before all the draw calls.
    // Setup shader/GL state, and shader state shared between draw calls.
    virtual void pre_draw(SceneInfo &scene_info) = 0;

    // Cleanup for GL/shader state.
    virtual void post_draw() = 0;

    // Draws the mesh's geometry with its material, transformed into world space.
    virtual void draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) = 0;
};
