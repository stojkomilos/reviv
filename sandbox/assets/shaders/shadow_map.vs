#version 430 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ShadowMapViewProjectionMatrix;

void main()
{
    gl_Position = u_ShadowMapViewProjectionMatrix * u_ModelMatrix * vec4(a_Pos, 1);
}