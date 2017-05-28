#version 330 core

out vec4 color;

in vec2 tex_coords;

uniform sampler2D depth_map;
uniform bool debug_shadow;

void main()
{
    vec3 color_value = texture(depth_map, tex_coords).rgb;

    if (debug_shadow)
        color = vec4(vec3(color_value.r), 1.0);
    else
        color = vec4(color_value, 1.0);
}