/**
* Text rendering adapted from: https://learnopengl.com/#!In-Practice/Text-Rendering
*/

#include "render2d.h"

#include <iostream>

#include "events.h"

Render2D::Render2D() {
    // Setup callbacks for window size.
    events::window_buffer_resize_event.connect([&](events::WindowSizeData d) {
        screen_width = d.width;
        screen_height = d.height;
        // Calculate projection matrix.
        projection = glm::ortho(0.0f,
                    static_cast<GLfloat>(d.width),
                    0.0f,
                    static_cast<GLfloat>(d.height));
    });

    // Generate textures for all glyphs.
    setup_glyphs();

    // Configure VAO/VBO for texture quads.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Render2D::setup_glyphs() {
    FT_Library ft;
    assert(!FT_Init_FreeType(&ft));

    // Load font ("face")
    FT_Face face;
    assert(!FT_New_Face(ft, "assets/fonts/kongtext.ttf", 0, &face));

    // Width and height params. Width is 0 for dynamic resizing.
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Set active glyph to a character
    // FT_LOAD_RENDER flag creates 8-bit grayscale bitmap image (access by face->glyph->bitmap)
    assert(!FT_Load_Char(face, 'X', FT_LOAD_RENDER));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (GLubyte c = 32; c < 128; c++) {
        // Load character glyph
        assert(!FT_Load_Char(face, c, FT_LOAD_RENDER));
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint) face->glyph->advance.x
        };
        characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Render2D::render_text(std::string text,
                           GLfloat x, GLfloat y,
                           GLfloat x_scale, GLfloat y_scale,
                           Color color, GLfloat alpha) {
    // Always render UI regardless of depth.
    glDisable(GL_DEPTH_TEST);

    // Activate corresponding render state
    ShaderManager::text.use();
    ShaderManager::text.set_uni("baseColor", color.to_vec());
    ShaderManager::text.set_uni("alpha", alpha);
    glUniformMatrix4fv(ShaderManager::text.get_uni("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.bearing.x * x_scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * y_scale;

        GLfloat w = ch.size.x * x_scale;
        GLfloat h = ch.size.y * y_scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_ID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * x_scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
}

void Render2D::render_rect(GLfloat x, GLfloat y,
                           GLfloat width, GLfloat height,
                           Color color, GLuint texture_ID,
                           GLfloat alpha) {
    // Always render UI regardless of depth.
    glDisable(GL_DEPTH_TEST);

    // Send uniforms to the text shader
    ShaderManager::ui.use();

    ShaderManager::ui.set_uni("baseColor", color.to_vec());
    ShaderManager::ui.set_uni("projection", projection);
    ShaderManager::ui.set_uni("alpha", alpha);

    if (texture_ID == 0)
        ShaderManager::ui.set_uni("hasTexture", false);
    else
        ShaderManager::ui.set_uni("hasTexture", true);

    // Set active arrays
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_ID);

    // Update VBO for this rect
    GLfloat vertices[6][4] = {
        { x,         y + height,   0.0, 0.0 },
        { x,         y,            0.0, 1.0 },
        { x + width, y,            1.0, 1.0 },

        { x,         y + height,   0.0, 0.0 },
        { x + width, y,            1.0, 1.0 },
        { x + width, y + height,   1.0, 0.0 }
    };
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
}

// Render rectangle in terms of percentages of screen width and height.
void Render2D::render_rectP(GLfloat x, GLfloat y,
                            GLfloat width, GLfloat height,
                            Color color, GLuint texture_ID,
                            GLfloat alpha) {
    GLfloat adj_x      = x      * UNIT_TO_PERCENT * screen_height;
    GLfloat adj_y      = y      * UNIT_TO_PERCENT * screen_height;
    GLfloat adj_width  = width  * UNIT_TO_PERCENT * screen_height;
    GLfloat adj_height = height * UNIT_TO_PERCENT * screen_height;
    render_rect(adj_x, adj_y, adj_width, adj_height, color, texture_ID, alpha);
}

// Render text in terms of percentages of screen width and height.
void Render2D::render_textP(std::string text,
                            GLfloat x, GLfloat y,
                            GLfloat x_scale, GLfloat y_scale,
                            Color color, GLfloat alpha) {
    GLfloat adj_x        = x        * UNIT_TO_PERCENT * screen_height;
    GLfloat adj_y        = y        * UNIT_TO_PERCENT * screen_height;
    GLfloat adj_x_scale  = x_scale  * UNIT_TO_PERCENT * GLYPH_UNIT * screen_height;
    GLfloat adj_y_scale  = y_scale  * UNIT_TO_PERCENT * GLYPH_UNIT * screen_height;
    render_text(text, adj_x, adj_y, adj_x_scale, adj_y_scale, color, alpha);
}
