#version 330 core

out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D u_ShadowMap;

void main()
{
    float depthValue = texture(u_ShadowMap, v_TexCoords).r;
    FragColor = vec4(depthValue, depthValue, depthValue, 1.0);
}