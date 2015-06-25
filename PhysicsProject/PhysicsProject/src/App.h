#pragma once


class Physics;
struct GLFWwindow;
#include "Assets/Camera/FlyCamera.h"
class AntTweakBar;
struct Light;

class App
{
public:
	App();
	~App();
	bool StartUp();
	void Run();


	void Update(float deltatime);
	void Draw(float a_deltatime);
	void Shutdown();

protected:
	GLFWwindow* window;
	float previousTime;
	FlyCamera _gameCamera;

private:
	

	float m_frameCounter;
	float m_framerate;
	float m_frameElaspedtime;

	int m_wHeight;
	int m_wWidth;

	AntTweakBar* m_bar;

	Light* m_light;

	glm::vec4 m_clearColour;

	Physics* m_Physics;
};

