#pragma once

#include"Object.hpp"

class Wavefront : public Object
{
public:
    std::vector <glm::vec3> obj_verts;
    std::vector <glm::vec2> obj_texCoords;
    std::vector <glm::vec3> obj_normals;
    std::vector <Face> obj_faces;

    Wavefront(std::string filename, std::vector <Texture>& textures);

    void parseFile(const char* filename);
};