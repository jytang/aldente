#version 330 core
struct Material {
    vec3 diffuse;
    float alpha;
};

in vec3 frag_pos;
in vec3 frag_normal;
in vec4 frag_pos_light;
in vec2 frag_tex_coord;

out vec4 color;

uniform Material material;

uniform bool texture_enabled;
uniform sampler2D texture_map;

// Colors vertex using only material color / texture color, with no lighting or shadows
void main()
{
    Material mat = material;
    vec3 result = mat.diffuse;

    if (texture_enabled){
        vec3 tex_color = vec3(texture(texture_map, frag_tex_coord));
        result = tex_color;
    }

    color = vec4(result, material.alpha);
}