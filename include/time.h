//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_TIME_H
#define SIMULATION_TIME_H

/*
 * \brief The "time" inside Marmalade. Handles deltaTime updates and the fixed time step used for physics
 */
class Time {
public:
    Time();

    /**
     * \brief Updates the application frame and delta time.
     */
    void Update();

    /**
     * \brief Updates the fixed update using the fix time step and the accumulator. This is intended for physics-based updates
     */
    void FixedUpdate();

    int GetCurrentFPS() const;
    float GetCurrentFrameTime() const;
    double GetDeltaTime() const;

    float GetFixedDeltaTime();

private:
    double lastTime;
    double deltaTime;
    int fps;
    float frameTime;

    // Accumulator for phys updates
    float _accumulator = 0.0f;
    float fixedTimeStep = 1.0f / 60.0f; // Target is 60 updates per second
};

#define GET_DELTA_TIME GET_APP.time.GetDeltaTime()
#define GET_FIXED_DELTA_TIME GET_APP.time.GetFixedDeltaTime()

#endif