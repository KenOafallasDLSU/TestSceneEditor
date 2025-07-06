#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VertexArray.h"
#include"ElementArrayBuffer.h"
#include"Camera.h"
#include"Texture.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;
    // Store VAO in public so it can be used in the Draw function
    VertexArray vertexArray;

    // Initializes the mesh
    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

    // Draws the mesh
    void draw(Shader& shader, Camera& camera);
};
#endif