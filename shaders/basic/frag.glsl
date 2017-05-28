#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool shadows_enabled;
    float alpha;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
    float ambient_coeff;
};
#define MAX_DIR_LIGHTS 10

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float quadratic;
    float ambient_coeff;
};
#define MAX_POINT_LIGHTS 20

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float angle;
    float taper;
    float intensity;
    float quadratic;
    float ambient_coeff;
};
#define MAX_SPOT_LIGHTS 10

in vec3 frag_pos;
in vec3 frag_normal;
in vec4 frag_pos_light;
in vec2 frag_tex_coord;

uniform vec3 cam_pos;
uniform Material material;

uniform DirLight dir_lights[MAX_DIR_LIGHTS];
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];
uniform int num_dir_lights;
uniform int num_point_lights;
uniform int num_spot_lights;

uniform bool texture_enabled;
uniform sampler2D texture_map;

uniform sampler2D shadow_map;

vec3 calc_dir_light(DirLight light, Material mat, vec3 normal, vec3 view_dir, bool is_shadow_light);
vec3 calc_point_light(PointLight light, Material mat, vec3 normal, vec3 view_dir, bool is_shadow_light);
vec3 calc_spot_light(SpotLight light, Material mat, vec3 normal, vec3 view_dir, bool is_shadow_light);
vec3 calc_color(Material mat, vec3 normal, vec3 view_dir, vec3 light_dir, float light_intensity, vec3 light_color, float ambient_coeff, bool is_shadow_light);
float calc_shadows(vec4 pos_from_light, vec3 light_dir);

// Colors vertex using phong shading model
void main()
{
    vec3 normal = normalize(frag_normal);
    vec3 view_dir = normalize(cam_pos - frag_pos);

    // Change material if there is texture attached.
    Material mat = material;
    if (texture_enabled){
        vec3 tex_color = vec3(texture(texture_map, frag_tex_coord));
        mat.diffuse = tex_color * mat.diffuse;
    }

    // Do lighting
    vec3 result = vec3(0.0f);
    // First directional light has shadows. TODO: allow for multiple light shadows!
    if (num_dir_lights > 0) {
        result += calc_dir_light(dir_lights[0], mat, normal, view_dir, true);
    }
    for (int i = 1; i < num_dir_lights; ++i) {
        result += calc_dir_light(dir_lights[i], mat, normal, view_dir, false);
    }
    for (int i = 0; i < num_point_lights; ++i) {
        result += calc_point_light(point_lights[i], mat, normal, view_dir, false);
    }
    for (int i = 0; i < num_spot_lights; ++i) {
        result += calc_spot_light(spot_lights[i], mat, normal, view_dir, false);
    }

    // Check whether the resulting color is brighter than a threshold -- if so, output as a bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722)); // human eye more sensitive to green colors than blue
    if (brightness > 1.0f)
        BrightColor = vec4(result, 1.0f);
    else
        BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    FragColor = vec4(result, material.alpha);
}

vec3 calc_dir_light(DirLight light, Material mat,
    vec3 normal, vec3 view_dir,
    bool is_shadow_light)
{
    vec3 light_dir = normalize(-light.direction);
    return calc_color(mat, normal, view_dir,
        light_dir, light.intensity,
        light.color, light.ambient_coeff,
        is_shadow_light);
}

vec3 calc_point_light(PointLight light, Material mat,
    vec3 normal, vec3 view_dir,
    bool is_shadow_light)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0f / (light.quadratic * distance * distance);
    float light_intensity = light.intensity * attenuation;

    return calc_color(mat, normal, view_dir,
        light_dir, light_intensity,
        light.color, light.ambient_coeff,
        is_shadow_light);
}

vec3 calc_spot_light(SpotLight light, Material mat,
    vec3 normal, vec3 view_dir,
    bool is_shadow_light)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 spotlight_dir = normalize(light.direction);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0f / (light.quadratic * distance * distance);

    vec3 light_color = vec3(0.0f);
    float light_intensity = light.intensity * attenuation;
    // Check that point is within the beam
    if (dot(-light_dir, spotlight_dir) > cos(light.angle)) {
        light_color = light.color * pow(dot(-light_dir, spotlight_dir), light.taper);
    }

    return calc_color(mat, normal, view_dir,
        light_dir, light_intensity,
        light_color, light.ambient_coeff,
        is_shadow_light);
}


// Colors fragment (triangle) based on material parameters
vec3 calc_color(Material mat,
    vec3 normal, vec3 view_dir,
    vec3 light_dir, float light_intensity,
    vec3 light_color, float ambient_coeff,
    bool is_shadow_light)
{
    // Diffuse: c_d = c_l * k_d * dot(n, L)
    vec3 diffuse = light_intensity * light_color *
        mat.diffuse * max(dot(normal, light_dir), 0.0);

    // Specular: c_s = c_l * k_s * dot(n, h)^s
    vec3 specular = light_intensity * light_color *
        mat.specular *
        pow(max(dot(normal, normalize(light_dir + view_dir)), 0.0), mat.shininess);

    // Ambient: c_a (diffuse color) * k_a (coeff)
    vec3 ambient = mat.diffuse * ambient_coeff;

    // Shadows
    float shadow = 0;
    if (material.shadows_enabled && is_shadow_light)
        shadow = calc_shadows(frag_pos_light, light_dir);
    return (1.0 - shadow) * (diffuse + specular) + ambient;
}

// Calculates shadows
float calc_shadows(vec4 pos_from_light, vec3 light_dir)
{
    vec3 clip_coords = pos_from_light.xyz / pos_from_light.w;

    // Transform to range of [0, 1] to fit depth map
    clip_coords = clip_coords * 0.5 + 0.5;
    float current_depth = clip_coords.z;

    if (current_depth > 1.0) {
        return 0.0;
    }

    float bias = max(0.005 * (1.0 - dot(normalize(frag_normal), light_dir)), 0.003);
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, clip_coords.xy + vec2(x, y) * texelSize).r;
            shadow += current_depth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}