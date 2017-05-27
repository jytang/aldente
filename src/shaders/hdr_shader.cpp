#include "hdr_shader.h"

#include "events.h"

void HDRShader::init() {
    // Set up floating point framebuffer to render scene to
    glGenFramebuffers(1, &FBO);

    // Create floating point color buffer
    glGenTextures(1, &color_buffer);

    // Create depth buffer (renderbuffer)
    glGenRenderbuffers(1, &rbo_depth);

    // Bind buffers.
    bind_buffers();

    // Attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "HDR framebuffer status is incomplete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup quad for render.
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

    // Setup listeners to adapt to screen width and height changing.
    events::window_buffer_resize_event.connect([&](events::WindowSizeData d) {
        screen_width = d.width;
        screen_height = d.height;
        bind_buffers();
    });
}

void HDRShader::bind_buffers() {
    glBindTexture(GL_TEXTURE_2D, color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    // Set texture to be shadow map texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_buffer);

    // Hard coded hdr always on + exposure = 1
    set_uni("hdr", true);
    set_uni("exposure", 1.f);

    // Draw quad.
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}