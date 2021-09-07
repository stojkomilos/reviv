#version 430 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normals;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 ue_ViewMatrix;
uniform mat4 ue_ProjectionMatrix;

void main()
{
    gl_Position = ue_ProjectionMatrix * ue_ViewMatrix * u_ModelMatrix * vec4(a_Pos, 1);
}
