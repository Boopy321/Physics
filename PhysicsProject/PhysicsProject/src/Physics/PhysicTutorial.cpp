#include "PhysicTutorial.h"
#include "PhysicsActor.h"
#include "GLFW\glfw3.h"

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
	physicsScene->gravity = glm::vec3(0, -10.0, 0);
	physicsScene->timeStep = .001f;
	//add four balls to our simulation
	SphereClass* newBall;
	newBall = new SphereClass(glm::vec3(0.5, 10, 0), glm::vec3(0, 0, 0), 20.0f, 1,
		glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(-5, 0, 0), glm::vec3(0, 20, 0), 3.0f, 1,
		glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(5, 20, 0), glm::vec3(200, 0, 0), 9.0f, 3,
		glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(0, 20, 0), glm::vec3(-2000, 0, 0), 40.0f, 1,
		glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(newBall);

	PlaneClass* newLine;
	//floor
	newLine = new PlaneClass(glm::vec3(0, 1, 0), 30);
	physicsScene->addActor(newLine);
	//left wall
	newLine = new PlaneClass(glm::vec3(1, 0, 0), 30);
	physicsScene->addActor(newLine);
	//right wall 
	newLine = new PlaneClass(glm::vec3(-1, 0, 0), 30);
	physicsScene->addActor(newLine);
	//Roof
	newLine = new PlaneClass(glm::vec3(0, -1, 0), 30);
	physicsScene->addActor(newLine);
}

void PhysicTutorial::Update(float deltatime)
{
	
	//if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)	
	//{
	//	SphereClass* newBall = new SphereClass(glm::vec3(0, 20, 0), glm::vec3(-2, 0, 0), 40.0f, 1,
	//		glm::vec4(1, 0, 0, 1));
	//	physicsScene->addActor(newBall);
	//}


	physicsScene->upDate();

}


//void PhysicTutorial::onUpdate(float deltaTime)
//{
//
//	//physicsScene->upDateGizmos();
//}