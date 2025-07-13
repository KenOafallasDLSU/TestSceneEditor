#pragma once

#include<string>

#include"VertexArray.h"
#include"ElementArrayBuffer.h"
#include"Camera.h"
#include"Texture.h"

struct FaceIndex
{
    int shapeInd;
    int textureInd;
    int normalInd;
};

struct Face
{
    std::vector <FaceIndex> faceIndices;
};

class Mesh
{
public:
    std::vector <glm::vec3> m_shapeVerts;
    std::vector <glm::vec3> m_normalVerts;
    std::vector <glm::vec2> m_textureVerts;
    std::vector <Face> m_faces;

    std::vector <Texture> textures;
    std::vector <GLuint> indices;

    // Store VAO in public so it can be used in the Draw function
    VertexArray vertexArray;

    Mesh();

    void init(std::vector <glm::vec3>& shapeVerts, std::vector <glm::vec3>& normalVerts, std::vector <glm::vec2>& textureVerts, std::vector <Face>& faces, std::vector <Texture>& textures);

    // Draws the mesh
    void draw(Shader& shader, Camera& camera);
};