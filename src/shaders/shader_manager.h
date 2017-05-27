#pragma once

#include "shader.h"
#include "basic_shader.h"
#include "skybox_shader.h"
#include "shadow_shader.h"
#include "text_shader.h"
#include "debug_shadow_shader.h"
#include "ui_shader.h"
#include "hdr_shader.h"
#include "blur_shader.h"

class ShaderManager {
public:
    // Single static instances of each shader
    static BasicShader basic;
    static SkyboxShader skybox;
    static ShadowShader shadow;
    static TextShader text;
    static DebugShadowShader debug_shadow;
    static UIShader ui;
    static BasicShader unlit;
    static BasicShader anim_basic;
    static BasicShader anim_unlit;
    static HDRShader hdr;
    static BlurShader blur;

    // Utility shader function to load and compile shader program from file.
    // Returns program ID assigned to the shader program.
    static GLuint create_shader_program(std::string type);

    // Compiles and initializes all the static shader instances.
    static void init() {
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
        ui.set_id(create_shader_program("ui"));
        ui.init();
        unlit.set_id(create_shader_program("unlit"));
        unlit.init();
        anim_basic.set_id(create_shader_program("anim_basic"));
        anim_basic.init();
        anim_unlit.set_id(create_shader_program("anim_unlit"));
        anim_unlit.init();
        hdr.set_id(create_shader_program("hdr"));
        hdr.init();
        blur.set_id(create_shader_program("blur"));
        blur.init();
    }
};
