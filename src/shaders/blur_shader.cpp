#include "blur_shader.h"

#include "events.h"

#define GAUSSIAN_ITERATIONS 10

void BlurShader::init() {
    /* GENERATE FRAMEBUFFERS, RENDERBUFFERS, AND TEXTURES */
    // Ping pong framebuffer for blurring.
    glGenFramebuffers(2, pingpong_FBO);
    glGenTextures(2, pingpong_color_buffers);

    // Bind buffers.
    bind_buffers();

    /* SET UP FRAMEBUFFER OBJECTS */
    // Attach buffers to blur (ping pong) FBO
    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpong_FBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpong_color_buffers[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Blur framebuffer status is incomplete!" << std::endl;
    }
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
}

void BlurShader::bind_buffers() {
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, pingpong_color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

void BlurShader::pre_draw(SceneInfo &scene_info) {
    // Nothing to be done. For now.
}

void BlurShader::post_draw() {
    // Nothing to be done. For now.
}

void BlurShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    // Blur bright fragments w/ two-pass Gaussian Blur
    bool horizontal = true;
    for (int i = 0; i < GAUSSIAN_ITERATIONS; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpong_FBO[horizontal]);
        set_uni("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, i == 0 ?  // bind texture of other framebuffer (or scene if first iteration)
                                     ShaderManager::hdr.color_buffers[1] :
                                     pingpong_color_buffers[!horizontal]);
        // Draw quad.
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        // Ping pong to vertical
        horizontal = !horizontal;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}