#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_ShadowViewMatrices[6];
uniform mat4 u_ShadowProjectionMatrix;

out vec4 g_FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face=0; face<6; face++)
    {
        gl_Layer = face;
        for(int i=0; i<3; i++)
        {
            g_FragPos = gl_in[i].gl_Position;
            gl_Position = u_ShadowProjectionMatrix * u_ShadowViewMatrices[face] * g_FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}