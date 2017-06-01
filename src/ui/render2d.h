#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

#include "shaders/shader.h"
#include "util/color.h"

#define UNIT_TO_PERCENT 0.01f
#define GLYPH_UNIT      0.1f

class Render2D {
    struct Character {
        GLuint     texture_ID; // ID handle of the glyph texture
        glm::ivec2 size;       // Size of glyph
        glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
        GLuint     advance;    // Offset to advance to next glyph
    };

public:
    Render2D();
    void render_textP(std::string text, GLfloat x, GLfloat y, GLfloat x_scale, GLfloat y_scale, Color color, GLfloat alpha);
    void render_rectP(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Color color, GLuint texture_ID, GLfloat alpha);
private:
    void setup_glyphs();
    void render_text(std::string text, GLfloat x, GLfloat y, GLfloat x_scale, GLfloat y_scale, Color color, GLfloat alpha);
    void render_rect(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Color color, GLuint texture_ID, GLfloat alpha);

    std::map<GLchar, Character> characters;
    GLuint VAO, VBO;
    glm::mat4 projection;
    int screen_width, screen_height;
};
