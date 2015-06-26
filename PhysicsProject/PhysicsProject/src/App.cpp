#include "gl_core_4_4.h"
#include "App.h"
#include "GLFW\glfw3.h"
#include "Physics/Physics.h"
#include "Physics\PhysicTutorial.h"
#include "AntTweakBar.h"
#include "Assets\Camera\FlyCamera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <string>
#include "Gizmos.h"
#include <fstream>

App::App()
{
	m_wWidth = 1024;
	m_wHeight = 800;
	m_Physics = new PhysicTutorial();
}

App::~App()
{
}

#pragma region Update/Draw/Startup

bool App::StartUp()
{
	if (glfwInit() == false)
		return false;
	
	window = glfwCreateWindow(m_wWidth, m_wHeight, "Physics", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	std::cout << "OpenGL loaded" << std::endl;

	_gameCamera.SetInputWindow(window);

	//Load Render into Tutorial
	
	m_clearColour = glm::vec4(1.0f, 1.0f, 1.00f, 1.0f);
	return true;
}

void App::Run()
{
	if (StartUp() == false)
		return;

		Gizmos::create();//0xFFFFFF, 0xFFFFFF);
	
		//allows Depth varible to be used
		glEnable(GL_DEPTH_TEST);
		while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
		{
			//Deltatime stuff
			float currentTime = (float)glfwGetTime();
			float deltatime = currentTime - previousTime;
			previousTime = currentTime;
			Update(deltatime);
		}
	
		Shutdown();
}

void App::Update(float deltatime)
{
	glClearColor(m_clearColour.x, m_clearColour.y, m_clearColour.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Camera
	_gameCamera.update(deltatime);
	//Current Games Draw
	Draw(deltatime);

	glfwSwapBuffers(window);
	glfwPollEvents();

	m_frameElaspedtime += deltatime;

	if (m_frameElaspedtime > 1.0f)
	{
		m_frameElaspedtime = 0.0f;
		m_framerate = m_frameCounter;
		m_frameCounter = 0;
	}
}

void App::Shutdown()
{
	//must Use with PHYSX Engine
		//m_Physics->Shutdown();
	TwDeleteAllBars();
	TwTerminate();
	glfwDestroyWindow(window);
	glfwTerminate();

}

void App::Draw(float a_deltatime)
{
	m_frameCounter++;
	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1), 5.0f);
	m_Physics->Update(a_deltatime);
	Gizmos::draw(_gameCamera.getProjectionView());
}
#pragma endregion