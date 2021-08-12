#version 330 core

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 v_TexCoords;

out vec4 FragColor;

vec3 calculatePointLight(PointLight light);
float calculateShadows();

#define NR_POINT_LIGHTS 1
uniform PointLight ue_PointLights[NR_POINT_LIGHTS];
uniform vec3 ue_ViewPosition;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedoSpecular;

vec3 d_FragPosition;
vec3 d_Normal;
vec3 d_MaterialDiffuse;
float d_MaterialSpecular;

uniform sampler2D ue_ShadowMap;
uniform mat4 ue_LightViewMatrix;
uniform mat4 ue_LightProjectionMatrix;

void main()
{

    vec3 result = vec3(0, 0, 0);

    d_FragPosition = texture(u_gPosition, v_TexCoords).rgb;
    d_Normal = texture(u_gNormal, v_TexCoords).rgb;
    d_MaterialDiffuse = texture(u_gAlbedoSpecular, v_TexCoords).rgb;
    d_MaterialSpecular = texture(u_gAlbedoSpecular, v_TexCoords).a;
    
    if(d_Normal == vec3(0, 0, 0))
    {
        discard;
    }

    for(int i=0; i<NR_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(ue_PointLights[i]);
    }

    float shadow = calculateShadow();
    if(shadow == 1)
        result = vec3(0, 0, 0);

    //result = result * 0.00001f + vec3(d_MaterialSpecular, d_MaterialSpecular, d_MaterialSpecular);
    //result = result * 0.00001f + d_MaterialDiffuse.rgb;
    //result = result * 0.00001f + normalize(d_FragPosition.xyz);

    FragColor = vec4(result, 1);
}

float calculateShadow()
{
    float bias = 0.005f;
    vec4 positionFromLight = ue_LightProjectionMatrix * (ue_LightViewMatrix * vec4(d_FragPosition, 1));
    positionFromLight.x = (positionFromLight.x + 1) / 2;
    positionFromLight.y = (positionFromLight.y + 1) / 2;
    float shadow = 0;
    if(texture(ue_ShadowMap, positionFromLight.xy).r * 2 - 1 < positionFromLight.z - bias)
    {
        shadow = 1;
    }
    if(positionFromLight.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 calculatePointLight(PointLight light)
{
    float materialShininess = 16.0;

    vec3 ambient = light.ambient * 0.1;

    float distance = length(light.position - d_FragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 lightDirection = normalize(light.position - d_FragPosition);

    float diffuseIntensity = max(0, dot(lightDirection, d_Normal));
    vec3 diffuse = light.diffuse * (diffuseIntensity * d_MaterialDiffuse);

    vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = light.specular * (specularIntensity * d_MaterialSpecular);

    return ((ambient + diffuse + specular) * attenuation);
}
