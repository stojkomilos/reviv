#version 330 core

struct ShadowMap
{
    sampler2D depthMap;
    mat4 viewProjectionMatrix;
};

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

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    ShadowMap shadowMap;
};

in vec2 v_TexCoords;

out vec4 FragColor;

vec3 calculatePointLight(PointLight light);
vec3 calculateDirectionalLight(DirectionalLight light);
float calculateShadow(ShadowMap shadowMap);

uniform int ue_NumberOfPointLights;
uniform int ue_NumberOfDirectionalLights;
//#define NR_POINT_LIGHTS 1
uniform PointLight ue_PointLights[1];
uniform DirectionalLight ue_DirectionalLights[1];
uniform vec3 ue_ViewPosition;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedoSpecular;

vec3 d_FragPosition;
vec3 d_Normal;
vec3 d_MaterialDiffuse;
float d_MaterialSpecular;

float materialShininess = 1024.f; // TODO

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

    for(int i=0; i<0; i++)
    {
        result += calculatePointLight(ue_PointLights[i]);
    }

    for(int i=0; i<1; i++)
    {
        result += calculateDirectionalLight(ue_DirectionalLights[i]);
    }

    FragColor = vec4(result, 1);
}

float calculateShadow(ShadowMap shadowMap)
{
    float bias = 0.005f;
    vec4 positionFromLight = shadowMap.viewProjectionMatrix * vec4(d_FragPosition, 1);
    positionFromLight.x = (positionFromLight.x + 1) / 2;
    positionFromLight.y = (positionFromLight.y + 1) / 2;
    float shadow = 0;
    float depthMapTextureValue = texture(shadowMap.depthMap, positionFromLight.xy).r * 2 - 1;
    if(depthMapTextureValue < positionFromLight.z - bias)
    {
        shadow = 1;
    }
    if(positionFromLight.z > 1.0)
        shadow = 0.0;

    //shadow = (depthMapTextureValue + 1) / 2;

    return shadow;
}

vec3 calculatePointLight(PointLight light)
{
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

    //float shadow = calculateShadow(light.shadowMap);

    return ((ambient + diffuse + specular) * attenuation);
}

vec3 calculateDirectionalLight(DirectionalLight light)
{
    vec3 ambient = light.ambient * 0.1;

    light.direction = -normalize(light.direction); // TODO:

    float diffuseIntensity = max(0, dot(light.direction, d_Normal));
    vec3 diffuse = light.diffuse * (diffuseIntensity * d_MaterialDiffuse);

    vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    vec3 halfwayDirection = normalize(light.direction + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = light.specular * (specularIntensity * d_MaterialSpecular);

    float shadow = calculateShadow(light.shadowMap);

    if(shadow == 1)
    {
        diffuse = vec3(0, 0, 0);
        specular = vec3(0, 0, 0);
    }

    return (ambient + diffuse + specular);
}