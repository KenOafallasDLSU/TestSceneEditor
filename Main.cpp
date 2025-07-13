#include"Mesh.h"
#include"SceneEditor.h"
#include<tiny_gltf/tiny_gltf.h>

#include"Cube.hpp"
#include"Plane.hpp"

int main()
{
	// Vertices coordinates

    //SceneEditor sceneEditor("models/grindstone/scene.gltf");
 /*   SceneEditor sceneEditor("models/main_sponza/NewSponza_Main_glTF_003.gltf");
    sceneEditor.run();*/

    tinygltf::Model model;

	const unsigned int width = 800;
	const unsigned int height = 800;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "KenSceneEditor", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

    
    Texture textures[]
    {
        Texture("kotone256px.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    std::vector <Object> scene;
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    Plane plane(tex);
    plane.setPosition(glm::vec3(0, -5, 0));
    scene.push_back(plane);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

        for (Object object : scene)
        {
            object.draw(camera);
        }

        if (scene.size() < 10) {
            Cube newCube(tex);
            newCube.setPosition(glm::vec3((float)scene.size(), 0.0f, 0.0f));
            newCube.setRotation(glm::vec3((float)scene.size() * 12.0f, 0.0f, 0.0f));
            newCube.setScale(glm::vec3(1.0f, 1.0f, 1.0f + (float)scene.size() * 0.5f));
            scene.push_back(newCube);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}