#include"Wavefront.hpp"
#include<fstream>

void Wavefront::parseFile(const char* filename)
{
    std::ifstream file(filename);
    std::string lineHeader;
    while (std::getline(file, lineHeader))
    {
        int res;

        // check if position vertex
        if (strcmp(lineHeader.substr(0, 2).c_str(), "v ") == 0) {
            glm::vec3 vertex;
            res = sscanf_s(lineHeader.substr(2).c_str(), "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            //std::cout << lineHeader.substr(2) << " " << res << std::endl;
            obj_verts.push_back(vertex);
        }
        // check if texture vertex
        else if (strcmp(lineHeader.substr(0, 2).c_str(), "vt") == 0) {
            glm::vec2 uv;
            res = sscanf_s(lineHeader.substr(3).c_str(), "%f %f", &uv.x, &uv.y);
            //std::cout << lineHeader.substr(3) << " " << res << std::endl;
            obj_texCoords.push_back(uv);
        }
        // check if normal
        else if (strcmp(lineHeader.substr(0, 2).c_str(), "vn") == 0) {
            glm::vec3 normal;
            res = sscanf_s(lineHeader.substr(3).c_str(), "%f %f %f", &normal.x, &normal.y, &normal.z);
            //std::cout << lineHeader.substr(3) << " " << res << std::endl;
            obj_normals.push_back(normal);
        }
        // check if face
        else if (strcmp(lineHeader.substr(0, 2).c_str(), "f ") == 0) {
            int vertexIndex[4], uvIndex[4], normalIndex[4];
            res = sscanf_s(lineHeader.substr(2).c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
            //std::cout << lineHeader.substr(2) << " " << res << std::endl;
            obj_faces.push_back(Face{
                std::vector <FaceIndex> {
                    FaceIndex{vertexIndex[0], uvIndex[0], normalIndex[0]},
                    FaceIndex{vertexIndex[1], uvIndex[1], normalIndex[1]},
                    FaceIndex{vertexIndex[2], uvIndex[2], normalIndex[2]}
                }
            });

            if (res == 12) {
                obj_faces.push_back(Face{
                std::vector <FaceIndex> {
                        FaceIndex{vertexIndex[0], uvIndex[0], normalIndex[0]},
                        FaceIndex{vertexIndex[2], uvIndex[2], normalIndex[2]},
                        FaceIndex{vertexIndex[3], uvIndex[3], normalIndex[3]}
                    }
                 });
            }
        }
    }

    std::cout << "Exited Parsing" << std::endl;
}

Wavefront::Wavefront(std::string filename, std::vector <Texture>&textures) : Object(filename)
{
    std::cout << "Importing Wavefront OBJ" << std::endl;
    parseFile(filename.c_str());
    init(obj_verts, obj_normals, obj_texCoords, obj_faces, textures);
}