#include"Cube.hpp"

Cube::Cube(std::string name, std::vector <Texture>& textures) : Object(name)
{
    m_name = name;
    std::cout << "Creating " << m_name << std::endl;
    init(cube_verts, cube_normals, cube_texCoords, cube_faces, textures);
}