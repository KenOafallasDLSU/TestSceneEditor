#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include"Mesh.h"
#include"GLTFViewer.h"
#include<tiny_gltf/tiny_gltf.h>

#include"Cube.hpp"
#include"Plane.hpp"
#include"Sphere.hpp"

const char* objectNameGetter(void* data, int ind)
{
    Object* objects = (Object*)data;
    Object& currentObject = objects[ind];
    return currentObject.m_name.c_str();
}

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

    Sphere sphere("Sphere", defaultTex);
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
            ImGui::Begin("AndGin");
            if (ImGui::Button("Play"))
            {
                playMode = true;
            }

            ImGui::ListBox("Scene", &hierarchySelectedInd, objectNameGetter, scene.data(), scene.size());
            if (hierarchySelectedInd >= 0) {
                ImGui::Text(scene[hierarchySelectedInd].m_name.c_str());
            }

            if (ImGui::Button("Cube"))
            {
                std::string buf("Cube ");
                buf.append(std::to_string(scene.size()).c_str());
                Cube newCube(buf, defaultTex);
                scene.push_back(newCube);
                scene[scene.size() - 1].setPosition(glm::vec3(3 * scene.size(), 0, 0));
            }

            if (ImGui::Button("Sphere"))
            {
                std::string buf("Sphere ");
                buf.append(std::to_string(scene.size()).c_str());
                Sphere newSphere(buf, defaultTex);
                scene.push_back(newSphere);
                scene[scene.size() - 1].setPosition(glm::vec3(3 * scene.size(), 0, 0));
            }

            if (ImGui::Button("Plane"))
            {
                std::string buf("Plane ");
                buf.append(std::to_string(scene.size()).c_str());
                Plane newPlane(buf, defaultTex);
                scene.push_back(newPlane);
                scene[scene.size() - 1].setPosition(glm::vec3(0, 0, -5));
            }

            ImGui::End();

            if (hierarchySelectedInd >= 0) {
                ImGui::Begin(scene[hierarchySelectedInd].m_name.c_str());

                ImGui::Checkbox("Enabled", &scene[hierarchySelectedInd].m_enabled);

                ImGui::Text("Position");
                ImGui::InputFloat("X##Position", &scene[hierarchySelectedInd].m_position.x, 0.0f, 0.0f, "%0.1f");
                ImGui::InputFloat("Y##Position", &scene[hierarchySelectedInd].m_position.y, 0.0f, 0.0f, "%0.1f");
                ImGui::InputFloat("Z##Position", &scene[hierarchySelectedInd].m_position.z, 0.0f, 0.0f, "%0.1f");

                ImGui::Text("Rotation");
                ImGui::InputFloat("X##Rotation", &scene[hierarchySelectedInd].m_rotation.x, 0.0f, 0.0f, "%0.1f");
                ImGui::InputFloat("Y##Rotation", &scene[hierarchySelectedInd].m_rotation.y, 0.0f, 0.0f, "%0.1f");
                ImGui::InputFloat("Z##Rotation", &scene[hierarchySelectedInd].m_rotation.z, 0.0f, 0.0f, "%0.1f");

                ImGui::Text("Scale");
                ImGui::InputFloat("X##Scale", &scene[hierarchySelectedInd].m_scale.x, 0.0f, 0.0f, "%0.1f");
                ImGui::InputFloat("Y##Scale", &scene[hierarchySelectedInd].m_scale.y, 0.0f, 0.0f, "%0.1f");
                ImGui::InputFloat("Z##Scale", &scene[hierarchySelectedInd].m_scale.z, 0.0f, 0.0f, "%0.1f");

                if (ImGui::Button("Delete"))
                {
                    scene.erase(scene.begin() + hierarchySelectedInd);
                    hierarchySelectedInd = -1;
                }

                ImGui::End();
            }
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