#include <camera.hpp>

Camera::Camera(float sens) {
    sensitivity = sens / 10.0f;
    eye = glm::vec3(0.0f, 0.0f, 10.0f);
    lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    fovy = 60.0f;
    aspect = 1.0f;
    near = 0.01f;
    far = 50.0f;
    base_speed = 10.0f;

    UpdateMatrices();
}

void Camera::UpdateMatrices() {
    projectionMatrix = glm::perspective(glm::radians(fovy), aspect, near, far);
    viewMatrix = glm::lookAt(eye, eye + lookDirection, upVector);
}

void Camera::MouseLook(float mouseX, float mouseY) {
    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);
    static bool firstLook = true;
    if (firstLook) {
        oldMousePos = currentMouse;
        firstLook = false;
    }

    glm::vec2 mouseDelta = oldMousePos - currentMouse;

    // Horizontal
    lookDirection = glm::rotate(lookDirection, sensitivity * glm::radians(mouseDelta.x), upVector);

    // Vertical
    glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));
    lookDirection = glm::rotate(lookDirection, sensitivity * glm::radians(mouseDelta.y), rightVector);
    
    lookDirection = glm::normalize(lookDirection);
    oldMousePos = currentMouse;
}

void Camera::MoveForward(float speed) {
    eye += (lookDirection * speed);
}

void Camera::MoveBackward(float speed) {
    eye -= (lookDirection * speed);
}

void Camera::MoveLeft(float speed) {
    glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));
    eye -= (rightVector * speed);
}

void Camera::MoveRight(float speed) {
    glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));
    eye += (rightVector * speed);
}

void Camera::MoveUp(float speed) {
    eye.y += glm::normalize(upVector).y * speed;
}

void Camera::MoveDown(float speed) {
    eye.y -= glm::normalize(upVector).y * speed;
}

void Camera::SetAspectRatio(float value) {
    aspect = value;
    UpdateMatrices();
}

void Camera::SetFovy(float value) {
    fovy = value;
}

void Camera::SetEyePosition(glm::vec3 position) {
    eye = position;
}

// Getters
float Camera::GetFovy() { return fovy; }
float Camera::GetSpeed() { return base_speed; }
glm::vec3 Camera::GetEye() { return eye; }
glm::vec3 Camera::GetLookDir() { return glm::normalize(lookDirection); }

glm::mat4 Camera::GetProjectionMatrix() const {
    return projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const {
    return viewMatrix;
}