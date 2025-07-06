#ifndef ELEMENTARRAYBUFFER_CLASS_H
#define ELEMENTARRAYBUFFER_CLASS_H

#include<glad/glad.h>
#include<vector>

class ElementArrayBuffer
{
	public:
		GLuint ID;

		// Constructor that generates a Elements Buffer Object and links it to indices
		ElementArrayBuffer(std::vector<GLuint>& indices);

		void bind();
		void unbind();
		void tearDown();
};

#endif