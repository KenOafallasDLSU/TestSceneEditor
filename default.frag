#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 currentPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
    // ambient light
    float ambient = 0.15f;

    // diffuse light
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - currentPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular light
	float specularLight = 0.7f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 10);
	float specular = specAmount * specularLight;

    FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}