#include "PhysicTutorial.h"
#include "PhysicsActor.h"

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
	physicsScene->gravity = glm::vec2(0, -10);
	physicsScene->timeStep = .001f;
	//add four balls to our simulation
	SphereClass* newBall;
	newBall = new SphereClass(glm::vec2(-40, 0), glm::vec2(0, 0), 3.0f, 1,
		glm::vec4(1, 0, 0, 1));
	physicsScene->addActor(newBall);
}void PhysicTutorial::Update(float deltatime){	physicsScene->upDate();}//void PhysicTutorial::onUpdate(float deltaTime)
//{
//
//	//physicsScene->upDateGizmos();
//}