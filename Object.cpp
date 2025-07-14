#include"Object.hpp"

Object::Object() : shader("default.vert", "default.frag") { }

void Object::init(std::vector <glm::vec3>& shapeVerts, std::vector <glm::vec3>& normalVerts, std::vector <glm::vec2>& textureVerts, std::vector <Face>& faces, std::vector <Texture>& textures)
{
    mesh.init(shapeVerts, normalVerts, textureVerts, faces, textures);
    Object::m_textures = textures;

    std::cout << "Creating Object" << std::endl;

    transform();

    shader.activate();
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
}

void Object::draw(Camera& camera)
{
    mesh.draw(shader, camera);
}

void Object::setScale(glm::vec3 scale)
{
    m_scale = scale;
    transform();
}
void Object::setRotation(glm::vec3 rotation)
{
    m_rotation = rotation;
    transform();
}
void Object::setPosition(glm::vec3 position)
{
    m_position = position;
    transform();
}

void Object::transform()
{
    shader.activate();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, m_scale);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, m_position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Object::setTextures(std::vector <Texture>& textures)
{
    Object::m_textures = textures;
    mesh.setTextures(textures);
}