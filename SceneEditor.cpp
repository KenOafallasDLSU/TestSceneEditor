#include"SceneEditor.h"

#include <iostream>
#include <numeric>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "gltf.h"
#include"Camera.h"
#include"shaders.hpp"


#include <tiny_gltf/tiny_gltf.h>

SceneEditor::SceneEditor(const char* gltfFilePath) :
    m_gltfFilePath{ gltfFilePath }
{
    
}

int SceneEditor::run()
{
    // Loader shaders
    const auto glslProgram =
        compileProgram({ m_vertexShader, m_fragmentShader });

    const auto modelViewProjMatrixLocation =
        glGetUniformLocation(glslProgram.glId(), "uModelViewProjMatrix");
    const auto modelViewMatrixLocation =
        glGetUniformLocation(glslProgram.glId(), "uModelViewMatrix");
    const auto normalMatrixLocation =
        glGetUniformLocation(glslProgram.glId(), "uNormalMatrix");

    const auto uLightDirectionLocation =
        glGetUniformLocation(glslProgram.glId(), "uLightDirection");
    const auto uLightIntensity =
        glGetUniformLocation(glslProgram.glId(), "uLightIntensity");

    const auto uBaseColorTexture =
        glGetUniformLocation(glslProgram.glId(), "uBaseColorTexture");
    const auto uBaseColorFactor =
        glGetUniformLocation(glslProgram.glId(), "uBaseColorFactor");

    const auto uMetallicRoughnessTexture =
        glGetUniformLocation(glslProgram.glId(), "uMetallicRoughnessTexture");
    const auto uMetallicFactor =
        glGetUniformLocation(glslProgram.glId(), "uMetallicFactor");
    const auto uRoughnessFactor =
        glGetUniformLocation(glslProgram.glId(), "uRoughnessFactor");

    const auto uEmissiveTexture =
        glGetUniformLocation(glslProgram.glId(), "uEmissiveTexture");
    const auto uEmissiveFactor =
        glGetUniformLocation(glslProgram.glId(), "uEmissiveFactor");

    const auto uOcclusionTexture =
        glGetUniformLocation(glslProgram.glId(), "uOcclusionTexture");
    const auto uOcclusionStrength =
        glGetUniformLocation(glslProgram.glId(), "uOcclusionStrength");
    const auto uApplyOcclusion =
        glGetUniformLocation(glslProgram.glId(), "uApplyOcclusion");

    tinygltf::Model model;
    if (!loadGltfFile(model)) {
        return -1;
    }
    glm::vec3 bboxMin, bboxMax;
    computeSceneBounds(model, bboxMin, bboxMax);

    // Build projection matrix
    const auto diag = bboxMax - bboxMin;
    auto maxDistance = glm::length(diag);
    const auto projMatrix =
        glm::perspective(70.f, float(m_nWindowWidth) / m_nWindowHeight,
            0.001f * maxDistance, 1.5f * maxDistance);

    /*
    std::unique_ptr<CameraController> cameraController =
        std::make_unique<TrackballCameraController>(
            m_GLFWHandle.window(), 0.5f * maxDistance);
    if (m_hasUserCamera) {
        cameraController->setCamera(m_userCamera);
    }
    else {
        const auto center = 0.5f * (bboxMax + bboxMin);
        const auto up = glm::vec3(0, 1, 0);
        const auto eye =
            diag.z > 0 ? center + diag : center + 2.f * glm::cross(diag, up);
        cameraController->setCamera(Camera{ eye, center, up });
    }
    */

    // Init light parameters
    glm::vec3 lightDirection(1, 1, 1);
    glm::vec3 lightIntensity(1, 1, 1);
    bool lightFromCamera = false;
    bool applyOcclusion = true;

    // Load textures
    const auto textureObjects = createTextureObjects(model);

    GLuint whiteTexture = 0;

    // Create white texture for object with no base color texture
    glGenTextures(1, &whiteTexture);
    glBindTexture(GL_TEXTURE_2D, whiteTexture);
    float white[] = { 1, 1, 1, 1 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    const auto bufferObjects = createBufferObjects(model);

    std::vector<VaoRange> meshToVertexArrays;
    const auto vertexArrayObjects =
        createVertexArrayObjects(model, bufferObjects, meshToVertexArrays);

    // Setup OpenGL state for rendering
    glEnable(GL_DEPTH_TEST);
    glslProgram.use();

    const auto bindMaterial = [&](const auto materialIndex) {
        if (materialIndex >= 0) {
            const auto& material = model.materials[materialIndex];
            const auto& pbrMetallicRoughness = material.pbrMetallicRoughness;
            if (uBaseColorFactor >= 0) {
                glUniform4f(uBaseColorFactor,
                    (float)pbrMetallicRoughness.baseColorFactor[0],
                    (float)pbrMetallicRoughness.baseColorFactor[1],
                    (float)pbrMetallicRoughness.baseColorFactor[2],
                    (float)pbrMetallicRoughness.baseColorFactor[3]);
            }
            if (uBaseColorTexture >= 0) {
                auto textureObject = whiteTexture;
                if (pbrMetallicRoughness.baseColorTexture.index >= 0) {
                    const auto& texture =
                        model.textures[pbrMetallicRoughness.baseColorTexture.index];
                    if (texture.source >= 0) {
                        textureObject = textureObjects[texture.source];
                    }
                }

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureObject);
                glUniform1i(uBaseColorTexture, 0);
            }
            if (uMetallicFactor >= 0) {
                glUniform1f(
                    uMetallicFactor, (float)pbrMetallicRoughness.metallicFactor);
            }
            if (uRoughnessFactor >= 0) {
                glUniform1f(
                    uRoughnessFactor, (float)pbrMetallicRoughness.roughnessFactor);
            }
            if (uMetallicRoughnessTexture >= 0) {
                auto textureObject = 0u;
                if (pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
                    const auto& texture =
                        model.textures[pbrMetallicRoughness.metallicRoughnessTexture
                        .index];
                    if (texture.source >= 0) {
                        textureObject = textureObjects[texture.source];
                    }
                }

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, textureObject);
                glUniform1i(uMetallicRoughnessTexture, 1);
            }
            if (uEmissiveFactor >= 0) {
                glUniform3f(uEmissiveFactor, (float)material.emissiveFactor[0],
                    (float)material.emissiveFactor[1],
                    (float)material.emissiveFactor[2]);
            }
            if (uEmissiveTexture >= 0) {
                auto textureObject = 0u;
                if (material.emissiveTexture.index >= 0) {
                    const auto& texture = model.textures[material.emissiveTexture.index];
                    if (texture.source >= 0) {
                        textureObject = textureObjects[texture.source];
                    }
                }

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, textureObject);
                glUniform1i(uEmissiveTexture, 2);
            }
            if (uOcclusionStrength >= 0) {
                glUniform1f(
                    uOcclusionStrength, (float)material.occlusionTexture.strength);
            }
            if (uOcclusionTexture >= 0) {
                auto textureObject = whiteTexture;
                if (material.occlusionTexture.index >= 0) {
                    const auto& texture = model.textures[material.occlusionTexture.index];
                    if (texture.source >= 0) {
                        textureObject = textureObjects[texture.source];
                    }
                }

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, textureObject);
                glUniform1i(uOcclusionTexture, 3);
            }
        }
        else {
            // Apply default material
            // Defined here:
            // https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#reference-material
            // https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#reference-pbrmetallicroughness3
            if (uBaseColorFactor >= 0) {
                glUniform4f(uBaseColorFactor, 1, 1, 1, 1);
            }
            if (uBaseColorTexture >= 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, whiteTexture);
                glUniform1i(uBaseColorTexture, 0);
            }
            if (uMetallicFactor >= 0) {
                glUniform1f(uMetallicFactor, 1.f);
            }
            if (uRoughnessFactor >= 0) {
                glUniform1f(uRoughnessFactor, 1.f);
            }
            if (uMetallicRoughnessTexture >= 0) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, 0);
                glUniform1i(uMetallicRoughnessTexture, 1);
            }
            if (uEmissiveFactor >= 0) {
                glUniform3f(uEmissiveFactor, 0.f, 0.f, 0.f);
            }
            if (uEmissiveTexture >= 0) {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, 0);
                glUniform1i(uEmissiveTexture, 2);
            }
            if (uOcclusionStrength >= 0) {
                glUniform1f(uOcclusionStrength, 0.f);
            }
            if (uOcclusionTexture >= 0) {
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, 0);
                glUniform1i(uOcclusionTexture, 3);
            }
        }
        };

    // Lambda function to draw the scene
    const auto drawScene = [&](Camera& camera) {
        glViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.inputs(m_GLFWHandle.window());
        camera.updateMatrix(70.0f, 0.001f * maxDistance, 1.5f * maxDistance);
        const auto viewMatrix = camera.view;

        //std::cout << "SHADER_COMPILATION_SUCCESS for:" << glm::mat4(1.0f) << std::endl;

        if (uLightDirectionLocation >= 0) {
            if (lightFromCamera) {
                glUniform3f(uLightDirectionLocation, 0, 0, 1);
            }
            else {
                const auto lightDirectionInViewSpace = glm::normalize(
                    glm::vec3(viewMatrix * glm::vec4(lightDirection, 0.)));
                glUniform3f(uLightDirectionLocation, lightDirectionInViewSpace[0],
                    lightDirectionInViewSpace[1], lightDirectionInViewSpace[2]);
            }
        }

        if (uLightIntensity >= 0) {
            glUniform3f(uLightIntensity, lightIntensity[0], lightIntensity[1],
                lightIntensity[2]);
        }

        if (uApplyOcclusion >= 0) {
            glUniform1i(uApplyOcclusion, applyOcclusion);
        }

        // The recursive function that should draw a node
        // We use a std::function because a simple lambda cannot be recursive
        const std::function<void(int, const glm::mat4&)> drawNode =
            [&](int nodeIdx, const glm::mat4& parentMatrix) {
            const auto& node = model.nodes[nodeIdx];
            const glm::mat4 modelMatrix =
                getLocalToWorldMatrix(node, parentMatrix);

            // If the node references a mesh (a node can also reference a
            // camera, or a light)
            if (node.mesh >= 0) {
                const auto mvMatrix =
                    viewMatrix * modelMatrix; // Also called localToCamera matrix
                const auto mvpMatrix =
                    projMatrix * mvMatrix; // Also called localToScreen matrix
                // Normal matrix is necessary to maintain normal vectors
                // orthogonal to tangent vectors
                // https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
                const auto normalMatrix = glm::transpose(glm::inverse(mvMatrix));

                glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE,
                    glm::value_ptr(mvpMatrix));
                glUniformMatrix4fv(
                    modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
                glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE,
                    glm::value_ptr(normalMatrix));

                const auto& mesh = model.meshes[node.mesh];
                const auto& vaoRange = meshToVertexArrays[node.mesh];
                for (size_t pIdx = 0; pIdx < mesh.primitives.size(); ++pIdx) {
                    const auto vao = vertexArrayObjects[vaoRange.begin + pIdx];
                    const auto& primitive = mesh.primitives[pIdx];

                    bindMaterial(primitive.material);

                    glBindVertexArray(vao);
                    if (primitive.indices >= 0) {
                        const auto& accessor = model.accessors[primitive.indices];
                        const auto& bufferView = model.bufferViews[accessor.bufferView];
                        const auto byteOffset =
                            accessor.byteOffset + bufferView.byteOffset;
                        glDrawElements(primitive.mode, GLsizei(accessor.count),
                            accessor.componentType, (const GLvoid*)byteOffset);
                    }
                    else {
                        // Take first accessor to get the count
                        const auto accessorIdx = (*begin(primitive.attributes)).second;
                        const auto& accessor = model.accessors[accessorIdx];
                        glDrawArrays(primitive.mode, 0, GLsizei(accessor.count));
                    }
                }
            }

            // Draw children
            for (const auto childNodeIdx : node.children) {
                drawNode(childNodeIdx, modelMatrix);
            }
            };

        // Draw the scene referenced by gltf file
        if (model.defaultScene >= 0) {
            for (const auto nodeIdx : model.scenes[model.defaultScene].nodes) {
                drawNode(nodeIdx, glm::mat4(1));
            }
        }
        };

    /*
    Vertex vertices[] =
    { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
        Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 5.0f)},
        Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 5.0f)},
        Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 0.0f)},

        Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(-0.8f, 0.5f,  0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(-0.8f, 0.5f,  0.0f), glm::vec2(5.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec3(-0.8f, 0.5f,  0.0f), glm::vec2(2.5f, 5.0f)},

        Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(5.0f, 0.0f)},
        Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(2.5f, 5.0f)},

        Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(5.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(2.5f, 5.0f)},

        Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(5.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(2.5f, 5.0f)},
    };

    GLuint indices[] =
    {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    };
    Texture textures[]
    {
        Texture("kotone256px.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    // Test Pyramid Object
    Shader shaderProgram("default.vert", "default.frag");
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh kotonyd(verts, ind, tex);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //glm::vec4 lightColor = glm::vec4(244.0 / 255.0, 194.0 / 255.0, 194.0 / 255.0, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    shaderProgram.activate();

    //vertex uniforms
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));

    //fragment uniforms
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    */
    Camera camera(m_nWindowWidth, m_nWindowHeight, glm::vec3(0.0f, 0.0f, 2.0f));
    // Loop until the user closes the window
    while (true) {
        const auto seconds = glfwGetTime();

        drawScene(camera);
        //kotonyd.draw(shaderProgram, camera);

        glfwPollEvents(); // Poll for and process events

        m_GLFWHandle.swapBuffers(); // Swap front and back buffers
    }

    // TODO clean up allocated GL data

    return 0;
}

