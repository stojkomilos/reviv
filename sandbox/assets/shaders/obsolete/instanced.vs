#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aOffset;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	
	vec4 rez = (projection * view * model * vec4(aPos + aOffset, 1));
	gl_Position = rez;
	
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
