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

void saveState(std::vector <std::vector <Object>>& sceneHistory, std::vector <Object>& scene, int& historyInd)
{
    const int MAX_STATES = 6;

    // if at max saved states, delete oldest state
    if (historyInd + 1 >= MAX_STATES)
    {
        sceneHistory.erase(sceneHistory.begin());
        sceneHistory.push_back(scene);
        return;
    }

    // if editing from an undo, erase forward history
    while (sceneHistory.size() > historyInd + 1)
    {
        sceneHistory.erase(sceneHistory.begin() + historyInd + 1);
    }

    sceneHistory.push_back(scene);
    historyInd = historyInd + 1;
}

int main()
{
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

    std::vector <std::vector <Object>> sceneHistory;
    int historyInd = -1;

    std::vector <Object> scene;
    std::vector <Texture> defaultTex{
        Texture("white.png", "diffuse", 0, GL_UNSIGNED_BYTE)
    };

    /*Cube cube("Default Cube", defaultTex);
    scene.push_back(cube);
    saveState(sceneHistory, scene, historyInd);*/

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    camera.Position = glm::vec3(4, 2, 4);
    camera.Orientation = glm::vec3(-0.66, -0.33, -0.66);

    int hierarchySelectedInd;
    char textureFilename[64] = "";
    char objFilename[128] = "";
    char gltfFilename[128] = "";
    float positionInput[3] = { 0, 0, 0 };
    float rotationInput[3] = { 0, 0, 0 };
    float scaleInput[3] = { 1, 1, 1 };

    //bool gltfViewerMode = true;
    //bool playMode = true;
    bool gltfViewerMode = false;
    bool playMode = false;

    GLTFViewer gltfViewer(camera);
    gltfViewer.init("models/grindstone/scene.gltf");
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (playMode) {
            camera.inputs(window);
        }
        if (gltfViewerMode) {
            //glUseProgram(gltfViewer.ID);
            gltfViewer.drawScene(camera);
        }
        
        else {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

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

                if (ImGui::CollapsingHeader("State Controls")) {
                    if (historyInd > 0) {
                        if (ImGui::Button("Undo"))
                        {
                            historyInd = historyInd - 1;
                            scene = sceneHistory[historyInd];
                        }
                        if (historyInd < sceneHistory.size() - 1)
                        {
                            ImGui::SameLine();
                        }
                    }

                    if (historyInd < sceneHistory.size() - 1)
                    {
                        if (ImGui::Button("Redo"))
                        {
                            historyInd = historyInd + 1;
                            scene = sceneHistory[historyInd];
                        }
                    }
                }

                if (ImGui::CollapsingHeader("Add Objects")) {
                    ImGui::Text("Add Primitive ");
                    ImGui::SameLine();
                    if (ImGui::Button("Cube"))
                    {
                        std::string buf("Cube ");
                        buf.append(std::to_string(scene.size()).c_str());
                        Cube newCube(buf, defaultTex);
                        scene.push_back(newCube);
                        saveState(sceneHistory, scene, historyInd);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Sphere"))
                    {
                        std::string buf("Sphere ");
                        buf.append(std::to_string(scene.size()).c_str());
                        Sphere newSphere(buf, defaultTex);
                        scene.push_back(newSphere);
                        saveState(sceneHistory, scene, historyInd);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Plane"))
                    {
                        std::string buf("Plane ");
                        buf.append(std::to_string(scene.size()).c_str());
                        Plane newPlane(buf, defaultTex);
                        scene.push_back(newPlane);
                        saveState(sceneHistory, scene, historyInd);
                    }

                    ImGui::Separator();

                    ImGui::InputText("OBJ Filename", objFilename, 128);
                    if (ImGui::Button("Import OBJ"))
                    {
                        Wavefront wavefront(objFilename, defaultTex);
                        scene.push_back(wavefront);
                        saveState(sceneHistory, scene, historyInd);
                    }
                    //ImGui::InputText("GLTF Filename", gltfFilename, 128);
                    if (ImGui::Button("Import GLTF"))
                    {
                        gltfViewer.init("models/grindstone/scene.gltf");
                        //gltfViewer.init("models/main_sponza/NewSponza_Main_glTF_003.gltf");
                        gltfViewerMode = true;
                        playMode = true;
                    }
                }


                ImGui::Separator();

                ImGui::ListBox("Elements", &hierarchySelectedInd, objectNameGetter, scene.data(), scene.size());

                ImGui::End();

                if (hierarchySelectedInd >= 0) {
                    ImGui::Begin(scene[hierarchySelectedInd].m_name.c_str());

                    if (scene[hierarchySelectedInd].m_enabled) {
                        ImGui::Text("Enabled");
                        ImGui::SameLine();
                        if (ImGui::Button("Disable"))
                        {
                            scene[hierarchySelectedInd].m_enabled = false;
                            saveState(sceneHistory, scene, historyInd);
                        }
                    }
                    else {
                        ImGui::Text("Disabled");
                        ImGui::SameLine();
                        if (ImGui::Button("Enable"))
                        {
                            scene[hierarchySelectedInd].m_enabled = true;
                            saveState(sceneHistory, scene, historyInd);
                        }
                    }
                    ImGui::Separator();

                    std::string currentPos = "Position: ( " + std::to_string(scene[hierarchySelectedInd].m_position.x) + ", "
                        + std::to_string(scene[hierarchySelectedInd].m_position.y) + ", "
                        + std::to_string(scene[hierarchySelectedInd].m_position.z) + " )";
                    if (ImGui::CollapsingHeader(currentPos.c_str()))
                    {
                        ImGui::InputFloat("X##Position", &positionInput[0], 0.0f, 0.0f, "%0.1f");
                        ImGui::InputFloat("Y##Position", &positionInput[1], 0.0f, 0.0f, "%0.1f");
                        ImGui::InputFloat("Z##Position", &positionInput[2], 0.0f, 0.0f, "%0.1f");
                        if (ImGui::Button("Edit Position"))
                        {
                            scene[hierarchySelectedInd].setPosition(glm::vec3(positionInput[0], positionInput[1], positionInput[2]));
                            saveState(sceneHistory, scene, historyInd);
                        }
                    }
                    ImGui::Separator();
                    std::string currentRot = "Rotation: ( " + std::to_string(scene[hierarchySelectedInd].m_rotation.x) + ", "
                        + std::to_string(scene[hierarchySelectedInd].m_rotation.y) + ", "
                        + std::to_string(scene[hierarchySelectedInd].m_rotation.z) + " )";
                    if (ImGui::CollapsingHeader(currentRot.c_str()))
                    {

                        ImGui::InputFloat("X##Rotation", &rotationInput[0], 0.0f, 0.0f, "%0.1f");
                        ImGui::InputFloat("Y##Rotation", &rotationInput[1], 0.0f, 0.0f, "%0.1f");
                        ImGui::InputFloat("Z##Rotation", &rotationInput[2], 0.0f, 0.0f, "%0.1f");
                        if (ImGui::Button("Edit Rotation"))
                        {
                            scene[hierarchySelectedInd].setRotation(glm::vec3(rotationInput[0], rotationInput[1], rotationInput[2]));
                            saveState(sceneHistory, scene, historyInd);
                        }
                    }
                    ImGui::Separator();
                    std::string currentScale = "Scale: ( " + std::to_string(scene[hierarchySelectedInd].m_scale.x) + ", "
                        + std::to_string(scene[hierarchySelectedInd].m_scale.y) + ", "
                        + std::to_string(scene[hierarchySelectedInd].m_scale.z) + " )";
                    if (ImGui::CollapsingHeader(currentScale.c_str()))
                    {
                        ImGui::InputFloat("X##Scale", &scaleInput[0], 0.0f, 0.0f, "%0.1f");
                        ImGui::InputFloat("Y##Scale", &scaleInput[1], 0.0f, 0.0f, "%0.1f");
                        ImGui::InputFloat("Z##Scale", &scaleInput[2], 0.0f, 0.0f, "%0.1f");
                        if (ImGui::Button("Edit Scale"))
                        {
                            scene[hierarchySelectedInd].setScale(glm::vec3(scaleInput[0], scaleInput[1], scaleInput[2]));
                            saveState(sceneHistory, scene, historyInd);
                        }
                    }
                    ImGui::Separator();
                    ImGui::Text("Current Material: ");
                    ImGui::SameLine();
                    ImGui::Text(scene[hierarchySelectedInd].m_textureFile.c_str());
                    ImGui::InputText("Material Input", textureFilename, 64);
                    if (ImGui::Button("Set Material"))
                    {
                        scene[hierarchySelectedInd].setTextures(textureFilename);
                        saveState(sceneHistory, scene, historyInd);
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Delete"))
                    {
                        scene.erase(scene.begin() + hierarchySelectedInd);
                        hierarchySelectedInd = -1;
                        saveState(sceneHistory, scene, historyInd);
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
        }

        /*gltfViewerMode = true;
        playMode = true;*/

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
    
	return 0;
}