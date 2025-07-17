#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 color;
out vec3 currentPos;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform float aspectRatio;

void main()
{
    currentPos = vec3(model * vec4(aPos, 1.0f));
    currentPos.x = currentPos.x / aspectRatio;
    currentPos.z = currentPos.z / aspectRatio;
    gl_Position = camMatrix * vec4(currentPos, 1.0);

    color = vec3(1.0f, 1.0f, 1.0f);
    texCoord = aTex;
    Normal = aNormal;
}