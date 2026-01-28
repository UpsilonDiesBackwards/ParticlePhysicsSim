//
// Created by tayler on 12/14/25.
//

#include "../include/time.h"

#include "../include/application.h"

/**
 * \brief Time constructor, sets defaults values
 */

Time::Time() : lastTime(0.0), deltaTime(0.0), fps(0), frameTime(0.0) {
}


void Time::Update() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    fps = static_cast<int>(1.0 / deltaTime);

    frameTime = deltaTime * 1000.0f; // Convert to milliseconds
}

/**
 * \brief If the engine play state is set to anything other than PlayState_STEP reset the accumulator
 * \brief if not, increase the accumulator by fixedTimeStep
 */
void Time::FixedUpdate() {
    if (Application::GetInstance().playState != PlayState::PlayState_STEP &&
        Application::GetInstance().playState != PlayState::PlayState_PAUSE) {

        _accumulator = fixedTimeStep;
    } else {
        _accumulator += fixedTimeStep;
    }

    while (_accumulator >= fixedTimeStep) { // We use an accumulator to prevent physic jitteriness
        _accumulator -= fixedTimeStep;
    }
}

/**
 * \brief Returns the current frame rate
 * \return int fps
 */

int Time::GetCurrentFPS() const {
    return fps;
}

/**
 * \brief Returns the current frame time (time it takes to complete a frame in ms)
 * \return float frameTime
 */

float Time::GetCurrentFrameTime() const {
    return frameTime;
}

/**
 * \brief Returns the deltaTime
 * \return double deltaTime
 */

double Time::GetDeltaTime() const {
    return deltaTime;
}

/**
 * \brief Returns fixed deltaTime (deltaTime from fixed time step)
 * \return float fixedTimeStep
 */

float Time::GetFixedDeltaTime() {
    return fixedTimeStep;
}