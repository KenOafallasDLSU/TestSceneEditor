#include"Cube.hpp"

std::vector <glm::vec3> cube_verts =
{
    glm::vec3(0.5, -0.5, -0.5),
    glm::vec3(0.5, -0.5, 0.5),
    glm::vec3(-0.5, -0.5, 0.5),
    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3(0.5, 0.5, -0.5),
    glm::vec3(0.5, 0.5, 0.5),
    glm::vec3(-0.5, 0.5, 0.5),
    glm::vec3(-0.5, 0.5, -0.5)
};

std::vector <glm::vec2> cube_texCoords =
{
    glm::vec2(0.0, 0.0),
    glm::vec2(1.0, 0.0),
    glm::vec2(1.0, 1.0),
    glm::vec2(0.0, 1.0)
};

std::vector <glm::vec3> cube_normals =
{
    glm::vec3(0, -1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(-0, -0, 1),
    glm::vec3(-1, -0, -0),
    glm::vec3(0, 0, -1),
    glm::vec3(1, -0, 0)
};

std::vector <Face> cube_faces =
{
    Face{
        std::vector <FaceIndex> {
            FaceIndex{2, 1, 1},
            FaceIndex{3, 2, 1},
            FaceIndex{4, 3, 1}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{5, 3, 2},
            FaceIndex{8, 4, 2},
            FaceIndex{7, 1, 2}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{5, 3, 3},
            FaceIndex{6, 4, 3},
            FaceIndex{2, 1, 3}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{2, 2, 4},
            FaceIndex{6, 3, 4},
            FaceIndex{7, 4, 4}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{7, 3, 5},
            FaceIndex{8, 4, 5},
            FaceIndex{4, 1, 5}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{5, 4, 6},
            FaceIndex{1, 1, 6},
            FaceIndex{4, 2, 6}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{1, 4, 1},
            FaceIndex{2, 1, 1},
            FaceIndex{4, 3, 1}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{6, 2, 2},
            FaceIndex{5, 3, 2},
            FaceIndex{7, 1, 2}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{1, 2, 7},
            FaceIndex{5, 3, 7},
            FaceIndex{2, 1, 7}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{3, 1, 4},
            FaceIndex{2, 2, 4},
            FaceIndex{7, 4, 4}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{3, 2, 5},
            FaceIndex{7, 3, 5},
            FaceIndex{4, 1, 5}
        }
    },
    Face{
        std::vector <FaceIndex> {
            FaceIndex{8, 3, 6},
            FaceIndex{5, 4, 6},
            FaceIndex{4, 2, 6}
        }
    }
};

Cube::Cube(std::vector <Texture>& textures) : 
    Object(cube_verts, cube_normals, cube_texCoords, cube_faces, textures)
{
    std::cout << "Creating Cube" << std::endl;
}