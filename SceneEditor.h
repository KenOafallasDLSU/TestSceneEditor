#pragma once

#include <tiny_gltf/tiny_gltf.h>

class SceneEditor
{
public:
    SceneEditor(const char* gltfFilePath);

    int run();

private:
    bool loadGltfFile(tinygltf::Model& model);

    const char* m_gltfFilePath;
};