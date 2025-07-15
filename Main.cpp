#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include"Mesh.h"
#include"SceneEditor.h"
#include<tiny_gltf/tiny_gltf.h>

#include"Cube.hpp"
#include"Plane.hpp"
#include"Sphere.hpp"

int main()
{
	// Vertices coordinates

    //SceneEditor sceneEditor("models/grindstone/scene.gltf");
 /*   SceneEditor sceneEditor("models/main_sponza/NewSponza_Main_glTF_003.gltf");
    sceneEditor.run();*/

    tinygltf::Model model;

	const unsigned int width = 1280;
	const unsigned int height = 720;

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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 430");

    std::vector <Object> scene;
    std::vector <Texture> defaultTex{
        Texture("Mocap512px.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE)
    };

    Sphere sphere(defaultTex);
    scene.push_back(sphere);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    std::vector <Texture> brickTex{
        Texture("brick.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
    };

    bool playMode = false;
    int hierarchySelectedInd;

	while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (playMode) {
            camera.inputs(window);
        }

		camera.updateMatrix(45.0f, 0.1f, 100.0f);

        for (Object object : scene)
        {
            object.draw(camera);
        }

        if (!playMode) {
            ImGui::Begin("Window");
            ImGui::Text("Kenneth");

            if (ImGui::Button("Play"))
            {
                playMode = true;
            }

            if (ImGui::Button("Cube"))
            {
                Cube newCube(defaultTex);
                scene.push_back(newCube);
                scene[scene.size() - 1].setPosition(glm::vec3(3 * scene.size(), 0, 0));
            }

            if (ImGui::Button("Sphere"))
            {
                Sphere newSphere(defaultTex);
                scene.push_back(newSphere);
                scene[scene.size() - 1].setPosition(glm::vec3(3 * scene.size(), 0, 0));
            }

            if (ImGui::Button("Plane"))
            {
                Plane newPlane(defaultTex);
                scene.push_back(newPlane);
                scene[scene.size() - 1].setPosition(glm::vec3(0, 0, -5));
            }

            ImGui::Text(std::to_string(scene[scene.size() - 1].m_position.x).c_str());
            ImGui::Text(std::to_string(scene[scene.size() - 1].m_position.y).c_str());
            ImGui::Text(std::to_string(scene[scene.size() - 1].m_position.z).c_str());

            float position[3];
            ImGui::InputFloat3("Position", position);
            if (ImGui::Button("Submit Position"))
            {
                scene[scene.size() - 1].setPosition(glm::vec3(position[0], position[1], position[2]));
            }

            float rotation[3];
            ImGui::InputFloat3("Rotation", rotation);
            if (ImGui::Button("Submit Rotation"))
            {
                scene[scene.size() - 1].setRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
            }

            float scale[3];
            ImGui::InputFloat3("Scale", scale);
            if (ImGui::Button("Submit Scale"))
            {
                scene[scene.size() - 1].setScale(glm::vec3(scale[0], scale[1], scale[2]));
            }

            ImGui::End();
        }
        else {
            ImGui::Begin("Window");
            ImGui::Text("Kenneth");
            if (ImGui::Button("Stop Play"))
            {
                playMode = false;
            }
            ImGui::End();
        }
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}