#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 ue_ViewMatrix;
uniform mat4 ue_ProjectionMatrix;

void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = ue_ProjectionMatrix * ue_ViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}