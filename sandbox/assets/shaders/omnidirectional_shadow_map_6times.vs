#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ShadowViewMatrix;
uniform mat4 u_ShadowProjectionMatrix;

void main()
{
    gl_Position = u_ShadowProjectionMatrix * u_ShadowViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}