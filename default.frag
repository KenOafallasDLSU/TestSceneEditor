#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 currentPos;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
    vec3 lightDistanceVec = lightPos - currentPos;
    float dist = length(lightDistanceVec);
    float a = 3.0;
    float b = 0.7;
    float intensity = 1.0f / ((a * dist * dist) + (b * dist) + 1.0f);

    // ambient light
    float ambient = 0.45f;

    // diffuse light
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightDistanceVec);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular light
	float specularLight = 0.7f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 10);
	float specular = specAmount * specularLight;

    vec4 baseLight = texture(diffuse0, texCoord) * (diffuse * intensity + ambient);
    float specLight = texture(specular0, texCoord).r * specular * intensity;

    return (baseLight + specLight) * lightColor;
}

vec4 directionalLight()
{
    // ambient light
    float ambient = 0.4f;

    // diffuse light
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular light
	float specularLight = 0.7f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 10);
	float specular = specAmount * specularLight;

    vec4 baseLight = texture(diffuse0, texCoord) * (diffuse + ambient);
    float specLight = texture(specular0, texCoord).r * specular;

    return (baseLight + specLight) * lightColor;
}

vec4 spotLight()
{
    vec3 lightDistanceVec = lightPos - currentPos;
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    // ambient light
    float ambient = 0.15f;

    // diffuse light
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightDistanceVec);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular light
	float specularLight = 0.7f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 10);
	float specular = specAmount * specularLight;

    // spot gradient
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    vec4 baseLight = texture(diffuse0, texCoord) * (diffuse * intensity + ambient);
    float specLight = texture(specular0, texCoord).r * specular * intensity;

    return (baseLight + specLight) * lightColor;
}

void main()
{
    //FragColor = pointLight();
    FragColor = directionalLight();
    //FragColor = spotLight();
}