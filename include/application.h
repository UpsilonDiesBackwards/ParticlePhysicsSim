//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_APPLICATION_H
#define SIMULATION_APPLICATION_H

#include <string>

#include "time.h"
#include "viewport.h"
#include "io/input.h"
#include "io/inputmanager.h"
#include "sim/interface.h"
#include "sim/system.h"

enum PlayState {
    PlayState_PLAY,
    PlayState_STOP,
    PlayState_PAUSE,
    PlayState_STEP
};

class Application {
public:
    static Application& GetInstance(int width = 1920, int height = 1080, const char* title = "Engine") {
        static Application instance(width, height, title);
        return instance;
    }
    ~Application();

    void Initialise();

    void Run();

    void InitialiseImGui();

    void TerminateGlfw();

    void TerminateImGui();

    PlayState playState = PlayState::PlayState_STOP;

    GLFWwindow* getWindow();
    Viewport* getCamera();

    std::string GetExecutablePath();

    InputManager& inputManager;
    Input input;

    Viewport* camera;

    Time time;

    ParticleType targetParticleType = ParticleType::ParticleType_Proton;

    SimulationSystem simulationSystem;
    Interface simInterface;

    bool captureMouse = false;
private:
    Application(int width, int height, const char* title);

    void SetupInputBindings();

    GLFWwindow* window;
    int width, height;
    const char* title;

    Shader* shader;

    glm::vec2 ScreenToWorld(double mouseX, double mouseY);

    bool isPanning;
    glm::vec2 lastMouse;
};

#define GET_APP Application::GetInstance()

#endif //SIMULATION_APPLICATION_H