//
// Created by tayler on 12/14/25.
//

#include "../include/application.h"

#include <iostream>
#include <unistd.h>


#include "glad/glad.h"


#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "../include/io/input.h"


Application::Application(int width, int height, const char *title) : window(nullptr), width(width), height(height),
                                                                     title(title),
                                                                     inputManager(InputManager::GetInstance()),
                                                                     input(&inputManager),
                                                                     camera(new Viewport(1920, 1080, 1.0f)) {
}

Application::~Application() {
}

void Application::Initialise() {
    if (!glfwInit()) {
        // Initialise GLFW
        std::cerr << "Failed to Initialise GLFW!" << std::endl;
        return;
    } else
        std::cout << "GLFW Initialised" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    } else
        std::cout << "Window Created" << std::endl;


    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *win, int w, int h) {
                                       glViewport(0, 0, w, h);

                                       auto &app = Application::GetInstance();
                                       app.getCamera()->UpdateViewport((float) w, (float) h);
                                   });

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    inputManager.SetWindow(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        // Initialise GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    } else
        std::cout << "GLAD Initialised" << std::endl;

    shader = new Shader((GetExecutablePath() + "/../res/shaders/shader.vert").c_str(),
                        (GetExecutablePath() + "/../res/shaders/shader.frag").c_str());

    glEnable(GL_MULTISAMPLE);

    input.BindMouseButton(GLFW_MOUSE_BUTTON_1, MOUSE_PRESSED, [this]() {
        if (ImGui::GetIO().WantCaptureMouse)
            return;

        if (isPanning) {
            lastMouse = {
                inputManager.getMouseX(),
                inputManager.getMouseY()
            };
        }
    });

    input.BindMouseButton(GLFW_MOUSE_BUTTON_1, MOUSE_UP, [this]() {
        if (playState != PlayState_STOP) return;

        glm::vec2 worldPos = ScreenToWorld(
            inputManager.getMouseX(),
            inputManager.getMouseY()
        );

        simulationSystem.CreateParticle(
        targetParticleType,
        worldPos
        );
    });

    input.BindKey(GLFW_KEY_LEFT_SHIFT, KEY_DOWN, [this]() {
        if (!isPanning) {
                lastMouse = {
                    inputManager.getMouseX(),
                    inputManager.getMouseY()
                };
            }
            isPanning = true;
    });

    input.BindKey(GLFW_KEY_LEFT_SHIFT, KEY_UP, [this]() {
        isPanning = false;
    });

    input.BindKey(GLFW_KEY_TAB, KEY_UP, [this]() {
       captureMouse = !captureMouse;
    });

    input.BindKey(GLFW_KEY_1, KEY_DOWN, [this]() {
        targetParticleType = ParticleType::ParticleType_Proton;
        simInterface.selectedType = ParticleType::ParticleType_Proton;
    });

    input.BindKey(GLFW_KEY_2, KEY_DOWN, [this]() {
        targetParticleType = ParticleType::ParticleType_Neutron;
        simInterface.selectedType = ParticleType::ParticleType_Neutron;
    });

    input.BindKey(GLFW_KEY_3, KEY_DOWN, [this]() {
        targetParticleType = ParticleType::ParticleType_Electron;
        simInterface.selectedType = ParticleType::ParticleType_Electron;
    });

    input.BindKey(GLFW_KEY_4, KEY_DOWN, [this]() {
        targetParticleType = ParticleType::ParticleType_Photon;
        simInterface.selectedType = ParticleType::ParticleType_Photon;
    });

    input.BindScroll([this](double, double y) {
        camera->Zoom((float) y);
    });
}

void Application::Run() {
    glfwPollEvents();

    Application &app = GetInstance();

    glClearColor(0.04f, 0.04, 0.04f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    simInterface.Show();

    input.Update();

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        isPanning = false;
    }

    if (isPanning) {
        glm::vec2 current(
            inputManager.getMouseX(),
            inputManager.getMouseY()
        );

        glm::vec2 delta = current - lastMouse;

        float orthoSize = 10.0f / camera->GetZoom();
        float aspect = camera->GetAspect();

        delta.x = -delta.x / camera->GetWidth()  * orthoSize * aspect * 2.0f;
        delta.y =  delta.y / camera->GetHeight() * orthoSize * 2.0f;

        camera->Move(delta.x, delta.y, true);

        lastMouse = current;
    }

    if (captureMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    simulationSystem.RenderAll(
        shader->ID,
        camera->GetProjection(),
        camera->GetView()
    );

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Application::InitialiseImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); // Create ImGui Context
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

#if DEBUG
    _imguiTestEngine = ImGuiTestEngine_CreateContext();
    ImGuiTestEngineIO &testIo = ImGuiTestEngine_GetIO(_imguiTestEngine);
    testIo.ConfigVerboseLevel = ImGuiTestVerboseLevel_Info;
    testIo.ConfigVerboseLevelOnError = ImGuiTestVerboseLevel_Debug;

    registerImGuiTests();

    ImGuiTestEngine_Start(_imguiTestEngine, ImGui::GetCurrentContext());
    ImGuiTestEngine_InstallDefaultCrashHandler();
#endif
    ImGuiIO &io = ImGui::GetIO();
    // Ini file saving is handled by the WorkspaceManager.
    io.IniFilename = nullptr;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    io.ConfigFlags |= ImGuiConfigFlags_None | ImGuiConfigFlags_NavEnableKeyboard;

#ifndef DEBUG
    io.ConfigDebugHighlightIdConflicts = false;
#endif
}

void Application::TerminateGlfw() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void Application::TerminateImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GLFWwindow *Application::getWindow() {
    return window;
}

Viewport *Application::getCamera() {
    return camera;
}

std::string Application::GetExecutablePath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count != -1) {
        std::string path(result, count);
        size_t pos = path.find_last_of('/');
        return path.substr(0, pos);
    }
    return "";
}

glm::vec2 Application::ScreenToWorld(double mouseX, double mouseY) {
    float x = (2.0f * mouseX) / camera->GetWidth() - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / camera->GetHeight();

    glm::vec4 ndc(x, y, 0.0f, 1.0f);

    glm::mat4 invVP = glm::inverse(
        camera->GetProjection() * camera->GetView()
    );

    glm::vec4 world = invVP * ndc;
    return glm::vec2(world);
}
