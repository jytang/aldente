#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 baseColor;
uniform bool hasTexture;
uniform float alpha;

void main()
{
    float final_alpha = alpha;

    if (hasTexture) {
        vec4 tex_color = texture(text, TexCoords);
        float tex_alpha = tex_color.a;
        final_alpha = min(tex_alpha, final_alpha);
        color = vec4(tex_color.rgb, final_alpha);
    } else
        color = vec4(baseColor, final_alpha);
}
