#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct FocusedLight
{
    vec3 position;
    vec3 direction;
    float innerCutOffAngle;
    float outerCutOffAngle;
};

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPosition;

out vec4 FragColor;

// environment
//uniform mat4 u_ModelMatrix;
//uniform mat4 u_ViewMatrix;
//uniform mat4 u_ProjectionMatrix;

uniform vec3 u_ViewPosition;

// material
uniform Material u_Material;

#define NR_POINT_LIGHTS 1
#define NR_DIRECTIONAL_LIGHTS 0
#define NR_FOCUSED_LIGHTS 0

uniform PointLight u_PointLights[NR_POINT_LIGHTS];
//uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
//uniform FocusedLight focusedLights[NR_FOCUSED_LIGHTS];

//vec3 calculateDirectionalLight(DirectionalLight directionalLight);
//vec3 calculateFocusedLight(PointLight pointLight);
vec3 calculatePointLight(PointLight pointLight);

void main()
{

    vec3 result = vec3(0, 0, 0);
    for(int i=0; i<NR_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(u_PointLights[i]);
    }

    FragColor = vec4(result, 1);
}

vec3 calculatePointLight(PointLight pointLight)
{
    vec3 ambient = pointLight.ambient * u_Material.ambient;

    float distance = length(pointLight.position - v_FragPosition);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(pointLight.position - v_FragPosition);

    float diffuseIntensity = max(0, dot(lightDirection, normal));
    vec3 diffuse = pointLight.diffuse * (diffuseIntensity * u_Material.diffuse);

    vec3 viewDirection = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularIntensity = pow(u_Material.shininess, max(0.0, dot(viewDirection, reflectionDirection)));
    vec3 specular = pointLight.specular * (specularIntensity * u_Material.specular);

    //float specularIntensity = pow(u_Material.shininess, attenuation * max(0.0, dot(viewDirection, reflectionDirection)));
    //ambient *= attenuation;
    //diffuse *= attenuation;
    //specular *= attenuation;
    //return (ambient + diffuse + specular);
    return ((ambient + diffuse + specular) * attenuation);
}

/*
vec3 calculateDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection)
{
    vec3 ambient = directionalLight.ambient * u_Material.ambient;

    vec3 lightDirection = normalize(directionalLight.direction); // TODO: znak "-" ispred?
    float diffuseIntensity = max(0, dot(lightDirection, normalize(v_Normal)));
    vec3 diffuse = directionalLight.diffuse * (diffuseIntensity * u_Material.diffuse);

    vec3 viewDirection = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, v_Normal);
    float specularIntensity = pow(u_Material.shininess, max(0.0, dot(viewDirection, reflectionDirection)));
    vec3 specular = directionalLight.specular * (specularIntensity * u_Material.specular);

    return (ambient + diffuse + specular);
}

vec3 calculateFocusedLight(FocusedLight focusedLight, vec3 normal, vec3 viewDirection)
{
    vec3 ambient = focusedLight.ambient * u_Material.ambient;

    vec3 lightDirection = normalize(focusedLight.position - v_FragPosition);
    float diffuseIntensity = max(0, dot(lightDirection, normalize(v_Normal)));
    vec3 diffuse = pointLight.diffuse * (diffuseIntensity * u_Material.diffuse);

    vec3 viewDirection = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, v_Normal);
    float specularIntensity = pow(u_Material.shininess, max(0.0, dot(viewDirection, reflectionDirection)));
    vec3 specular = pointLight.specular * (specularIntensity * u_Material.specular);

    // clamp
    float angle = dot(lightDirection, focusedLight.direction);
    float attenuation = (outerCutOffAngle - angle) / (outerCutOffAngle - innerCutOffAngle);
    attenuation = clamp(attenuation, 0.0, 1.0);

    return (ambient + diffuse + specular) * attenuation;
}
*/