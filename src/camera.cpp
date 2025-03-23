#include <camera.hpp>

Camera::Camera(float sens) {
    sensitivity = sens / 10.0f;
    eye = glm::vec3(0.0f, 0.0f, 0.0f);
    lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    fovy = 60.0f;
    aspectRatio = 1.0f;
    near = 0.01f;
    far = 50.0f;
}

void Camera::UpdateAspectRatio(float aspectRatio) {
    projectionMatrix = glm::perspective(fovy, aspectRatio, near, far);
}

void Camera::MouseLook(int mouseX, int mouseY) {
    glm::vec2 currentMouse = glm::vec2((float) mouseX, (float)mouseY);
    static bool firstLook = true;
    if (firstLook) {
        oldMousePos = currentMouse;
        firstLook = false;
    }

    glm::vec2 mouseDelta = oldMousePos - currentMouse;

    // Horizontal
    lookDirection = glm::rotate(lookDirection, sensitivity * glm::radians(mouseDelta.x), upVector);
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

void Camera::SetFovy(float value) {
    fovy = value;
    projectionMatrix = glm::perspective(glm::radians(fovy), aspectRatio, near, far);
}

void Camera::SetEyePosition(glm::vec3 position) {
    eye = position;
}

// Getters
float Camera::GetFovy() { return fovy; }

glm::vec3 Camera::GetEye() { return eye; }
glm::vec3 Camera::GetLookDir() { return lookDirection; }

glm::mat4 Camera::GetProjectionMatrix() const {
    return projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(eye, eye + lookDirection, upVector);
}