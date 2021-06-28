#version 460 core

in vec2 v_TexCoord;

out vec4 FragColor;


uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){

	FragColor = u_Color * texture(u_Texture, v_TexCoord);
}
