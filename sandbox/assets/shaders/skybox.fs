#version 330 core

in vec3 v_TexCoords;
out vec4 FragColor;

uniform samplerCube u_CubeMap;

void main()
{
    //FragColor = texture(u_CubeMap, v_TexCoords);
    FragColor = vec4(texture(u_CubeMap, v_TexCoords).rgb, 1);
}