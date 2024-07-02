//
// Created by super on 20.06.2024.
//

#include "ImguiContext.h"


#include "main.h"

//OPENAL
#include <al.h>

//IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//Games
#include "Snake.h"
//#include "Pacman.h"

//System
#include "typeinfo"


void ImguiContext::init() {
#define AddGame(classname) GameList.push_back(new classname());
    AddGame(Snake)
   // AddGame(Pacman)

    //IMGUI INIT
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

// Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(getwindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void ImguiContext::loop() {
    glfwWaitEvents();

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("q", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
    ImGui::BeginMenuBar();
#ifdef _DEBUG
    if (ImGui::BeginMenu("Debugging"))
    {
        if (ImGui::MenuItem("glfwGetError();")) {
            const char **errorDisc = NULL;
            auto e = glfwGetError(errorDisc);
            if (errorDisc == NULL)
            {
                Error(("No. Code of error: " + std::to_string(e)).c_str())
            } else {
                Error((std::string(*errorDisc) + ". Code of error: " + std::to_string(e)).c_str())
            }
        }
        if (ImGui::MenuItem("glGetError();")) {
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
            Error((errorDisc + ". Code of error: " + std::to_string(e)).c_str());
        }
        if (ImGui::MenuItem("alGetError();")) {
            auto e = alGetError();
            std::string errorDisc = "Unknown error";
            switch(e){
                case AL_INVALID_NAME:
                    errorDisc = "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                    break;
                case AL_INVALID_ENUM:
                    errorDisc = "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                    break;
                case AL_INVALID_VALUE:
                    errorDisc = "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                    break;
                case AL_INVALID_OPERATION:
                    errorDisc = "AL_INVALID_OPERATION: the requested operation is not valid";
                    break;
                case AL_OUT_OF_MEMORY:
                    errorDisc = "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                    break;
                case AL_NO_ERROR:
                    errorDisc = "AL_NO_ERROR";
                    break;
                default:
                    errorDisc = "Unknown error";
                    break;
            }
            Error((errorDisc + ". Code of error: " + std::to_string(e)).c_str());
        }
        if (ImGui::MenuItem("ImGui::ShowDebugLogWindow();")) {
            ImguiDebugLog = true;
        }
        ImGui::EndMenu();
    }
#endif
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem(("About " + std::string(APPNAME)).c_str())) {
            Info((std::string(APPNAME) + " v." + std::to_string(APPVERSION) + " by " + std::string(APPAUTHOR)).c_str())
        }
        if (ImGui::MenuItem("About ImGui")) {
            ImguiAbout = true;
        }
        if (ImGui::MenuItem("User guide")) {
            ImguiUGuide = true;
        }
        if (ImGui::MenuItem("Quit")) {
            glfwSetWindowShouldClose(getwindow(), GLFW_TRUE);
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
#ifdef _DEBUG
    if (ImguiDebugLog) {
        ImGui::ShowDebugLogWindow(&ImguiDebugLog);
    }
#endif
    if (ImguiAbout) {
        ImGui::ShowAboutWindow(&ImguiAbout);
    }
    if (ImguiUGuide) {
        ImGui::Begin("UserGuide", &ImguiUGuide);
            ImGui::ShowUserGuide();
        ImGui::End();
    }


    for (auto g:GameList) {
        if (ImGui::Button(typeid(*g).name(), ImVec2(250, 250)))
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
