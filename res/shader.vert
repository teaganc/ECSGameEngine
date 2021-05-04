#version 330 core

layout (location = 0) in vec3 position_ms;
layout (location = 1) in vec3 normal_ms;
layout (location = 2) in vec2 tex_coord;

out vec2 UV;
out vec3 position_ws;
out vec3 normal_cs;
out vec3 eye_direction_cs;
out vec3 light_direction_cs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 MV;
uniform mat4 MVP;
uniform vec3 light_position_ws;

void main() {
    gl_Position = MVP * vec4(position_ms, 1.0);

    position_ws = (model * vec4(position_ms,1)).xyz;

    vec3 position_cs = (view * vec4(position_ws, 1.0)).xyz;
    eye_direction_cs = vec3(0.0) - position_cs;

    vec3 light_position_cs = (view * vec4(light_position_ws, 1.0)).xyz;
    light_direction_cs = light_position_cs - position_cs;

    //normal_cs = (view * transpose(inverse(model)) * vec4(normal_ms, 0.0)).xyz;
    normal_cs = (MV * vec4(normal_ms, 0.0)).xyz;

    UV = tex_coord;
}