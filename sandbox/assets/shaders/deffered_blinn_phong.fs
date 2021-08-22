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

    int isShadowMapped;
    ShadowMapOmnidirectional shadowMap;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isShadowMapped;
    ShadowMapDirectional shadowMap;
};

in vec2 v_TexCoords;

out vec4 FragColor;

uniform DirectionalLight ue_DirectionalLights[8];
vec3 calculateDirectionalLight(DirectionalLight light);
float calculateShadowDirectional(ShadowMapDirectional shadowMap);

uniform PointLight ue_PointLights[8];
vec3 calculatePointLight(PointLight light);
//float calculateShadowOmnidirectional(PointLight light);

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
        result += calculatePointLight(ue_PointLights[i]);
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

    //shadow = texture(shadowMap.depthMap, v_TexCoords).r; // for DEBUG

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

    float shadow = 0.0;

    if(light.isShadowMapped == 1)
    {
        shadow = calculateShadowDirectional(light.shadowMap);
    }

    if(shadow == 1)
    {
        diffuse = vec3(0, 0, 0); //alternative that may be more optimized:  diffuse = diffuse * (1 - shadow);
        specular = vec3(0, 0, 0);                             //            specular = specular * (1 - shadow);
    }

    return (ambient + diffuse + specular);
    //vec3 vShadow = vec3(shadow, shadow, shadow);                // for DEBUG
    //return (ambient + diffuse + specular) * 0.0001f + vShadow;
}


/*
float calculateShadowOmnidirectional(PointLight light)
{
    float bias = 0.05f; //TODO: why different from directionalLight bias?
    vec3 positionFromLight = d_FragPosition - light.position;

    float shadow = 0;
    float depthMapTextureValue = texture(light.shadowMap.depthMap, positionFromLight).r;

    depthMapTextureValue *= light.shadowMap.farPlane;
    float currentDepth = length(positionFromLight);

    if(currentDepth > depthMapTextureValue + bias)
    {
        shadow = 1.0;
    }

    if(currentDepth > light.shadowMap.farPlane)
        shadow = 1.0;

    return shadow;
}
*/

vec3 calculatePointLight(PointLight light)
{
    vec3 ambient = light.ambient * 0.1;
    //vec3 ambient = vec3(0, 0, 0);

    float fragmentDistance = length(light.position - d_FragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * fragmentDistance + light.quadratic * fragmentDistance * fragmentDistance);

    vec3 lightDirection = normalize(light.position - d_FragPosition);

    //float diffuseIntensity = max(0, dot(lightDirection, d_Normal));
    //vec3 diffuse = light.diffuse * (diffuseIntensity * d_MaterialDiffuse);
    vec3 diffuse = vec3(0, 0, 0);

    //vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    //vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    //float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    //vec3 specular = light.specular * (specularIntensity * d_MaterialSpecular);
    vec3 specular = vec3(0, 0, 0);

    float shadow = 0.0;
    if(light.isShadowMapped == 1)
    {
        //shadow = calculateShadowOmnidirectional(light.shadowMap);
    }

    //if(shadow == 1)
    //{
    //    diffuse = vec3(0, 0, 0); //alternative that may be more optimized:  diffuse = diffuse * (1 - shadow);
    //    specular = vec3(0, 0, 0);                             //            specular = specular * (1 - shadow);
    //}

    return ((ambient + diffuse + specular) * attenuation);
}