bool SceneEditor::loadGltfFile(tinygltf::Model& model)
{
    std::clog << "Loading file " << m_gltfFilePath << std::endl;

    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, m_gltfFilePath);
    std::clog << "Tex Size: " << model.textures.size() << std::endl;

    if (!warn.empty()) {
        std::cerr << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to parse glTF file" << std::endl;
        return false;
    }

    return true;
}

std::vector<GLuint> SceneEditor::createTextureObjects(
    const tinygltf::Model& model) const
{
    std::vector<GLuint> textureObjects(model.textures.size(), 0);

    tinygltf::Sampler defaultSampler;
    defaultSampler.minFilter = GL_LINEAR;
    defaultSampler.magFilter = GL_LINEAR;
    defaultSampler.wrapS = GL_REPEAT;
    defaultSampler.wrapT = GL_REPEAT;

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(GLsizei(model.textures.size()), textureObjects.data());
    for (size_t i = 0; i < model.textures.size(); ++i) {
        const auto& texture = model.textures[i];
        assert(texture.source >= 0);
        const tinygltf::Image& image = model.images[texture.source];

        std::cout << "SHADER_COMPILATION_SUCCESS for:" << image.uri << std::endl;

        const auto& sampler =
            texture.sampler >= 0 ? model.samplers[texture.sampler] : defaultSampler;
        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
            GL_RGBA, image.pixel_type, image.image.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            sampler.minFilter != -1 ? sampler.minFilter : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
            sampler.magFilter != -1 ? sampler.magFilter : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);

        if (sampler.minFilter == GL_NEAREST_MIPMAP_NEAREST ||
            sampler.minFilter == GL_NEAREST_MIPMAP_LINEAR ||
            sampler.minFilter == GL_LINEAR_MIPMAP_NEAREST ||
            sampler.minFilter == GL_LINEAR_MIPMAP_LINEAR) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureObjects;
}

