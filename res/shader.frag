#version 330 core
out vec4 frag_color;

#define DIFFUSE_MASK 0x01
#define AMBIENT_MASK 0x02
#define SPECULAR_MASK 0x04
#define NORMAL_MASK 0x08

in vec2 UV;
in vec3 position_ws;
in vec3 normal_cs;
in vec3 eye_direction_cs;
in vec3 light_direction_cs;

uniform sampler2D ambient_texture_sampler;
uniform sampler2D diffuse_texture_sampler;
uniform sampler2D specular_texture_sampler;
uniform sampler2D normal_texture_sampler;

uniform vec3 light_position_ws;
uniform int mask;

void main() {
    float distance = length(light_position_ws - position_ws) / 30.f;

    vec3 n = normalize(normal_cs);
    vec3 l = normalize(light_direction_cs);
    vec3 e = normalize(eye_direction_cs);
    vec3 r = reflect(-l, n);

    float cos_theta = clamp(dot(n, l), 0.0, 1.0);
    float cos_alpha = clamp(dot(e,r), 0.0, 1.0);

    vec3 diffuse_material = vec3(1.0);
    if ((mask & DIFFUSE_MASK) != 0) {
        diffuse_material = texture2D (diffuse_texture_sampler, UV).rgb;
    }
    vec3 ambient_material = vec3(0.2);
    if ((mask & AMBIENT_MASK) != 0) {
        ambient_material = texture2D (ambient_texture_sampler, UV).rgb;
    }
    ambient_material *= diffuse_material;

    vec3 specular_material = vec3(0.3);
    if ((mask & SPECULAR_MASK) != 0) {
        specular_material *= texture2D (specular_texture_sampler, UV).rgb; 
    }

    vec3 result = ambient_material + 
    diffuse_material * cos_theta * 0.4 / (distance * distance) + 
    specular_material * pow(cos_alpha, 8) / (distance * distance);
    
    frag_color = vec4(result, 1.0);
}