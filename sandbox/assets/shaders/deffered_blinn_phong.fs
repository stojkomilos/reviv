#version 400 core
// requires version 400 for samplerCube arrays

//struct ShadowMapDirectional // NOTE: can't have ShadowMapDirectional as member element of DirectionalLight structure because GLSL is stupid
//{
//    sampler2D depthMap;
//    mat4 viewMatrix;
//    mat4 projectionMatrix;
//};
//
//struct ShadowMapOmnidirectional // NOTE: can't have ShadowMapOmnidirectional as member element of PointLight structure because GLSL is stupid
//{
//    samplerCube depthMap;
//    float farPlane;
//};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isShadowMapped;
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
};

in vec2 v_TexCoords;

out vec4 FragColor;

// Directional light stuff
#define NR_DIR_LIGHT 1
uniform int ue_NumberOfDirectionalLights;
uniform DirectionalLight ue_DirectionalLights[NR_DIR_LIGHT];
//uniform sampler2D ue_DirectionalLightShadowMapsDepthMap[NR_DIR_LIGHT];
//uniform float ue_DirectionalLightShadowMapsViewProjectionMatrix[NR_DIR_LIGHT];
vec3 calculateDirectionalLight(int index);
//float calculateShadowDirectional(int index);
// ---

// Point light stuff
//#define NR_POINT_LIGHT 0
//uniform int ue_NumberOfPointLights;
//uniform PointLight ue_PointLights[NR_POINT_LIGHT];
//uniform samplerCube ue_PointLightShadowMapsDepthMap[NR_POINT_LIGHT];
//uniform float ue_PointLightShadowMapsFarPlane[NR_POINT_LIGHT];
//vec3 calculatePointLight(int index);
//float calculateShadowOmnidirectional(int index);
//uniform samplerCube ue_StupidShadowMap;

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

    //for(int i=0; i<NR_POINT_LIGHT; i++)
    //{
    //    result += calculatePointLight(i);
    //}

    for(int i=0; i<ue_NumberOfDirectionalLights; i++)
    {
        result += calculateDirectionalLight(i);
    }

    //result = result * 0.001f + texture(u_TestTexture, v_TexCoords).rgb;
    //result = result * 0.001f + normalize(d_Normal) + d_FragPosition + d_MaterialDiffuse + vec3(d_MaterialSpecular, d_MaterialSpecular, d_MaterialSpecular);
    FragColor = vec4(result, 1);
}

vec3 calculateDirectionalLight(int index)
{
    vec3 ambient = ue_DirectionalLights[index].ambient * 0.1;

    vec3 lightDirection = -normalize(ue_DirectionalLights[index].direction);

    float diffuseIntensity = max(0, dot(lightDirection, d_Normal));
    vec3 diffuse = ue_DirectionalLights[index].diffuse * (diffuseIntensity * d_MaterialDiffuse);

    vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = ue_DirectionalLights[index].specular * (specularIntensity * d_MaterialSpecular);

    //float shadow = 0.0;

    //if(ue_DirectionalLights[index].isShadowMapped == 1)
    //{
    //    shadow = calculateShadowDirectional(index);
    //}

    //if(shadow == 1)
    //{
    //    diffuse = vec3(0, 0, 0); //alternative that may be more optimized:  diffuse = diffuse * (1 - shadow);
    //    specular = vec3(0, 0, 0);                             //            specular = specular * (1 - shadow);
    //}

    return (ambient + diffuse + specular);
    //vec3 vShadow = vec3(shadow, shadow, shadow);                // for DEBUG
    //return (ambient + diffuse + specular) * 0.0001f + vShadow;

}

/*

float calculateShadowDirectional(int index)
{
    float bias = 0.005f;
    vec4 positionFromLight = ue_DirectionalLightShadowMapsViewProjectionMatrix[index] * vec4(d_FragPosition, 1);
    positionFromLight.x = (positionFromLight.x + 1) / 2;
    positionFromLight.y = (positionFromLight.y + 1) / 2;
    float shadow = 0;

    float depthMapTextureValue = texture(ue_DirectionalLightShadowMapsDepthMap[index], positionFromLight.xy).r * 2 - 1;
    //float depthMapTextureValue = 1.0;

    if(depthMapTextureValue < positionFromLight.z - bias)
    {
        shadow = 1.0;
    }
    if(positionFromLight.z > 1.0)
        shadow = 0.0;

    //shadow = texture(shadowMap.depthMap, v_TexCoords).r; // for DEBUG

    return shadow;
}
*/


/*
vec3 calculatePointLight(int index)
{
    vec3 ambient = ue_PointLights[index].ambient * 0.1;
    //vec3 ambient = vec3(0, 0, 0);

    float fragmentDistance = length(ue_PointLights[index].position - d_FragPosition);
    float attenuation = 1.0f / (ue_PointLights[index].constant + ue_PointLights[index].linear * fragmentDistance + ue_PointLights[index].quadratic * fragmentDistance * fragmentDistance);

    vec3 lightDirection = normalize(ue_PointLights[index].position - d_FragPosition);

    float diffuseIntensity = max(0, dot(lightDirection, d_Normal));
    vec3 diffuse = ue_PointLights[index].diffuse * (diffuseIntensity * d_MaterialDiffuse);
    //vec3 diffuse = vec3(0, 0, 0);

    vec3 viewDirection = normalize(ue_ViewPosition - d_FragPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = ue_PointLights[index].specular * (specularIntensity * d_MaterialSpecular);
    //vec3 specular = vec3(0, 0, 0);

    float shadow = 0.0;
    if(ue_PointLights[index].isShadowMapped == 1)
    {
        shadow = calculateShadowOmnidirectional(index);
    }
    
    if(shadow == 1)
    {
        diffuse = vec3(0, 0, 0); //alternative that may be more optimized:  diffuse = diffuse * (1 - shadow);
        specular = vec3(0, 0, 0);                             //            specular = specular * (1 - shadow);
        //diffuse = diffuse + vec3(0.0001, 0.0001, 0.0001);
    }

    return (ambient + diffuse + specular);
    //return ((ambient + diffuse + specular) * attenuation) * 0.00001f + vec3(shadow, shadow, shadow);
    //return ((ambient + diffuse + specular) * attenuation);
}


float calculateShadowOmnidirectional(int index)
{
    float bias = 0.05f; //TODO: why different from directionalLight bias?
    vec3 positionFromLight = d_FragPosition - ue_PointLights[index].position;

    float shadow = 0;
    float closestDepth = texture(ue_PointLightShadowMapsDepthMap[index], positionFromLight).r; // TODO: does not have to be normalized

    closestDepth *= ue_PointLightShadowMapsFarPlane[index];
    float currentDepth = length(positionFromLight);

    if(currentDepth > closestDepth + bias)
    {
        shadow = 1.0;
    }

    if(currentDepth > ue_PointLightShadowMapsFarPlane[index])
        shadow = 1.0;

    //return shadow;
    return (closestDepth / ue_PointLightShadowMapsFarPlane[index]);
}
*/
