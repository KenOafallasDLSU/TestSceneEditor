#include"SceneEditor.h"

#include <iostream>
#include <numeric>

SceneEditor::SceneEditor(const char* gltfFilePath) :
    m_gltfFilePath{ gltfFilePath }
{
    
}

int SceneEditor::run() {
    tinygltf::Model model;
    if (!loadGltfFile(model)) {
        return -1;
    }
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
