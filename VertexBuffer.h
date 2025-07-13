#pragma once

#include<glm/glm.hpp>
#include<glad/glad.h>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    //glm::vec3 color;
    glm::vec2 texUV;
};

class VertexBuffer
{
	public:
		GLuint ID;

		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VertexBuffer(std::vector<Vertex>& vertices);

		void bind() const;
		void unbind() const;
		void tearDown() const;
};