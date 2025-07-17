# GameLab Test Submission

By Kenneth Neil B. Oafallas

This is a barebones scene editor written in C++ using OpenGL for rendering and ImGui for UI.

The project is setup as a Visual Studio 2017 Solution

# Starting the project:
1. Clone from https://github.com/KenOafallasDLSU/TestSceneEditor.git
2. Launch in Visual Studio through the `.sln` file in the project root. Use VS2017 compiler/settings if prompted
3. Run with the VS player. The project settings are included in the repo, so VS should be able to automatically find and run the main function. Otherwise, the main function is `int main()` in the `Main.cpp` file in project root.

## Troubleshooting

Sometimes, VS will not automatically find some dependencies. But all the dependencies are included in the project. If that happens, 

1. Select `Show All Files` option in VS Solution Explorer window.
2. Right click on dependecies with a red stop sign
3. Select `Include in Project`
   
A common culprit is `Libraries/lib/glfw3.lib` 

# Feature Notes:
## Material Import
The project supports importing images with RED, RGB or RGBA channels as materials. Materials on OBJ-imported objects and primitives are replaceable. The following images are included in the project root for reference: “white.png", “brick.png", “mocap.png”.

## Wavefront OBJ File Import
The project supports single-object static mesh import. The faces represented in triangles or quads are supported. Associated materials are not imported, only the mesh. The file “Heart.obj” is included in the project root for reference.

## GLTF File Import
The project has a bugged GLTF import. Multi-object scenes including materials can be imported, but some objects are rendered in incorrect transform or rotation.

## State Undo/Redo
A total of 6 states are saved, allowing undo/redo 5 times anywhere along the 6 newest states. When making changes while in an undone state, forward states are deleted and replaced with a new state branch.

## Error Handling
The project does not handle errors, and will crash when invalid input (such as letters for transform values, or non-existent OBJ files) is provided
