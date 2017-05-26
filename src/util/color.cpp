#include "color.h"

Color Color::WHITE              (1.f, 1.f, 1.f);
Color Color::BLACK              (0.f, 0.f, 0.f);
Color Color::RED                (1.f, 0.f, 0.f);
Color Color::GREEN              (0.f, 1.f, 0.f);
Color Color::BLUE               (0.f, 0.f, 1.f);
Color Color::CYAN               (0.f, 1.f, 1.f);
Color Color::YELLOW             (1.f, 1.f, 0.f);
Color Color::MAGENTA            (1.f, 0.f, 1.f);

Color Color::OCEAN_BLUE         (80, 186, 244);
Color Color::WINDWAKER_GREEN    (156, 217, 84);
Color Color::WINDWAKER_SAND     (251, 250, 158);

Color Color::AUTUMN_ORANGE      (214, 116, 24);
Color Color::OLIVE_GREEN        (85, 107, 47);
Color Color::PURPLE             (107, 47, 85);
Color Color::BONE_WHITE         (204, 208, 210);
Color Color::INDIAN_RED         (205, 92, 92);

Color Color::BROWN              (114, 75, 49);
Color Color::WOOD_TAN           (139, 90, 43);
Color Color::WOOD_TAN_LIGHT     (255, 165, 79);
Color Color::WOOD_SIENNA        (160, 82, 45);
Color Color::WOOD_SADDLE        (139, 69, 19);

Color Color::LOZ_DARK_GREEN     (14, 81, 53);
Color Color::LOZ_GREEN          (13, 146, 99);
Color Color::LOZ_LIGHT_GREEN    (74, 186, 145);

Color Color::GOLD               (255, 215, 10);
Color Color::SILVER             (192, 192, 192);
Color Color::BRONZE             (205, 127, 50);

Color::Color(float red, float green, float blue) {
    r = red;
    g = green;
    b = blue;
    if (r > 1.f || g > 1.f || b > 1.f) { // RGB format where values out of 255
        r /= 255.f;
        g /= 255.f;
        b /= 255.f;
    }
}

Color::Color(glm::vec3 v) {
    r = v.x;
    g = v.y;
    b = v.z;
    if (r > 1.f || g > 1.f || b > 1.f) { // RGB format where values out of 255
        r /= 255.f;
        g /= 255.f;
        b /= 255.f;
    }
}

void Color::set(float red, float green, float blue) {
    r = red;
    g = green;
    b = blue;
}

void Color::set(glm::vec3 v) {
    r = v.x;
    g = v.y;
    b = v.z;
}

void Color::add(const Color other) {
    r += other.r;
    g += other.g;
    b += other.b;
}

void Color::add_scaled(const Color other, float factor) {
    r += factor * other.r;
    g += factor * other.g;
    b += factor * other.b;
}

void Color::scale(float factor) {
    r *= factor;
    g *= factor;
    b *= factor;
}

void Color::set_scaled(const Color other, float factor) {
    r = factor * other.r;
    g = factor * other.g;
    b = factor * other.b;
}

void Color::multiply(const Color other) {
    r *= other.r;
    g *= other.g;
    b *= other.b;
}

void Color::divide(const Color other) {
    if (other.r == 0 || other.g == 0 || other.b == 0)
        return;

    r = r / other.r;
    g = g / other.g;
    b = b / other.b;
}

glm::vec3 Color::to_vec() {
    return { r, g, b };
}