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

GLfloat triforceVertices[] = 
{ //               COORDINATES                  /     COLORS           //
	-0.8f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
	 0.2f,  0.3f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
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
	// VA must be bound before VB and EB are made
	vertexArray.bind();

	VertexBuffer vertexBuffer(triforceVertices, sizeof(triforceVertices));
	ElementArrayBuffer elementArrayBuffer(indices, sizeof(indices));

	vertexArray.linkAttrib(vertexBuffer, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vertexArray.linkAttrib(vertexBuffer, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	vertexArray.unbind();
	vertexBuffer.unbind();
	elementArrayBuffer.unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(244.0 / 255.0, 194.0 / 255.0, 194.0 / 255.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.activate();
		glUniform1f(uniID, 0.2f);

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