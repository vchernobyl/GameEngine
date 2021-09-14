#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    void Init(int screenWidth, int screenHeight);
    void Update();

    void SetPosition(const glm::vec2& position) { this->position = position; needsMatrixUpdate = true; }
    const glm::vec2& GetPosition() const { return position; }

    void SetScale(float scale) { this->scale = scale; needsMatrixUpdate = true; }
    float GetScale() const { return scale; }

    const glm::mat4& GetCameraMatrix() const { return cameraMatrix; }
    glm::vec2 ScreenToWorld(glm::vec2 screenCoords);

private:
    float scale;
    glm::vec2 position;
    glm::mat4 cameraMatrix;
    glm::mat4 orthoMatrix;
    bool needsMatrixUpdate;
    int screenWidth, screenHeight;
};
