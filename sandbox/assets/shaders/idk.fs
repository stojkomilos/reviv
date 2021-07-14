#version 460 core

in vec3 v_Normal;
in vec3 v_VertexPos;
in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;

uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

void main(){

	vec4 color = texture(u_Texture, v_TexCoord);
	vec3 lightColor = vec3(1, 0, 0);
	
	vec3 lightDir = normalize(u_LightPosition - v_VertexPos);

	vec3 ambient = vec3(0.1, 0.1, 0.1);
	
	float diff = dot(lightDir, v_Normal); 
	float diffStrenth = 0.5;
	vec3 diffuse = diff * diffStrenth * lightColor;

	vec3 viewDir = normalize(u_CameraPosition - v_VertexPos);
	vec3 reflectionDir = reflect(-lightDir, v_Normal);
	float specularStrength = 0.5;
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
	vec3 specular = spec * specularStrength * lightColor;

	vec3 totalLight = ambient + diffuse + specular;

	FragColor = color * vec4(totalLight, 1);
}
