//
// Created by super on 20.06.2024.
//

#include "ImguiContext.h"


#include "main.h"

//IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

//Games
#include "Snake.h"
//#include "Pacman.h"

//System
#include "typeinfo"
#include "SystemAdapter.h"

//JSON
#include "nlohmann/json.hpp"

void ImguiContext::init() {
    SetIcon("standard_icon.png");
#define AddGame(classname) GameList.push_back(new classname());
    AddGame(Snake)
   // AddGame(Pacman)
    auto e = cpr::GetCallback([this](const cpr::Response& r){ImguiContext::GetUpdateInfo(r);}, cpr::Url{"https://maserplay.ru/arcadegamesglfw_version.json"});

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

void ImguiContext::GetUpdateInfo(const cpr::Response& r) {
    if (r.error)
    {
        SPDLOG_WARN("cannot check for updates: {} {}", (int) r.error.code, r.error.message);
        return;
    }
    nlohmann::json Update = nlohmann::json::parse(r.text);
    try {
        if (std::stoi((std::string) Update["latestVersionCode"]) > APPVERSIONCODE) {
            UpdateUrl = Update["url"];
        }
    } catch (const std::invalid_argument& ia) {
        SPDLOG_ERROR("Update[\"latestVersionCode\"] std::invalid_argument");
    } catch (const std::out_of_range& oor) {
        SPDLOG_ERROR("Update[\"latestVersionCode\"] std::out_of_range");
    }
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
                ErrorBox(("No. Code of error: " + std::to_string(e)).c_str())
            } else {
                ErrorBox((std::string(*errorDisc) + ". Code of error: " + std::to_string(e)).c_str())
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
            ErrorBox((errorDisc + ". Code of error: " + std::to_string(e)).c_str());
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
            ErrorBox((errorDisc + ". Code of error: " + std::to_string(e)).c_str());
        }
        if (ImGui::MenuItem("ImGui::ShowDebugLogWindow();")) {
            ImguiDebugLog = true;
        }
        if (ImGui::MenuItem("ImGui::ShowDemoWindow();")) {
            ImguiDemo = true;
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
        if (ImGui::MenuItem("There is error!")) {
            SendError = true;
        }
        if (ImGui::MenuItem("Quit")) {
            glfwSetWindowShouldClose(getwindow(), GLFW_TRUE);
        }
        ImGui::EndMenu();
    }
    if (!UpdateUrl.empty()) {
        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0, 0.8f, 0.8f));
        if (ImGui::Button("Update Available!"))
        {
            SystemAdapter::OpenLink(UpdateUrl);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }

    ImGui::EndMenuBar();
#ifdef _DEBUG
    if (ImguiDebugLog) {
        ImGui::ShowDebugLogWindow(&ImguiDebugLog);
    }
    if (ImguiDemo)
    {
        ImGui::ShowDemoWindow(&ImguiDemo);
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
    if (SendError) {
        ImGui::SetNextWindowSize(ImVec2(700,220));
        ImGui::Begin("Send error", &SendError);
            ImGui::LabelText(" ", "Input Text In Space bellow, and print your email");
            ImGui::InputText(" ", &ErrorText);
            if (ImGui::Button("Send")) {
                nlohmann::json error;
                error["AppName"] = APPNAME;
                error["AppVersion"] = std::to_string(APPVERSION);
                error["Message"] = ErrorText;
                cpr::PostAsync(cpr::Url{"https://maserplay.ru/api/crash"},
                          cpr::Body{to_string(error)},
                          cpr::Header{{"Content-Type", "application/json"}});
                SendError = false;
                Info("Thanks for error report!")
            }
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
