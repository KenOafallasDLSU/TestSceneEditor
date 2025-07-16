#pragma once

#include <tiny_gltf/tiny_gltf.h>

#include"Texture.h"
#include"VertexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"
#include"Mesh.h"
#include "GLFWHandler.hpp"

class GLTFViewer
{
public:
    GLTFViewer(const char* gltfFilePath);

    int run();

private:
    // A range of indices in a vector containing Vertex Array Objects
    struct VaoRange
    {
        GLsizei begin; // Index of first element in vertexArrayObjects
        GLsizei count; // Number of elements in range
    };

    bool loadGltfFile(tinygltf::Model& model);

    GLsizei m_nWindowWidth = 1280;
    GLsizei m_nWindowHeight = 720;
    const char* m_gltfFilePath;
    std::string m_vertexShader = "forward.vert";
    std::string m_fragmentShader = "pbr.frag";

    std::vector<GLuint> createTextureObjects(const tinygltf::Model& model) const;

    std::vector<GLuint> createBufferObjects(const tinygltf::Model& model) const;

    std::vector<GLuint> createVertexArrayObjects(const tinygltf::Model& model,
        const std::vector<GLuint>& bufferObjects,
        std::vector<VaoRange>& meshToVertexArrays) const;

    GLFWHandle m_GLFWHandle{ int(m_nWindowWidth), int(m_nWindowHeight),
      "glTF Viewer",
      true };
};