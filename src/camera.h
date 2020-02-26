#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera
{
public:
  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV);
  // Constructor with scalar
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV);

  // Returns the view matrix calculated using Euler's angles and the LookAt matrix
  glm::mat4 GetViewMatrix();

  void ProcessKeyBoard(CameraMovement direction, float deltaTime);

  void ProcessMouseMovement()

private:
  // Camera's vectors
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;
  // Euler's angles
  float m_yaw;
  float m_pitch;
  // Camera's options
  float m_movementSpeed;
  float m_mouseSensitivity;
  float m_fov;

};
