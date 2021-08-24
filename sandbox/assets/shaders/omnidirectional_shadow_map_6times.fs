#version 330 core

in vec4 g_FragPosition;

uniform vec3 u_LightPosition;
uniform float u_FarPlane;

void main()
{
    float lightDistance = length(g_FragPosition.xyz - u_LightPosition);
    lightDistance /= u_FarPlane;

    gl_FragDepth = lightDistance;
}