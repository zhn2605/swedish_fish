#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/rotate_vector.hpp"
#include <iostream>

class Camera {
public:
    Camera(float sens);

    void UpdateProjectionMatrix();
    void UpdateViewMatrix();
    void UpdateMatrices();

    void MouseLook(int mouseX, int mouseY);
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

    void SetAspectRatio(float value);
	void SetProjectionMatrix(float fovy, float aspect, float near, float far);
	void SetFovy(float fovy);
	void SetEyePosition(glm::vec3 position);

	float GetFovy();
	glm::vec3 GetEye();
	glm::vec3 GetLookDir();
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;

private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    glm::vec3 eye;
    glm::vec3 lookDirection;
    glm::vec3 upVector;

    float sensitivity;
    float fovy;
    float aspect;
    float near;
    float far;

    glm::vec2 oldMousePos;
};

#endif