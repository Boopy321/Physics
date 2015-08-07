#pragma once

class Physics;
struct GLFWwindow;
class PhysicTutorial;
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
	bool keypress;

	void Update(float deltatime);
	void Draw(float a_deltatime);
	void Shutdown();

	bool GetKeyDown();
	static void ChangePhysics();

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
	Physics* m_PhyX;
	PhysicTutorial* m_Physics;

	static bool _Phyx;
};

