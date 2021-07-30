#version 330 core
layout (location = 0) in vec2 a_Position;
layout (location = 2)

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = a_Position;
    gl_Position = vec4(a_Position, 0, 1);
}