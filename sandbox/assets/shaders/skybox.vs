#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_SkyboxViewMatrix;

void main()
{
    v_TexCoords.x = -a_Position.y; // this is because opengl is stupid and likes to rotate cube map textures on it's own, so i convert to the more conventional coordinate system, where the Y axis is upward
    v_TexCoords.y = -a_Position.z;
    v_TexCoords.z = a_Position.x;

    vec4 result = u_ProjectionMatrix * u_SkyboxViewMatrix * vec4(a_Position, 1.0);

    gl_Position = result.xyww;
}