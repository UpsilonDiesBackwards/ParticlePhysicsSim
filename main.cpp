#include "include/application.h"

int main() {
    Application& application =
        Application::GetInstance(1080, 720, "Particle Physics Simulation");

    application.Initialise();
    application.InitialiseImGui();

    while (!glfwWindowShouldClose(application.getWindow())) {
        glfwPollEvents();

        application.Run();
    }

    application.TerminateImGui();
    application.TerminateGlfw();

    return 0;
}

