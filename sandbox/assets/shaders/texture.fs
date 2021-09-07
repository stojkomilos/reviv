#version 330 core

in vec2 v_TexCoord;
out vec4 FragColor;

uniform sampler2D u_Texture0;

void main()
{
    vec3 result = vec3(0, 0, 0);

   //result = texture(u_Texture0, v_TexCoord);

    FragColor = mix(texture(u_Texture0, v_TexCoord), vec4(v_TexCoord, 0, 1), 0);

    //FragColor = vec4(result, 1);
    //FragColor = mix(texture(u_Texture0, v_TexCoord), vec4(0, 1, 0, 1), 0.8);
}