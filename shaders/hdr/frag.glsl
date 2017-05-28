#version 330 core

out vec4 color;
in vec2 frag_tex_coords;

uniform sampler2D hdr_buffer;
uniform sampler2D bloom_blur;
uniform float exposure;
uniform bool hdr;
uniform bool bloom;

void main()
{
    const float gamma = 2.2f;
    vec4 hdr_color_alpha = texture(hdr_buffer, frag_tex_coords).rgba;
    vec3 hdr_color = hdr_color_alpha.rgb;
    vec3 bloom_color = texture(bloom_blur, frag_tex_coords).rgb;

    if (bloom) {
        hdr_color += bloom_color; // additive blending
    }

    vec3 result = hdr_color;

    if (hdr) {
        result = vec3(1.0) - exp(-hdr_color * exposure);
        // also gamma correct while we're at it
        result = pow(result, vec3(1.0 / gamma));
    }

    color = vec4(result, hdr_color_alpha.a);
}