#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in ivec4 bone_ids;
layout (location = 4) in vec4 weights;

out vec3 frag_pos;
out vec3 frag_normal;
out vec4 frag_pos_light;
out vec2 frag_tex_coord;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_matrix;
uniform mat4 mesh_model;
uniform mat4 bones[MAX_BONES];

void main()
{
    vec4 new_pos = vec4(position, 1.0);
    vec4 new_norm = vec4(normal, 0.0);

    bool has_bones = weights[0] + weights[1] + weights[2] + weights[3] > 0;
    //bool has_bones = dot(weights, vec4(1)) > 0;

    if (has_bones){
        // Handles all bone transformations if has bone weights
        // Any vertex can only be influenced by up to four bones
        mat4 bone_trans = bones[bone_ids[0]] * weights[0];
        bone_trans += bones[bone_ids[1]] * weights[1];
        bone_trans += bones[bone_ids[2]] * weights[2];
        bone_trans += bones[bone_ids[3]] * weights[3];

        // Currently need inverse due to bug with mesh_model being multipled twice in bone
        new_pos = bone_trans * new_pos;
        new_norm = bone_trans * new_norm;
    }

    gl_Position = projection * view * model * mesh_model * new_pos;
    frag_pos = vec3(model * mesh_model * vec4(position, 1.0f));
    frag_normal = mat3(transpose(inverse(model * mesh_model))) * vec3(new_norm);
    frag_pos_light = light_matrix * vec4(frag_pos, 1.0);
    frag_tex_coord = vec2(tex_coord.x, 1.0 - tex_coord.y); //y-axis usually requires inverting
}
