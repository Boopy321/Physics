#pragma once
#include "Camera.h"
#include "GLFW\glfw3.h"


class FlyCamera :
	public Camera
{
public:
	FlyCamera();

	~FlyCamera() {};
	virtual void update(double dt);

	void SetFlySpeed(float speed);
	float GetFlySpeed() const { return m_fFlySpeed; }
	void SetInputWindow(GLFWwindow* pWindow){ m_pWindow = pWindow; }


protected:
	void HandleKeyboardInput(double dt);
	void HandleMouseInput(double dt);

	void CalculateRoation(glm::vec2 a_Offset);

	GLFWwindow* m_pWindow;
	float m_fFlySpeed;

	bool m_bViewButtonClicked;
	double m_dCursorX, m_dCursorY;

	
};

