#include"Mesh.h"

Mesh::Mesh() { }

void Mesh::init(std::vector <glm::vec3>& shapeVerts, std::vector <glm::vec3>& normalVerts, std::vector <glm::vec2>& textureVerts, std::vector <Face>& faces, std::vector <Texture>& textures)
{
    m_textures = textures;

    // VA must be bound before VB and EB are made
    vertexArray.bind();

    std::vector <Vertex> vertices;
    for (Face face : faces)
    {
        for (FaceIndex faceIndex : face.faceIndices)
        {
            int presentIndex = -1;
            
            Vertex newVertex{};
            newVertex.position = shapeVerts[faceIndex.shapeInd - 1];
            newVertex.normal = normalVerts[faceIndex.normalInd - 1];
            newVertex.texUV = textureVerts[faceIndex.textureInd - 1];

            for (int i = 0; i < vertices.size(); i++) {
                const Vertex vertex = vertices[i];

                if (glm::all(glm::equal(vertex.normal, newVertex.normal)) &&
                    glm::all(glm::equal(vertex.position, newVertex.position)) &&
                    glm::all(glm::equal(vertex.texUV, newVertex.texUV)))
                {
                    presentIndex = i;
                    break;
                }
            }

            if (presentIndex >= 0) {
                m_indices.push_back(presentIndex);
            }
            else {
                vertices.push_back(newVertex);
                m_indices.push_back(vertices.size() - 1);
            }
        }
    }

    std::cout << "Created Vertices: " << vertices.size() << std::endl;
    std::cout << "Created Indices: " << m_indices.size() << std::endl;
   
    VertexBuffer vertexBuffer(vertices);
    ElementArrayBuffer elementArrayBuffer(m_indices);

    vertexArray.linkAttrib(vertexBuffer, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    vertexArray.linkAttrib(vertexBuffer, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    vertexArray.linkAttrib(vertexBuffer, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));

    vertexArray.unbind();
    vertexBuffer.unbind();
    elementArrayBuffer.unbind();
}

// Draws the mesh
void Mesh::draw(Shader& shader, Camera& camera)
{
    shader.activate();
    vertexArray.bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        std::string num;
        std::string type = m_textures[i].type;
        if (type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular")
        {
            num = std::to_string(numSpecular++);
        }
        m_textures[i].texUnit(shader, (type + num).c_str(), i);
        m_textures[i].bind();
    }

    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.initMatrix(shader, "camMatrix");

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::setTextures(std::vector <Texture>& textures)
{
    m_textures = textures;
}