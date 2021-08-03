#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    vec4 worldPosition = u_ModelMatrix * vec4(a_Position, 1.0);
    v_FragPos = worldPosition.xyz;
    v_Normal = a_Normal;
    v_TexCoords = a_TexCoords;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPosition;
}