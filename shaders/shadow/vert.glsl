#version 330 core
layout (location = 0) in vec3 position;
layout (location = 3) in ivec4 bone_ids;
layout (location = 4) in vec4 weights;
layout (location = 5) in mat4 instance_matrix; // 3-4 taken up by animation stuff

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view_projection;
uniform mat4 mesh_model;
uniform mat4 bones[MAX_BONES];
uniform bool has_bones;

void main()
{
    vec4 new_pos = vec4(position, 1.0);

    if (has_bones){
        // Handles all bone transformations if has bone weights
        // Any vertex can only be influenced by up to four bones
        mat4 bone_trans = bones[bone_ids[0]] * weights[0];
        bone_trans += bones[bone_ids[1]] * weights[1];
        bone_trans += bones[bone_ids[2]] * weights[2];
        bone_trans += bones[bone_ids[3]] * weights[3];

        new_pos = bone_trans * new_pos;
    }

    gl_Position = view_projection * instance_matrix * model * mesh_model * new_pos;
}
