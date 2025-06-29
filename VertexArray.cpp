#include"VertexArray.h"

// Constructor that generates a VertexArray ID
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
}

void VertexArray::linkAttrib(VertexBuffer& vertexBuffer, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	vertexBuffer.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vertexBuffer.unbind();
}

// Links a VertexBuffer to the VertexArray using a certain layout
void VertexArray::linkVertexBuffer(VertexBuffer& vertexBuffer, GLuint layout)
{
	linkAttrib(vertexBuffer, layout, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
}

void VertexArray::bind() const
{
	glBindVertexArray(ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::tearDown() const
{
	glDeleteVertexArrays(1, &ID);
}