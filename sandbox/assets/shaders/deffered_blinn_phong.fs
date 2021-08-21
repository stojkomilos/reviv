#version 330 core

struct ShadowMapDirectional
{
    sampler2D depthMap;
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

struct ShadowMapOmnidirectional
{
    samplerCube depthMap;
    float farPlane;
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

    ShadowMapOmnidirectional shadowMap;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    ShadowMapDirectional shadowMap;
};

in vec2 v_TexCoords;

out vec4 FragColor;

uniform DirectionalLight ue_DirectionalLights[8];
vec3 calculateDirectionalLight(DirectionalLight light);
float calculateShadowDirectional(ShadowMapDirectional shadowMap);

uniform PointLight ue_PointLights[8];
vec3 calculatePointLight(int index);
float calculateShadowOmnidirectional(int index);

uniform int ue_NumberOfPointLights;
uniform int ue_NumberOfDirectionalLights;
uniform vec3 ue_ViewPosition;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedoSpecular;

vec3 d_FragPosition;
vec3 d_Normal;
vec3 d_MaterialDiffuse;
float d_MaterialSpecular;

const float materialShininess = 128.f; // TODO

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

    for(int i=0; i<ue_NumberOfPointLights; i++)
    {
        result += calculatePointLight(i);
    }

    for(int i=0; i<ue_NumberOfDirectionalLights; i++)
    {
        result += calculateDirectionalLight(ue_DirectionalLights[i]);
    }

    FragColor = vec4(result, 1);
}

float calculateShadowDirectional(ShadowMapDirectional shadowMap)
{
    float bias = 0.005f;
    vec4 positionFromLight = shadowMap.projectionMatrix * shadowMap.viewMatrix * vec4(d_FragPosition, 1);
    positionFromLight.x = (positionFromLight.x + 1) / 2;
    positionFromLight.y = (positionFromLight.y + 1) / 2;
    float shadow = 0;
    float depthMapTextureValue = texture(shadowMap.depthMap, positionFromLight.xy).r * 2 - 1;
    if(depthMapTextureValue < positionFromLight.z - bias)
    {
        shadow = 1.0;
    }
    if(positionFromLight.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 calculateDirectionalLight(DirectionalLight light)
{
    vec3 ambient = light.ambient * 0.1;

    vec3 lightDirection = -normalize(light.direction);

    float diffuseIntensity = max(0, dot(lightDirection, d_Normal));
    vec3 diffuse = light.diffuse * (diffuseIntensity * d_MaterialDiffuse);

    vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = light.specular * (specularIntensity * d_MaterialSpecular);

    float shadow = calculateShadowDirectional(light.shadowMap);

    if(shadow == 1)
    {
        diffuse = vec3(0, 0, 0); //alternative that may be more optimized:  diffuse = diffuse * (1 - shadow);
        specular = vec3(0, 0, 0);                             //            specular = specular * (1 - shadow);
    }

    return (ambient + diffuse + specular);
    //vec3 vShadow = vec3(shadow, shadow, shadow);
    //return (ambient + diffuse + specular) * 0.0001f + vShadow;
}

float calculateShadowOmnidirectional(int index)
{
    float bias = 0.05f; //TODO: why different from directionalLight bias?
    vec3 positionFromLight = d_FragPosition - ue_PointLights[index].position;

    float shadow = 0;
    float depthMapTextureValue = texture(ue_PointLights[index].shadowMap.depthMap, positionFromLight).r;

    depthMapTextureValue *= ue_PointLights[index].shadowMap.farPlane;
    float currentDepth = length(positionFromLight);

    if(currentDepth > depthMapTextureValue + bias)
    {
        shadow = 1.0;
    }

    if(currentDepth > ue_PointLights[index].shadowMap.farPlane)
        shadow = 1.0;

    return shadow;
}

vec3 calculatePointLight(int index)
{
    vec3 ambient = ue_PointLights[index].ambient * 0.1;

    float distance = length(ue_PointLights[index].position - d_FragPosition);
    float attenuation = 1.0f / (ue_PointLights[index].constant + ue_PointLights[index].linear * distance + ue_PointLights[index].quadratic * distance * distance);

    vec3 lightDirection = normalize(ue_PointLights[index].position - d_FragPosition);

    float diffuseIntensity = max(0, dot(lightDirection, d_Normal));
    vec3 diffuse = ue_PointLights[index].diffuse * (diffuseIntensity * d_MaterialDiffuse);

    vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = ue_PointLights[index].specular * (specularIntensity * d_MaterialSpecular);

    float shadow = calculateShadowOmnidirectional(index);

    if(shadow == 1)
    {
        diffuse = vec3(0, 0, 0); //alternative that may be more optimized:  diffuse = diffuse * (1 - shadow);
        specular = vec3(0, 0, 0);                             //            specular = specular * (1 - shadow);
    }

    return ((ambient + diffuse + specular) * attenuation);
}
