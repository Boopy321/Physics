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
	physicsScene->gravity = glm::vec3(0, -100.0, 0);
	physicsScene->timeStep = .001f;
	//add four balls to our simulation
	PlaneClass* newLine;
	BoxClass* newBox;
	SphereClass* newBall;


	newBox = new BoxClass(glm::vec3(10, 0,0 ), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false,true);
	physicsScene->addActor(newBox);

	newBox = new BoxClass(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, true);
	physicsScene->addActor(newBox);

	newBox = new BoxClass(glm::vec3(30, 0, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, true);
	physicsScene->addActor(newBox);

	newBox = new BoxClass(glm::vec3(10, 30, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, true);
	physicsScene->addActor(newBox);


	newBall = new SphereClass(glm::vec3(0, 40, 0), glm::vec3(-200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1),false);
	physicsScene->addActor(newBall);


	newBall = new SphereClass(glm::vec3(0, 20, 0), glm::vec3(-2, 100, 0), 40.0f, 1,
		glm::vec4(1, 0, 0, 1),false);
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(0.5, 10, 0), glm::vec3(0, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1),true);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(-5, 0, 0), glm::vec3(0, 20, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1),false);
	physicsScene->addActor(newBall);
	
	newBall = new SphereClass(glm::vec3(5, 20, 0), glm::vec3(200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1),false);
	physicsScene->addActor(newBall);
		
	newBall = new SphereClass(glm::vec3(5, 30, 0), glm::vec3(200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1), false);
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(5, 1, 0), glm::vec3(200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1), false);
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(5, 4, 0), glm::vec3(200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1), false);
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(5, 3, 0), glm::vec3(200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1), false);
	physicsScene->addActor(newBall);

	newBall = new SphereClass(glm::vec3(5, 7, 0), glm::vec3(200, 0, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1), false);
	physicsScene->addActor(newBall);

	newLine = new PlaneClass(glm::vec3(0, 1, 0), -60);
	physicsScene->addActor(newLine);
	//left wall
	newLine = new PlaneClass(glm::vec3(1, 0, 0), -60);
	physicsScene->addActor(newLine);
	//right wall 
	newLine = new PlaneClass(glm::vec3(-1, 0, 0), -60);
	physicsScene->addActor(newLine);
	//Roof
	newLine = new PlaneClass(glm::vec3(0, -1, 0), -60);
	physicsScene->addActor(newLine);

}

void PhysicTutorial::Update(float deltatime)
{

	physicsScene->upDate();

}


void PhysicTutorial::DrawPlanes(DIYPhysicScene *that)
{
	
	//floor
	
}
void PhysicTutorial::DrawAABB(DIYPhysicScene *that)
{



	//newBox = new BoxClass(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.0f, glm::vec3(1, 1, 1), glm::vec4(0, 1, 1, 1), false);
	//that->addActor(newBox);
}

void PhysicTutorial::DrawSpheres(DIYPhysicScene *that)
{


//


}
