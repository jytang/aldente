#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    vec3 diffuse;
    float alpha;
};

in vec3 frag_pos;
in vec3 frag_normal;
in vec4 frag_pos_light;
in vec2 frag_tex_coord;

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

    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722)); // human eye more sensitive to green colors than blue
    if (brightness > 1.0f)
        BrightColor = vec4(result, 1.0f);
    else
        BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    FragColor = vec4(result, material.alpha);
}