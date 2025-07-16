#pragma once

#include <tiny_gltf/tiny_gltf.h>

#include"Texture.h"
#include"VertexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"
#include"Mesh.h"
#include "GLFWHandler.hpp"
#include"shaders.hpp"

class GLTFViewer
{
public:
    GLuint ID;

    GLTFViewer(Camera& camera);

    int init(const char* gltfFilePath);
    void drawScene(Camera& camera);
    void bindMaterial(int materialIndex);

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
    tinygltf::Model model;
    Camera m_camera;

    glm::vec3 lightDirection;
    glm::vec3 lightIntensity;
    float maxDistance;
    glm::mat4 projMatrix;

    std::vector<GLuint> bufferObjects;
    std::vector<VaoRange> meshToVertexArrays;
    std::vector<GLuint> vertexArrayObjects;
    std::vector<GLuint> textureObjects;
    GLuint whiteTexture;

    // UniformLocations
    int modelViewProjMatrixLocation;
    int modelViewMatrixLocation;
    int normalMatrixLocation;
    int uLightDirectionLocation;
    int uLightIntensity;
    int uBaseColorTexture;
    int uBaseColorFactor;
    int uMetallicRoughnessTexture;
    int uMetallicFactor;
    int uRoughnessFactor;
    int uEmissiveTexture;
    int uEmissiveFactor;
    int uOcclusionTexture;
    int uOcclusionStrength;
    int uApplyOcclusion;

    std::vector<GLuint> createTextureObjects(const tinygltf::Model& model) const;

    std::vector<GLuint> createBufferObjects(const tinygltf::Model& model) const;

    std::vector<GLuint> createVertexArrayObjects(const tinygltf::Model& model,
        const std::vector<GLuint>& bufferObjects,
        std::vector<VaoRange>& meshToVertexArrays) const;
};