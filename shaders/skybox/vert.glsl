#version 330 core
layout (location = 0) in vec3 position;

out vec3 tex_coords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 rot;

void main()
{
    vec4 pos = projection * view * vec4(position, 1.0f);
    gl_Position = pos.xyww; // make z component always 1 (furthest away)
    tex_coords = vec3(rot * vec4(position, 1.0f));
}
