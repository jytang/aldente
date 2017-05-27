#include "hdr_shader.h"

#include "events.h"

void HDRShader::init() {
    /* GENERATE FRAMEBUFFERS, RENDERBUFFERS, AND TEXTURES */
    // Set up floating point framebuffer to render scene to
    glGenFramebuffers(1, &FBO);

    // Create floating point color buffer
    glGenTextures(2, color_buffers);

    // Create depth buffer (renderbuffer)
    glGenRenderbuffers(1, &rbo_depth);

    // Bind buffers.
    bind_buffers();

    /* SET UP FRAMEBUFFER OBJECTS */
    // Attach buffers to HDR FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    for (int i = 0; i < 2; ++i)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_buffers[i], 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);

    // Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    // Finalize framebuffer stuff.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "HDR framebuffer status is incomplete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* SETUP QUAD */
    GLfloat quad_vertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));

    /* SETUP EVENT LISTENERS */
    // Setup listeners to adapt to screen width and height changing.
    events::window_buffer_resize_event.connect([&](events::WindowSizeData d) {
        screen_width = d.width;
        screen_height = d.height;
        bind_buffers();
    });

    // Debug hdr uniforms
    events::debug::increase_hdr_exposure_event.connect([&]() {
        hdr_exposure += 0.1f;
    });
    events::debug::decrease_hdr_exposure_event.connect([&]() {
        hdr_exposure -= 0.1f;
    });
    events::debug::toggle_hdr_event.connect([&]() {
       hdr_on = !hdr_on;
    });
    events::debug::toggle_bloom_event.connect([&]() {
       bloom_on = !bloom_on;
    });
}

void HDRShader::bind_buffers() {
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height);
}

void HDRShader::pre_draw(SceneInfo &scene_info) {
    // Nothing to be done. For now.
}

void HDRShader::post_draw() {
    // Nothing to be done. For now.
}

void HDRShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    // Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_buffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ShaderManager::blur.pingpong_color_buffers[0]);

    set_uni("hdr", hdr_on);
    set_uni("bloom", bloom_on);
    set_uni("exposure", hdr_exposure);

    // Draw quad.
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}