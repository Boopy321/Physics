#pragma once

class DIYPhysicScene;
struct GLFWwindow;
class FlyCamera;
class PhysicTutorial
{
public:
	PhysicTutorial();
	~PhysicTutorial();

	void DIYPhysicsSetup1();
	void onUpdate(float deltatime);
	void Update(float deltatime, FlyCamera &a_gameCamera);



private: 
	DIYPhysicScene* physicsScene;
	GLFWwindow* m_pWindow;
};

