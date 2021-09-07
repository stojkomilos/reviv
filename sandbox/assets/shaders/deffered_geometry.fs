#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform vec3 u_Diffuse;
uniform float u_Specular;

void main()
{
    gPosition = v_FragPos;

    // TODO: don't normalize here, but rather normalize in deffered lighting pass. performance reasons
    gNormal = normalize(v_Normal);

    gAlbedoSpecular.rgb = u_Diffuse.rgb;
    gAlbedoSpecular.a = u_Specular;
}