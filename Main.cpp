#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Shader.h"
#include"VertexArray.h"
#include"VertexBuffer.h"
#include"ElementArrayBuffer.h"

GLfloat triangleVertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
};

GLfloat triforceVertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,

		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,
};

GLuint indices[] = {
	0, 3, 5,
	3, 2, 4,
	5, 4, 1
};

int main()
{
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
	vertexArray.bind();

	VertexBuffer vertexBuffer(triforceVertices, sizeof(triforceVertices));
	vertexArray.linkVertexBuffer(vertexBuffer, 0);

	ElementArrayBuffer elementArrayBuffer(indices, sizeof(indices));

	vertexArray.unbind();
	vertexBuffer.unbind();
	elementArrayBuffer.unbind();
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(244.0 / 255.0, 194.0 / 255.0, 194.0 / 255.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.activate();
		vertexArray.bind();

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Tear Down
	vertexArray.tearDown();
	vertexBuffer.tearDown();
	elementArrayBuffer.tearDown();
	shaderProgram.tearDown();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}