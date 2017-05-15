#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 5) in mat4 instance_matrix; // 3-4 taken up by animation stuff

out vec3 frag_pos;
out vec3 frag_normal;
out vec4 frag_pos_light;
out vec2 frag_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_matrix;
uniform mat4 mesh_model;

void main()
{    
    gl_Position = projection * view * instance_matrix * model * mesh_model * vec4(position, 1.0);
    frag_pos = vec3(instance_matrix * model * mesh_model * vec4(position, 1.0f));
    frag_normal = mat3(transpose(inverse(instance_matrix * model * mesh_model))) * normal;
    frag_pos_light = light_matrix * vec4(frag_pos, 1.0);
    frag_tex_coord = vec2(tex_coord.x, 1.0 - tex_coord.y); //y-axis usually requires inverting
}
