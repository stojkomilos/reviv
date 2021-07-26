#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPosition;

void main()
{
    gl_Position = u_ProjectionMatrix * (u_ViewMatrix * (u_ModelMatrix * vec4(a_Position, 1.0)));

    v_Normal = a_Normal;
    v_TexCoord = a_TexCoord;
    v_FragPosition = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
}