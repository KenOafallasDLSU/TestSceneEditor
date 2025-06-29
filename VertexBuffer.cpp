#include"VertexBuffer.h"

VertexBuffer::VertexBuffer(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::tearDown() const
{
	glDeleteBuffers(1, &ID);
}