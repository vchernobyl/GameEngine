#include "Camera.h"

Camera::Camera() :
    scale(1.0f),
    position(0.0f, 0.0f),
    cameraMatrix(1.0f),
    orthoMatrix(1.0f),
    needsMatrixUpdate(true),
    screenWidth(500),
    screenHeight(500) {
}

void Camera::Init(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
}

void Camera::Update() {
    if (needsMatrixUpdate) {
	glm::vec3 translate(-position.x + screenWidth / 2, -position.y + screenHeight / 2, 0.0f);
	cameraMatrix = glm::translate(orthoMatrix, translate);
	cameraMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 0.0f)) * cameraMatrix;
	needsMatrixUpdate = false;
    }
}

glm::vec2 Camera::ScreenToWorld(glm::vec2 screenCoords) {
    // Invert y coords.
    screenCoords.y = screenHeight - screenCoords.y;
    
    // Make (0, 0) to be at the center of the screen.
    screenCoords -= glm::vec2(screenWidth / 2, screenHeight / 2);
    screenCoords /= scale;
    screenCoords += position;
    return screenCoords;
}

bool Camera::IsBoxInView(const glm::vec2& boxPosition, const glm::vec2& size) const {
    glm::vec2 scaledScreenSize = glm::vec2(screenWidth, screenHeight) / scale;
    float minDistanceX = size.x / 2.0f + scaledScreenSize.x / 2.0f;
    float minDistanceY = size.y / 2.0f + scaledScreenSize.y / 2.0f;

    glm::vec2 centerPos = boxPosition + size / 2.0f;
    glm::vec2 distVec = boxPosition - position;

    float xDepth = minDistanceX - abs(distVec.x);
    float yDepth = minDistanceY - abs(distVec.y);

    if (xDepth > 0 && yDepth > 0) {
	return true;
    }

    return false;
}
