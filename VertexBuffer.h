#ifndef VERTEXBUFFER_CLASS_H
#define VERTEXBUFFER_CLASS_H

#include<glad/glad.h>

class VertexBuffer
{
	public:
		GLuint ID;

		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VertexBuffer(GLfloat* vertices, GLsizeiptr size);

		void bind() const;
		void unbind() const;
		void tearDown() const;
};

#endif