#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_VertexPos;
out vec2 v_TexCoord;

void main(){
	
	vec4 rez = (u_Projection * u_View * u_Model * vec4(a_Pos, 1));
	v_Normal = a_Normal;
	v_VertexPos = (u_Model * vec4(a_Pos, 1)).xyz;
	gl_Position = rez;
	v_TexCoord = a_TexCoord;
}
