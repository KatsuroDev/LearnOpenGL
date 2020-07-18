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
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera
{
public:
  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
  // Constructor with scalar
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

  // Returns the view matrix calculated using Euler's angles and the LookAt matrix
  glm::mat4 GetViewMatrix();

  // Processes input from any keyboard-like input system. We give it in camera defined ENUM to abstract it.
  void ProcessKeyboard(CameraMovement direction, float deltaTime);

  // Processes input from a mouse input system. Expects the x and y offset.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

  // Processes input from a mouse scroll-wheel event. Only requires the y axis.
  void ProcessMouseScroll(float yoffset);

  // Get attributes
  inline glm::vec3 GetPosition() { return m_position; }
  inline glm::vec3 GetFrontVec() { return m_front; }
  inline glm::vec3 GetUpVec() { return m_up; }
  inline glm::vec3 GetRightVec() { return m_right; }
  inline glm::vec3 GetWorldUpVec() { return m_worldUp; }
  inline float GetYaw() { return m_yaw; }
  inline float GetPitch() { return m_pitch; }
  inline float GetMovementSpeed() { return m_movementSpeed; }
  inline float GetMouseSensitivity() { return m_mouseSensitivity; }
  inline float GetFov() { return m_fov; }

  // Set attributes
  inline void SetPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); }
  inline void SetPosition(glm::vec3 pos) { m_position = pos; }
  inline void SetFrontVec(float x, float y, float z) { m_front = glm::normalize(glm::vec3(x, y, z)); updateCameraVectors();/*Update Right and Up vectors as well*/ }
  inline void SetFrontVec(glm::vec3 front) { m_front = glm::normalize(front); updateCameraVectors(); }
  inline void SetYaw(float yaw) { m_yaw = yaw; updateCameraVectors(); }
  inline void SetPitch(float pitch) { m_pitch = pitch; updateCameraVectors(); } // Update the Front, Right and Up vectors with the new Euler angle
  inline void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
  inline void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
  inline void SetFov(float fov) { m_fov = fov; } // Negative fov will invert the vision. Could be useful sometime I guess.




private:
  // Updates the vectors using the updated Euler angles.
  void updateCameraVectors();

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

#endif
