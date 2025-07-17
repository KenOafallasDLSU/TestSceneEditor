#include"ElementArrayBuffer.h"

// Constructor that generates a Elements Buffer Object and links it to indices
ElementArrayBuffer::ElementArrayBuffer(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void ElementArrayBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementArrayBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementArrayBuffer::tearDown()
{
	glDeleteBuffers(1, &ID);
}