#pragma once

#include"Object.hpp"

class Plane : public Object
{
public:
    std::vector <glm::vec3> plane_verts =
    {
        glm::vec3(-10.0, 0.0, 10.0),
        glm::vec3(-10.0, 0.0, -10.0),
        glm::vec3(10.0, 0.0, -10.0),
        glm::vec3(10.0, 0.0, 10.0),
    };

    std::vector <glm::vec2> plane_texCoords =
    {
        glm::vec2(0.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 1.0),
        glm::vec2(1.0, 0.0)
    };

    std::vector <glm::vec3> plane_normals =
    {
        glm::vec3(0, 1, 0)
    };

    std::vector <Face> plane_faces =
    {
        Face{
            std::vector <FaceIndex> {
                FaceIndex{1, 1, 1},
                FaceIndex{2, 2, 1},
                FaceIndex{3, 3, 1}
            }
        },
        Face{
            std::vector <FaceIndex> {
                FaceIndex{1, 1, 1},
                FaceIndex{3, 3, 1},
                FaceIndex{4, 4, 1}
            }
        }
    };

    Plane(std::vector <Texture>& textures);
};