#include "FlyCamera.h"
#include <GLFW\glfw3.h>
#include <AntTweakBar.h>
#include <iostream>


FlyCamera::FlyCamera()
{						//Position           //Point of View    //Up vector
	m_viewTransform = lookAt(vec3(0,10,10), vec3(0,0,1), vec3(0, 1,0));
	m_worldTransform = glm::inverse(m_viewTransform);
	setPersepective(glm::pi<float>()*0.25f, 16.f / 9.f, 0.1f, 1000.f);
	m_fFlySpeed = 100.f;
	
}


void FlyCamera::update(double dt)
{
	HandleKeyboardInput(dt);
	HandleMouseInput(dt);

	UpdateProjectionViewTransform();
}

void FlyCamera::HandleKeyboardInput(double dt)
{
	glm::vec3 vRight = m_worldTransform[0].xyz;
	glm::vec3 vUp = m_worldTransform[1].xyz;
	glm::vec3 vForward = m_worldTransform[2].xyz;

	glm::vec3 moveDir(0.0f);

	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDir -= vForward;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDir += vForward;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDir -= vRight;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDir += vRight;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);
	}

	float fLength = glm::length(moveDir);

	if (fLength > 0.01f)
	{
		moveDir = ((float)dt * m_fFlySpeed * glm::normalize(moveDir));
		setPosition(GetPosition() + moveDir);
	}
}

void FlyCamera::HandleMouseInput(double dt)
{
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		if(m_bViewButtonClicked == false)
		{
			int width, height;
			glfwGetFramebufferSize(m_pWindow, &width, &height);

			m_dCursorX = width / 2.0;
			m_dCursorY = height / 2.0;
			glfwSetCursorPos(m_pWindow, m_dCursorX, m_dCursorY);
			m_bViewButtonClicked = true;
		}
		else
		{
			double mouseX;
			double mouseY;
			glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);

			double xOffset = mouseX - m_dCursorX;
			double yOffset = mouseY - m_dCursorY;

			m_dCursorX = mouseX;
			m_dCursorY = mouseY;

			CalculateRoation(glm::vec2(xOffset,yOffset));

		}
	}
	else
	{
		m_bViewButtonClicked = false;
	}
}

void FlyCamera::CalculateRoation(glm::vec2 a_Offset)
{
	//Grabs XYZ VALUES WITHOUT THE END
	glm::vec3 vRight = glm::vec3(1, 0, 0);
	glm::vec3 vUp = glm::vec3(0, 1, 0);
	glm::vec3 Vector3z = m_worldTransform[2].xyz;
	//Temporary Matrixs to deal with rotation
	glm::mat4 xRotMatrix;
	glm::mat4 yRotMatrix;
	//Creates the actual rotated Matrix
	xRotMatrix = glm::rotate(-a_Offset.x * 0.001f, vUp);
	yRotMatrix = glm::rotate(-a_Offset.y * 0.001f, vRight);

	//Now got to apply that to world.
	m_worldTransform = m_worldTransform * xRotMatrix* yRotMatrix;

	//Cross product of the vectors 
	vRight = glm::cross(glm::vec3(0, 1, 0), m_worldTransform[2].xyz());
	vUp = glm::cross(m_worldTransform[2].xyz(), vRight);
	//Nomarlize the bizare values
	vRight = glm::normalize(vRight);
	vUp = glm::normalize(vUp);
	
	//OVerride the original Transform values and chuck them back in it.
	m_worldTransform[0] = vec4(vRight, 0);
	m_worldTransform[1] = vec4(vUp, 0);
	//Updates the View matrix and Projection view
	UpdateProjectionViewTransform();
}


