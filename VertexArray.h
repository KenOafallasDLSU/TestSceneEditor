#ifndef VERTEXARRAY_CLASS_H
#define VERTEXARRAY_CLASS_H

#include<glad/glad.h>
#include"VertexBuffer.h"

class VertexArray
{
	public:
		GLuint ID;

		// Constructor that generates a VAO ID
		VertexArray();

		void linkAttrib(VertexBuffer& vertexBuffer, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		void linkVertexBuffer(VertexBuffer& vertexBuffer, GLuint layout);
		void bind() const;
		void unbind() const;
		void tearDown() const;
};
#endif