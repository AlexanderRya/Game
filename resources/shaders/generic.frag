#version 460
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 texture_coords;

layout (location = 0) out vec4 frag_color;

layout (set = 0, binding = 2) uniform sampler2D default_sampler;

void main() {
    frag_color = vec4(texture(default_sampler, texture_coords).rgb, 1.0f);
}
