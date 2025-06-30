#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"Shader.h"
#include"VertexArray.h"
#include"VertexBuffer.h"
#include"ElementArrayBuffer.h"
#include"Texture.h"

int main()
{
	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
	};

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "KenSceneEditor", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	// Create buffer
	VertexArray vertexArray;
	// VA must be bound before VB and EB are made
	vertexArray.bind();

	VertexBuffer vertexBuffer(vertices, sizeof(vertices));
	ElementArrayBuffer elementArrayBuffer(indices, sizeof(indices));

	vertexArray.linkAttrib(vertexBuffer, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vertexArray.linkAttrib(vertexBuffer, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vertexArray.linkAttrib(vertexBuffer, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vertexArray.unbind();
	vertexBuffer.unbind();
	elementArrayBuffer.unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture texture("kotone256px.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "tex0", 0);
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(244.0 / 255.0, 194.0 / 255.0, 194.0 / 255.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.activate();
		glUniform1f(uniID, 0.2f);

		texture.bind();
		vertexArray.bind();

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Tear Down
	texture.tearDown();
	vertexArray.tearDown();
	vertexBuffer.tearDown();
	elementArrayBuffer.tearDown();
	shaderProgram.tearDown();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}