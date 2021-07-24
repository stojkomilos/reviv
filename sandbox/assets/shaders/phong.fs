#version 330 core

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPosition;

out vec4 FragColor;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec3 u_Color;
uniform vec3 u_LightPosition;

void main()
{
    vec3 lightDirection = u_LightPosition - v_FragPosition;
    float diffuseIntensity = max(0, dot(normalize(lightDirection), normalize(v_Normal)));

    float ambientIntensity = 0.1f;

    float lightIntensity = min(1, ambientIntensity + diffuseIntensity);
    FragColor = vec4(u_Color * lightIntensity, 1);
}