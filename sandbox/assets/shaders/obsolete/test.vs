#version 430 core

uniform mat4 u_ProjectionMatrix;
uniform vec3 u_Position;

void main()
{
    gl_Position = u_ProjectionMatrix * vec4(u_Position, 1);
}
