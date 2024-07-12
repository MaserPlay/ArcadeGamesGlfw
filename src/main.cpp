// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// OPENAL
#include <alc.h>

//LOG
#include "LOG.h"
#ifdef _DEBUG
#include "spdlog/sinks/stdout_color_sinks.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
#include <stb_image.h>
#endif
//CONTEXT
#include "ImguiContext.h"
//MEMORY
#include "memory"
//OTHER
#include "SystemAdapter.h"
//ARCHIVE
#include "ZipArchive.h"
//OTHER
#include <stb_image.h>
#include "debug.h"
#include "main.h"
#include "IntroContext.h"
#include "Localization.h"

#ifdef _WINDOWS
#include <windows.h>
#endif
bool isContextInit = false;
Font::Font* baseFont = nullptr;
Font::Font* getFont(){return baseFont;}
#ifndef DO_WINMAIN
    bool skipintro = false;
    bool genlangfile = false;
#endif

void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int code, const char *description);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;
GLuint getWidth()
{
    return WIDTH;
}
GLuint getHeight()
{
    return HEIGHT;
}
GLFWwindow* window;
ALCdevice *Aldevice;
ALCcontext *AlContext;
GLFWwindow* getwindow()
{
    return window;
}
Context* currentContext;
Context* tempContext;
void setContext(Context* context)
{
    tempContext = currentContext;
    isContextInit = false;
    currentContext = context;
}

void SetIcon(const std::string& name)
{
    SPDLOG_DEBUG("loading Assets.zip && " + name);
    if (std::filesystem::is_regular_file(SystemAdapter::GetGameFolderName("") + "Assets.zip")) {
        ZipArchive zip{SystemAdapter::GetGameFolderName("") + "Assets.zip"};
        char *content = NULL; zip_uint64_t size = 0;
        zip.get(name, content, size);
        if (content != NULL) {
            unsigned char *image = NULL; int width, height, nrChannels;
            image = stbi_load_from_memory(reinterpret_cast<const unsigned char *const>(content), size, &width, &height,
                                          &nrChannels, 0);
            GLFWimage e{ width, height, image};
            glfwSetWindowIcon(window, 1, &e);
            stbi_image_free(content);
            SPDLOG_INFO(name + " loaded && applied");
        } else {
            SPDLOG_WARN(name + " not found");
        }
    } else {
        SPDLOG_WARN("Assets.zip not found");
    }
}
void loadMainMenu()
{
    setContext(new ImguiContext());
}
void InitAsync()
{
    //DEFALT FONT
    SPDLOG_DEBUG("loading Assets.zip && font.ttf");
    if (std::filesystem::is_regular_file(SystemAdapter::GetGameFolderName("") + "Assets.zip")) {
        ZipArchive zip{SystemAdapter::GetGameFolderName("") + "Assets.zip"};
        char *content = NULL; zip_uint64_t size = 0;
        zip.get("font.ttf", content, size);
        if (content != NULL) {
            baseFont = new Font::Font(reinterpret_cast<const FT_Byte *>(content), size);
            SPDLOG_INFO("font.ttf loaded && applied");
        } else {
            SPDLOG_WARN("font.ttf not found");
        }
    } else {
        SPDLOG_WARN("Assets.zip not found");
    }
}

// The MAIN function, from here we start the application and run the game loop
#ifdef DO_WINMAIN
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#else
int main(int argc, char** argv)
#endif
{
    //Create Log
#ifdef _DEBUG
// NO abort() message in windows
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    try
    {
        // create a color multi-threaded logger
        auto console = spdlog::stdout_color_mt("console");
        auto err_logger = spdlog::stderr_color_mt("stderr");
        spdlog::set_level(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
#else
    auto logger = spdlog::basic_logger_mt("logger", "logs/latest.log");
//    spdlog::set_level(spdlog::level::info); // Set global log level to debug3
    spdlog::set_default_logger(logger);
#endif
    spdlog::set_pattern("%s:%# [%^%l%$] %v");
    SPDLOG_DEBUG("Starting...");
#ifndef DO_WINMAIN
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "skipintro")
        {
            skipintro = true;
        } else if (std::string(argv[i]) == "genlang") {
            genlangfile = true;
        }
    }
#endif
    // Init GLFW
    glfwInit();
    //Init
    SystemAdapter::Init();
    Localization::init();
#ifdef _DEBUG
    if (genlangfile) {
        Localization::writefile();
    }
#endif
    //INIT FONT LIB
    Font::InitLib();
    // Init OpenAl
    Aldevice = alcOpenDevice(nullptr);
    AlContext = alcCreateContext(Aldevice, nullptr);
    alcMakeContextCurrent(AlContext);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, APPNAME, nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        ErrorAbort("Failed to create GLFW window")
        return -1;
    }

    glfwMakeContextCurrent(window);
    //ERROR
    glfwSetErrorCallback(error_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        ErrorAbort("Failed to initialize GLEW")
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetWindowSizeLimits(window, 400,400, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    InitAsync();
#if defined(_DEBUG) && !defined(DO_WINMAIN)
    if (skipintro) {
//        std::thread t{InitAsync};
//        if (t.joinable()) {
//            t.join();
//        }
        SPDLOG_DEBUG("intro_skipped");
        loadMainMenu();
    } else {
        setContext(new IntroContext());
    }
#else
    setContext(new IntroContext());
#endif
    //INIT IT
    isContextInit = true;
    currentContext->init();
    currentContext->size_callback(WIDTH, HEIGHT);

    //CHECK CONTEXT
    if (currentContext == NULL)
    {
        ErrorThrow("currentContext == NULL")
    }

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        if (isContextInit) {
            currentContext->loop();
        } else {
            if (tempContext != NULL) {
                delete tempContext;
                tempContext = NULL;
                if (!isContextInit) {
                    isContextInit = true;
                    currentContext->init();
                    currentContext->size_callback(WIDTH, HEIGHT);
                }
            }
        }
    }
    SPDLOG_DEBUG("Closing...");
#ifdef _DEBUG
    if (genlangfile) {
        Localization::genfile();
    }
#endif

    delete currentContext;
    SystemAdapter::Destroy();
    glfwTerminate();
    //OPENAL
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(AlContext);
    alcCloseDevice(Aldevice);

    return 0;
}
void window_size_callback(GLFWwindow* window, int width, int height){
    WIDTH = width; HEIGHT = height;
    currentContext->size_callback(width, height);
    currentContext->loop();
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    currentContext->key_callback(key, scancode, action, mods);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    currentContext->mouse_button_callback(button, action, mods);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    currentContext->cursor_position_callback(xpos, ypos);
}
void error_callback(int code, const char *description)
{
    SPDLOG_ERROR("GLFW error was occuruped" + std::string(description) + " with code" + std::to_string(code));
}
bool fullscreen = false;
void SwitchFullscreen(){
    if (fullscreen)
    {
        glfwSetWindowMonitor(window, NULL, 100, 100, 800, 600, NULL);
        fullscreen = false;
    } else {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        fullscreen = true;
    }
}