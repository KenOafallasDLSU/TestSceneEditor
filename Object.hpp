#pragma once

#include"Mesh.h"
#include"Shader.h"

class Object
{
public:
    Shader shader;
    Mesh mesh;

    std::vector <Texture> m_textures;

    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);

    std::string m_name = "Object";
    bool m_enabled = true;

    Object(std::string name);

    void init(std::vector <glm::vec3>& shapeVerts, std::vector <glm::vec3>& normalVerts, std::vector <glm::vec2>& textureVerts, std::vector <Face>& faces, std::vector <Texture>& textures);
    void draw(Camera& camera);

    void setScale(glm::vec3 scale);
    void setRotation(glm::vec3 rotation);
    void setPosition(glm::vec3 position);
    void transform();

    void setTextures(std::vector <Texture>& textures);
};