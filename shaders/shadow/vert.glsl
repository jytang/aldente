#version 330 core
layout (location = 0) in vec3 position;
layout (location = 5) in mat4 instance_matrix; // 3-4 taken up by animation stuff

uniform mat4 model;
uniform mat4 view_projection;
uniform mat4 mesh_model;

void main()
{
    gl_Position = view_projection * instance_matrix * model * mesh_model * vec4(position, 1.0f);
}
