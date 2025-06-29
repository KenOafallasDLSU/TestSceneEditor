#include"VertexArray.h"

// Constructor that generates a VertexArray ID
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
}

// Links a VertexBuffer to the VertexArray using a certain layout
void VertexArray::linkVertexBuffer(VertexBuffer& vertexBuffer, GLuint layout)
{
	vertexBuffer.bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);
	vertexBuffer.unbind();
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