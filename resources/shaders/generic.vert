#version 460

layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec3 texture_coords;

/*layout (set = 0, binding = 0) uniform CameraData {
    mat4 pv_matrix;
} camera;

layout (set = 0, binding = 1) uniform Transform {
    mat4 model;
} transform;

layout (set = 0, binding = 2) buffer readonly Instance {
    mat4 model[];
} instances;*/

void main() {
    gl_Position = /*camera.pv_matrix * instances.model[gl_InstanceIndex] * transform.model * */vec4(vertex_pos, 1.0f);
}