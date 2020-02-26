#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV)
{
  m_position = position;
  m_worldUp = up;
  m_yaw = yaw;
  m_pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV)
{
  m_position = glm::vec3(posX, posY, posZ);
  m_worldUp = glm::vec3(upX, upY, upZ);
  m_yaw = yaw;
  m_pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
  return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
  float velocity = m_movementSpeed * deltaTime;
  if(direction == FORWARD)
    m_position += m_front * velocity;
  if(direction == BACKWARD)
    m_position -= m_front * velocity;
  if(direction == RIGHT)
    m_position += m_right * velocity;
  if(direction == LEFT)
    m_position -= m_right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
  xoffset *= m_mouseSensitivity;
  yoffset *= m_mouseSensitivity;

  m_yaw += xoffset;
  m_pitch += yoffset;

  if(constrainPitch)
  {
    if(m_pitch > 89.0f)
      m_pitch = 89.0f;
    if(m_pitch < -89.0f)
      m_pitch = -89.0f;
  }

  // Update Front, Right and Up vectors using the updated Euler angles
  updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
  if(m_fov >= 1.0f && m_fov <= 45.0f)
    m_fov -= yoffset;
  if(m_fov <= 1.0f)
    m_fov = 1.0f;
  else if(m_fov >= 45.0f)
    m_fov = 45.0f;
}

void Camera::updateCameraVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);

  // Also re-calucalte the right and up vector;
  m_right = glm::normalize(glm::cross(m_front, m_worldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movemnt.
  m_up = glm::normalize(glm::cross(m_right, m_front));
}
