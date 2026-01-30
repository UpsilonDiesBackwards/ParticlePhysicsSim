#include "../include/viewport.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../include/application.h"

Viewport::Viewport(float width, float height, float zoom)
    : position(0.0f, 0.0f),
      zoom(zoom),
      width(width),
      height(height) {
}

void Viewport::Move(float xOffset, float yOffset, bool rawWorldUnits) {
    if (rawWorldUnits) {
        position.x += xOffset;
        position.y += yOffset;
    } else {
        position.x += xOffset * (_panSpeed * GET_APP.camera->GetZoom()) * GET_APP.time.GetDeltaTime();
        position.y += yOffset * (_panSpeed * GET_APP.camera->GetZoom()) * GET_APP.time.GetDeltaTime();
    }
}

void Viewport::Zoom(float amount) {
    constexpr float zoomFactor = 1.025f;

    if (amount > 0.0f) {
        zoom *= zoomFactor;
    } else if (amount < 0.0f) {
        zoom /= zoomFactor;
    }

    zoom = glm::clamp(zoom, _zoomMin, _zoomMax);
}

void Viewport::UpdateViewport(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

glm::mat4 Viewport::GetProjection() {
    const float aspectRatio = width / height;
    const float orthoSize = 10.0f / zoom;

    return glm::ortho(
        -orthoSize * aspectRatio,
         orthoSize * aspectRatio,
        -orthoSize,
         orthoSize,
        -1.0f,
         1.0f
    );
}

glm::mat4 Viewport::GetView() {
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(
        view,
        glm::vec3(-position, 0.0f)
    );

    return view;
}

glm::vec2 Viewport::GetPosition() const {
    return position;
}

float Viewport::GetZoom() const {
    return zoom;
}