std::vector<GLuint> SceneEditor::createBufferObjects(
    const tinygltf::Model& model) const
{
    std::vector<GLuint> bufferObjects(model.buffers.size(), 0);

    glGenBuffers(GLsizei(model.buffers.size()), bufferObjects.data());
    for (size_t i = 0; i < model.buffers.size(); ++i) {
        glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[i]);
        glBufferStorage(GL_ARRAY_BUFFER, model.buffers[i].data.size(),
            model.buffers[i].data.data(), 0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return bufferObjects;
}

std::vector<GLuint> SceneEditor::createVertexArrayObjects(
    const tinygltf::Model& model, const std::vector<GLuint>& bufferObjects,
    std::vector<VaoRange>& meshToVertexArrays) const
{
    std::vector<GLuint> vertexArrayObjects; // We don't know the size yet

    // For each mesh of model we keep its range of VAOs
    meshToVertexArrays.resize(model.meshes.size());

    const GLuint VERTEX_ATTRIB_POSITION_IDX = 0;
    const GLuint VERTEX_ATTRIB_NORMAL_IDX = 1;
    const GLuint VERTEX_ATTRIB_TEXCOORD0_IDX = 2;

    for (size_t i = 0; i < model.meshes.size(); ++i) {
        const auto& mesh = model.meshes[i];

        auto& vaoRange = meshToVertexArrays[i];
        vaoRange.begin =
            GLsizei(vertexArrayObjects.size()); // Range for this mesh will be at
        // the end of vertexArrayObjects
        vaoRange.count =
            GLsizei(mesh.primitives.size()); // One VAO for each primitive

        // Add enough elements to store our VAOs identifiers
        vertexArrayObjects.resize(
            vertexArrayObjects.size() + mesh.primitives.size());

        glGenVertexArrays(vaoRange.count, &vertexArrayObjects[vaoRange.begin]);
        for (size_t pIdx = 0; pIdx < mesh.primitives.size(); ++pIdx) {
            const auto vao = vertexArrayObjects[vaoRange.begin + pIdx];
            const auto& primitive = mesh.primitives[pIdx];
            glBindVertexArray(vao);
            { // POSITION attribute
              // scope, so we can declare const variable with the same name on each
              // scope
                const auto iterator = primitive.attributes.find("POSITION");
                if (iterator != end(primitive.attributes)) {
                    const auto accessorIdx = (*iterator).second;
                    const auto& accessor = model.accessors[accessorIdx];
                    const auto& bufferView = model.bufferViews[accessor.bufferView];
                    const auto bufferIdx = bufferView.buffer;

                    glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION_IDX);
                    assert(GL_ARRAY_BUFFER == bufferView.target);
                    // Theorically we could also use bufferView.target, but it is safer
                    // Here it is important to know that the next call
                    // (glVertexAttribPointer) use what is currently bound
                    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[bufferIdx]);

                    // tinygltf converts strings type like "VEC3, "VEC2" to the number of
                    // components, stored in accessor.type
                    const auto byteOffset = accessor.byteOffset + bufferView.byteOffset;
                    glVertexAttribPointer(VERTEX_ATTRIB_POSITION_IDX, accessor.type,
                        accessor.componentType, GL_FALSE, GLsizei(bufferView.byteStride),
                        (const GLvoid*)byteOffset);
                }
            }
            // todo Refactor to remove code duplication (loop over "POSITION",
            // "NORMAL" and their corresponding VERTEX_ATTRIB_*)
            { // NORMAL attribute
                const auto iterator = primitive.attributes.find("NORMAL");
                if (iterator != end(primitive.attributes)) {
                    const auto accessorIdx = (*iterator).second;
                    const auto& accessor = model.accessors[accessorIdx];
                    const auto& bufferView = model.bufferViews[accessor.bufferView];
                    const auto bufferIdx = bufferView.buffer;

                    glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL_IDX);
                    assert(GL_ARRAY_BUFFER == bufferView.target);
                    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[bufferIdx]);
                    glVertexAttribPointer(VERTEX_ATTRIB_NORMAL_IDX, accessor.type,
                        accessor.componentType, GL_FALSE, GLsizei(bufferView.byteStride),
                        (const GLvoid*)(accessor.byteOffset + bufferView.byteOffset));
                }
            }
            { // TEXCOORD_0 attribute
                const auto iterator = primitive.attributes.find("TEXCOORD_0");
                if (iterator != end(primitive.attributes)) {
                    const auto accessorIdx = (*iterator).second;
                    const auto& accessor = model.accessors[accessorIdx];
                    const auto& bufferView = model.bufferViews[accessor.bufferView];
                    const auto bufferIdx = bufferView.buffer;

                    glEnableVertexAttribArray(VERTEX_ATTRIB_TEXCOORD0_IDX);
                    assert(GL_ARRAY_BUFFER == bufferView.target);
                    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[bufferIdx]);
                    glVertexAttribPointer(VERTEX_ATTRIB_TEXCOORD0_IDX, accessor.type,
                        accessor.componentType, GL_FALSE, GLsizei(bufferView.byteStride),
                        (const GLvoid*)(accessor.byteOffset + bufferView.byteOffset));
                }
            }
            // Index array if defined
            if (primitive.indices >= 0) {
                const auto accessorIdx = primitive.indices;
                const auto& accessor = model.accessors[accessorIdx];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const auto bufferIdx = bufferView.buffer;

                assert(GL_ELEMENT_ARRAY_BUFFER == bufferView.target);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                    bufferObjects[bufferIdx]); // Binding the index buffer to
                // GL_ELEMENT_ARRAY_BUFFER while the VAO
                // is bound is enough to tell OpenGL we
                // want to use that index buffer for that
                // VAO
            }
        }
    }
    glBindVertexArray(0);

    std::clog << "Number of VAOs: " << vertexArrayObjects.size() << std::endl;

    return vertexArrayObjects;
}