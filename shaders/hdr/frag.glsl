#version 330 core

out vec4 color;
in vec2 frag_tex_coords;

uniform sampler2D hdr_buffer;
uniform float exposure;
uniform bool hdr;

void main()
{
    const float gamma = 2.2;
    vec3 hdr_color = texture(hdr_buffer, frag_tex_coords).rgb;

    // reinhard
    // vec3 result = hdrColor / (hdrColor + vec3(1.0));
    // exposure
    vec3 result = vec3(1.0) - exp(-hdr_color * exposure);
    // also gamma correct while we're at it
    //result = pow(result, vec3(1.0 / gamma));
    color = vec4(result, 1.0f);
}