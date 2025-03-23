#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/rotate_vector.hpp"
#include <iostream>

class Camera {
public:
    Camera(float sens);

    void UpdateAspectRatio(float aspect);

    void MouseLook(int mouseX, int mouseY);
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

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

    glm::vec3 eye;
    glm::vec3 lookDirection;
    glm::vec3 upVector;

    float sensitivity;
    float fovy;
    float aspectRatio;
    float near;
    float far;

    glm::vec2 oldMousePos;
};

#endif