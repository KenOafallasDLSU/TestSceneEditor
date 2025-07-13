#include"Cube.hpp"

Cube::Cube(std::vector <Texture>& textures)
{
    std::cout << "Creating Cube" << std::endl;
    init(cube_verts, cube_normals, cube_texCoords, cube_faces, textures);
}