//
// Created by super on 20.06.2024.
//

#include "ImguiContext.h"


#include "main.h"

//IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//Games
#include "Snake.h"

//System
#include "typeinfo"


void ImguiContext::init() {
    GameList.push_back(new Snake());

    //IMGUI INIT
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

// Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(getwindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void ImguiContext::loop() {
    //glfwWaitEvents();
    glfwPollEvents();

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("q", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("Opengl"))
    {
        if (ImGui::MenuItem("Get Error", "CTRL+H")) {
            auto e = glGetError();
            std::string errorDisc = "Unknown error";
            switch(e){
                case GL_NO_ERROR:
                    errorDisc = "No errors";
                    break;
                case GL_INVALID_ENUM:
                    errorDisc = "Invalid enum";
                    break;
                case GL_INVALID_VALUE:
                    errorDisc = "Invalid value";
                    break;
                case GL_INVALID_OPERATION:
                    errorDisc = "Invalid operation";
                    break;
                case GL_STACK_OVERFLOW:
                    errorDisc = "Stack overflow";
                    break;
                case GL_STACK_UNDERFLOW:
                    errorDisc = "Stack underflow";
                    break;
                case GL_OUT_OF_MEMORY:
                    errorDisc = "Out of memory";
                    break;
                default:
                    errorDisc = "Unknown error";
                    break;
            }
            Info((errorDisc + ". Code of error: " + std::to_string(e)).c_str());
        }
        /*if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();*/
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();


    for (auto g:GameList) {
        if (ImGui::Button(typeid(g).name(), ImVec2(250, 250)))
        {
            setContext(g);
        }
    }

    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    // Swap the screen buffers
    glfwSwapBuffers(getwindow());
}

ImguiContext::~ImguiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
