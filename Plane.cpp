#include"Plane.hpp"

Plane::Plane(std::string name, std::vector <Texture>& textures) : Object(name)
{
    std::cout << "Creating Plane" << std::endl;
    init(plane_verts, plane_normals, plane_texCoords, plane_faces, textures);
}