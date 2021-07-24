#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec3 u_Color;
uniform vec3 u_LightPosition;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPosition;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.f);

    v_Normal = a_Normal;
    v_TexCoord = a_TexCoord;
    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
}