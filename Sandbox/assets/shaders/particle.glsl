#define _TYPE_VERTEX_SHADER
#version 460 core
layout (location = 0) in vec2 a_Position;

layout (location = 1) in vec2 a_InstanceOffset;
layout (location = 2) in vec4 a_InstanceColor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main() {
    v_Color = a_InstanceColor;
    gl_Position = u_ViewProjection * vec4(a_Position + a_InstanceOffset, 0.0, 1.0);
}

#define _TYPE_FRAGMENT_SHADER
#version 460 core
layout (location = 0) out vec4 color;

in vec4 v_Color;

void main() {
    color = v_Color;
}
