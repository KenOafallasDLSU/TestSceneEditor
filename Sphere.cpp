#include"Sphere.hpp"

struct SphereProperties {
    std::vector <glm::vec3> sphere_verts;
    std::vector <glm::vec2> sphere_texCoords;
    std::vector <glm::vec3> sphere_normals;
    std::vector <Face> sphere_faces;
};

SphereProperties buildVerticesSmooth()
{
    std::vector <glm::vec3> sphere_verts;
    std::vector <glm::vec2> sphere_texCoords;
    std::vector <glm::vec3> sphere_normals;
    std::vector <Face> sphere_faces;

    const float PI = acos(-1.0f);
    const float radius = 1.0f;
    const float sectorCount = 36.0f;
    const float stackCount = 18.0f;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            sphere_verts.push_back(glm::vec3(x, y, z));

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            sphere_normals.push_back(glm::vec3(nx, ny, nz));

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            sphere_texCoords.push_back(glm::vec2(s, t));
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                sphere_faces.push_back(Face{
                    std::vector <FaceIndex> {
                        FaceIndex{k1 + 1, k1 + 1, k1 + 1},
                        FaceIndex{k2 + 1, k2 + 1, k2 + 1},
                        FaceIndex{k1 + 2, k1 + 2, k1 + 2}
                    }
                    });  // k1---k2---k1+1
            }

            if (i != (stackCount - 1))
            {
                sphere_faces.push_back(Face{
                    std::vector <FaceIndex> {
                        FaceIndex{k1 + 2, k1 + 2, k1 + 2},
                        FaceIndex{k2 + 1, k2 + 1, k2 + 1},
                        FaceIndex{k2 + 2, k2 + 2, k2 + 2}
                    }
                 }); // k1+1---k2---k2+1
            }
        }
    }

    return SphereProperties{
        sphere_verts,
        sphere_texCoords,
        sphere_normals,
        sphere_faces
    };
}

SphereProperties sphereProperties = buildVerticesSmooth();

Sphere::Sphere(std::vector <Texture>& textures) :
    Object(sphereProperties.sphere_verts, sphereProperties.sphere_normals, sphereProperties.sphere_texCoords, sphereProperties.sphere_faces, textures)
{
    std::cout << "Creating Sphere" << std::endl;
}