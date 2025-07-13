#include"Plane.hpp"

Plane::Plane(std::vector <Texture>& textures)
{
    std::cout << "Creating Plane" << std::endl;
    init(plane_verts, plane_normals, plane_texCoords, plane_faces, textures);
}