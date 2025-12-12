//
// Created by tayler on 12/11/25.
//

#ifndef SIMULATION_VIEWPORT_H
#define SIMULATION_VIEWPORT_H

#include <glm/glm.hpp>

class Viewport {
public:
    Viewport(float width, float height, float zoom = 0.0f);

    void Move(float xOffset, float yOffset, bool rawWorldUnits = false);
    void Zoom(float amount);

    void UpdateViewport(float width, float height);

    glm::mat4 GetProjection();
    glm::mat4 GetView();
    glm::vec2 GetPosition() const;

    float GetZoom() const;
private:
    glm::vec2 position;
    float zoom;
    float width, height;

    glm::mat4 viewMatrix, projectionMatrix;

    float _panSpeed = 0.3f;

    float _zoomMin = 0.01f;
    float _zoomMax = 2500.0f;
};



#endif //SIMULATION_VIEWPORT_H