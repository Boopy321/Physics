#include "Camera.h"
#include "GLFW\glfw3.h"
#include <glm\glm.hpp>
Camera::Camera() :Camera(mat4(1))
{

}

Camera::Camera(vec3 position) : Camera()
{
	m_worldTransform[3] = vec4(position, 1);
	m_bPerspectiveSet = false;
}

Camera::Camera(mat4 transform) : m_worldTransform(transform)
{
	UpdateProjectionViewTransform();
}

void Camera::SetTransform(mat4 transform)
{
	m_worldTransform = transform;
	UpdateProjectionViewTransform();
}

//Sets the camera postion
void Camera::setPersepective(float FOV, float aspectRatio, float near = 0.1f, float far = 1000.0f)
{
	m_projectionTransform = glm::perspective(FOV, aspectRatio, near, far);
	m_bPerspectiveSet = true;

	UpdateProjectionViewTransform();
}

void Camera::setLookAt(vec3 position, vec3 lookAt, vec3 worldUp)
{
	m_worldTransform = glm::inverse(glm::lookAt(position, lookAt, worldUp));
	UpdateProjectionViewTransform();
}

void Camera::setLookAt(vec3 lookAt, vec3 worldup)
{
	vec4 vLocation = m_worldTransform[3];
	m_worldTransform = glm::inverse(glm::lookAt(vLocation.xyz(), lookAt, worldup));
	UpdateProjectionViewTransform();
}

void Camera::setPosition(vec3 pos)
{
	m_worldTransform[3] = vec4(pos, 1);
	UpdateProjectionViewTransform();
}

void Camera::UpdateProjectionViewTransform()
{
	m_viewTransform = glm::inverse(m_worldTransform);
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}

mat4 Camera::getWorldTransform()
{
	return m_worldTransform;
}
Camera::~Camera()
{

}


glm::vec3 Camera::screenPositionToDirection(float x, float y) const {

	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projectionTransform[0][0];
	screenPos.y /= m_projectionTransform[1][1];

	return glm::normalize(m_worldTransform * glm::vec4(screenPos, 0)).xyz();
}

glm::vec3 Camera::pickAgainstPlane(float x, float y, const glm::vec4& plane) const {

	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projectionTransform[0][0];
	screenPos.y /= m_projectionTransform[1][1];

	glm::vec3 dir = glm::normalize(m_worldTransform * glm::vec4(screenPos, 0)).xyz();

	float d = (plane.w - glm::dot(m_worldTransform[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	return m_worldTransform[3].xyz() + dir * d;
}
