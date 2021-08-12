#version 430 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ShadowMapViewMatrix;
uniform mat4 u_ShadowMapProjectionMatrix;

void main()
{
    gl_Position = u_ShadowMapProjectionMatrix * u_ShadowMapViewMatrix * u_ModelMatrix * vec4(a_Pos, 1);
}
