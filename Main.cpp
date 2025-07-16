#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include"Mesh.h"
#include"GLTFViewer.h"
#include<tiny_gltf/tiny_gltf.h>

#include"Cube.hpp"
#include"Plane.hpp"
#include"Sphere.hpp"
#include"Wavefront.hpp"

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
        Texture("white.png", "diffuse", 0, GL_UNSIGNED_BYTE)
    };

    Cube cube("Default Cube", defaultTex);
    scene.push_back(cube);

    /*Wavefront wavefront("Heart.obj", defaultTex);
    scene.push_back(wavefront);*/

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    bool playMode = false;
    int hierarchySelectedInd;
    char textureFilename[64] = "";

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
            ImGui::Begin("Scene Editor");
            if (ImGui::Button("Play"))
            {
                playMode = true;
            }

            ImGui::ListBox("Elements", &hierarchySelectedInd, objectNameGetter, scene.data(), scene.size());

            if (ImGui::Button("Cube"))
            {
                std::string buf("Cube ");
                buf.append(std::to_string(scene.size()).c_str());
                Cube newCube(buf, defaultTex);
                scene.push_back(newCube);
            }

            if (ImGui::Button("Sphere"))
            {
                std::string buf("Sphere ");
                buf.append(std::to_string(scene.size()).c_str());
                Sphere newSphere(buf, defaultTex);
                scene.push_back(newSphere);
            }

            if (ImGui::Button("Plane"))
            {
                std::string buf("Plane ");
                buf.append(std::to_string(scene.size()).c_str());
                Plane newPlane(buf, defaultTex);
                scene.push_back(newPlane);
            }

            if (ImGui::Button("Import OBJ"))
            {
                //std::string buf("Plane ");
                //buf.append(std::to_string(scene.size()).c_str());
                Wavefront wavefront("Heart.obj", defaultTex);
                scene.push_back(wavefront);
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

                ImGui::Text("Current Material");
                ImGui::Text(scene[hierarchySelectedInd].m_textureFile.c_str());
                ImGui::InputText("Material Input", textureFilename, 64);
                if (ImGui::Button("Set Material"))
                {
                    scene[hierarchySelectedInd].setTextures(textureFilename);
                }

                if (ImGui::Button("Delete"))
                {
                    scene.erase(scene.begin() + hierarchySelectedInd);
                    hierarchySelectedInd = -1;
                }

                ImGui::End();
            }
        }
        else {
            ImGui::Begin("In Play");
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