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

#define NR_POINT_LIGHTS 1
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedoSpecular;

vec3 calculatePointLight(PointLight pointLight);

//vec3 gFragPosition, gNormal;
//vec4 gAlbedo;

uniform vec3 u_ViewPosition;

void main()
{

    vec3 result = vec3(0, 0, 0);
    for(int i=0; i<NR_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(u_PointLights[i]);
    }

    vec3 fragPosition = normalize(texture(u_gPosition, v_TexCoords).rgb);
    vec3 normal = texture(u_gNormal, v_TexCoords).rgb;
    vec3 materialDiffuse = texture(u_gAlbedoSpecular, v_TexCoords).rgb;
    float materialSpecular = texture(u_gAlbedoSpecular, v_TexCoords).a;

    result = result * 0.00001f + materialDiffuse;
    FragColor = vec4(result, 1);
}

vec3 calculatePointLight(PointLight pointLight)
{
    vec3 fragPosition = texture(u_gPosition, v_TexCoords).rgb;
    vec3 normal = texture(u_gNormal, v_TexCoords).rgb;

    vec3 materialDiffuse = texture(u_gAlbedoSpecular, v_TexCoords).rgb;
    float materialSpecular = texture(u_gAlbedoSpecular, v_TexCoords).a;
    float materialShininess = 16.0;

    vec3 ambient = pointLight.ambient * 0.1;

    float distance = length(pointLight.position - fragPosition);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    vec3 lightDirection = normalize(pointLight.position - fragPosition);

    float diffuseIntensity = max(0, dot(lightDirection, normal));
    vec3 diffuse = pointLight.diffuse * (diffuseIntensity * materialDiffuse);

    vec3 viewDirection = normalize(u_ViewPosition - fragPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(0.0, dot(viewDirection, halfwayDirection)), materialShininess);
    vec3 specular = pointLight.specular * (specularIntensity * materialSpecular);

    return ((ambient + diffuse + specular) * attenuation);
}
