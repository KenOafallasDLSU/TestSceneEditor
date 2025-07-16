#pragma once

#include"Object.hpp"

class Sphere : public Object
{
public:
    std::vector <glm::vec3> sphere_verts;
    std::vector <glm::vec2> sphere_texCoords;
    std::vector <glm::vec3> sphere_normals;
    std::vector <Face> sphere_faces;

    Sphere(std::string name, std::vector <Texture>& textures);

    void buildVerticesSmooth();
};