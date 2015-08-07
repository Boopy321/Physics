#include "PhysicTutorial.h"
#include "PhysicsActor.h"
#include "GLFW\glfw3.h"
#include "Assets\Camera\FlyCamera.h"
#include <cfloat>
PhysicTutorial::PhysicTutorial()
{
	DIYPhysicsSetup1();
}

PhysicTutorial::~PhysicTutorial()
{
}

void PhysicTutorial::DIYPhysicsSetup1()
{
	physicsScene = new DIYPhysicScene();
	physicsScene->gravity = glm::vec3(0, -10.0,0);
	physicsScene->timeStep = .001f;
	//add four balls to our simulation
	PlaneClass* newLine;
	BoxClass* newBox;
	SphereClass* newBall;
  
	newBox = new BoxClass(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 1.0f, false);
	physicsScene->addActor(newBox);
  	
	newBox = new BoxClass(glm::vec3(30, 10, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 1.0f, false);
	physicsScene->addActor(newBox);
  	
	newBox = new BoxClass(glm::vec3(40, 0, 10), glm::vec3(0, 0, 10), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 1.0f, false);
	physicsScene->addActor(newBox);
	
	
	newBox = new BoxClass(glm::vec3(10, 0, 10), glm::vec3(0, 10, 100), 1.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 1.0f, false);
	physicsScene->addActor(newBox);
	
	
	newBox = new BoxClass(glm::vec3(16, 0, 10), glm::vec3(0, 0, 10), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 1.0f, false);
	physicsScene->addActor(newBox);
	
	
	newBox = new BoxClass(glm::vec3(5, 0, 0), glm::vec3(-50,0 , 0), 5.0f, glm::vec3(4, 2, 4), glm::vec4(0, 1, 1, 1), false, 1.0f, false);
	physicsScene->addActor(newBox);


  
	
	newBall = new SphereClass(glm::vec3(12, 12, 12), glm::vec3(-50,-50, -50), 1.0f, 8,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 35, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 30, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 25, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 29, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 20, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 15, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(15, 0, 0), glm::vec3(0, 50, 0), 1.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f, false);
	physicsScene->addActor(newBall);

	//SPRING JOINt
	SphereClass* ball1;
	SphereClass* ball2;
	float ballRadius = 2;
	float mass = 2;
	ball1 = new SphereClass(glm::vec3(-100, 40, 0), glm::vec3(0, 0, 0), FLT_MAX, 3,
		glm::vec4(1, 0, 0, 1),0.9f,true);
	
	
	physicsScene->addActor(ball1);

	int numberBalls = 30;
	for (int i = 1; i <= numberBalls; i++)
	{
		if (i == numberBalls || i == numberBalls /2)
		{
			ball2 = new SphereClass(glm::vec3((i * 6.0f) - 100, 40, 0), glm::vec3(0, 0, 0), FLT_MAX, 2,
				glm::vec4(0, 1, 0, 1), 0.9f, true);


		}
		else
		{
			ball2 = new SphereClass(glm::vec3((i * 6.0f) - 100, 40, 0), glm::vec3(0, 0, 0), 1.0f, 2,
				glm::vec4(0, 1, 0, 1), 0.9f, false);
		}
	

		physicsScene->addActor(ball2);
		SpringJoint* spring = new SpringJoint(ball1, ball2, 5.0f, .9999f);
		physicsScene->addActor(spring);
		ball1 = ball2;

	}
	

	//Ground
	newLine = new PlaneClass(glm::vec3(0, 1, 0), -160);
	physicsScene->addActor(newLine);
	//left wall
	newLine = new PlaneClass(glm::vec3(1, 0, 0), -160);
	physicsScene->addActor(newLine);
	//right wall 
	newLine = new PlaneClass(glm::vec3(-1, 0, 0), -160);
	physicsScene->addActor(newLine);
	//Roof
	newLine = new PlaneClass(glm::vec3(0, -1, 0), -160);
	physicsScene->addActor(newLine);
	
	//Closest Wall on the z
	newLine = new PlaneClass(glm::vec3(0, 0, 1), -160);
	physicsScene->addActor(newLine);
	//Furthest wall on the z
	newLine = new PlaneClass(glm::vec3(0, 0, -1), -160);
	physicsScene->addActor(newLine);

}

void PhysicTutorial::Update(float deltatime, FlyCamera &a_gameCamera)
{

	m_pWindow = glfwGetCurrentContext();
	//New gun for Custom Engine for testing purposes
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{

		glm::vec3 startingPosition(a_gameCamera.getWorldTransform()[3]);
		glm::vec3 direction(a_gameCamera.getWorldTransform()[2]);

		float density = 10;
		float muzzleSpeed = 500;
		
		SphereClass* newBall = new SphereClass(startingPosition, -direction * muzzleSpeed, 1.0f, 3,
			glm::vec4(1, 0, 0, 1), 0.9f, false);
		physicsScene->addActor(newBall);


	}

	physicsScene->upDate();

}



