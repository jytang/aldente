#pragma once

#include <glm/glm.hpp>

class Color {
private:
    float r, g, b; // red green blue color values (vary from 0.f - 1.f)
public:
    Color(float red = 1.f, float green = 1.f, float blue = 1.f); // Default color is white (1.f, 1.f, 1.f)
    Color(glm::vec3 v);
    void set(float red, float green, float blue); // set this color
    void set(glm::vec3 v);
    void add(const Color other); // add another color
    void add_scaled(const Color other, float factor); // add a color that is scaled before added
    void scale(float factor); // scale this color by a factor
    void set_scaled(const Color other, float factor); // set this color to another color * scale factor
    void multiply(const Color other); // multiply another color into this one
    void divide(const Color other); // divide with another color
    glm::vec3 to_vec(); // convert to a glm::vec3 for GL purposes

    static Color WHITE, BLACK;
    static Color RED, GREEN, BLUE;
    static Color CYAN, YELLOW, MAGENTA;
    static Color OCEAN_BLUE, WINDWAKER_GREEN, WINDWAKER_SAND;
    static Color AUTUMN_ORANGE, OLIVE_GREEN, PURPLE, BONE_WHITE, INDIAN_RED;
    static Color BROWN, WOOD_TAN, WOOD_TAN_LIGHT, WOOD_SIENNA, WOOD_SADDLE;
    static Color LOZ_DARK_GREEN, LOZ_GREEN, LOZ_LIGHT_GREEN;
    static Color GOLD, SILVER, BRONZE;
};
