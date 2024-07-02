// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// OPENAL
#include <alc.h>
#include <stb_image.h>

#include "debug.h"
#include "main.h"
//CONTEXT
#include "ImguiContext.h"
//MEMORY
#include "memory"
//OTHER
#include "SystemAdapter.h"
//LOG
#ifdef _DEBUG
#include "spdlog/sinks/stdout_color_sinks.h"
#else
#include "spdlog/sinks/basic_file_sink.h"
#include <windows.h>
#include <stb_image.h>

#endif
#include "spdlog/spdlog.h"
#include "ZipArchive.h"

void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int code, const char *description);

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
Context* currentContext = NULL;
Context* tempContext = NULL;
void setContext(Context* context)
{
    tempContext = currentContext;
    context->init();
    context->size_callback(WIDTH, HEIGHT);
    currentContext = context;
}

void SetIcon(const std::string& name)
{
    spdlog::debug("loading Assets.zip && " + name);
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
            spdlog::info(name + " loaded && applied");
        } else {
            spdlog::warn(name + " not found");
        }
    } else {
        spdlog::warn("Assets.zip not found");
    }
}
void loadMainMenu()
{
    setContext(new ImguiContext());
}

// The MAIN function, from here we start the application and run the game loop
#ifdef DO_WINMAIN
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#else
int main()
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
        spdlog::set_level(spdlog::level::debug); // Set global log level to debug
        spdlog::set_pattern("[%^%l%$] %v");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
#else
    auto logger = spdlog::basic_logger_mt("logger", "logs/log.txt");
    spdlog::set_level(spdlog::level::info); // Set global log level to debug3
    spdlog::set_default_logger(logger);
#endif
    spdlog::debug("Starting...");
    // Init GLFW
    glfwInit();
    //Init
    SystemAdapter::Init();
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

    loadMainMenu();

    //CHECK CONTEXT
    if (currentContext == NULL)
    {
        ErrorThrow("currentContext == NULL")
    }

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        currentContext->loop();
        if (tempContext != NULL) {
            delete tempContext;
            tempContext = NULL;
        }
    }
    spdlog::debug("Closing...");

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
void error_callback(int code, const char *description)
{
    spdlog::error("GLFW error was occuruped" + std::string(description) + " with code" + std::to_string(code));
}