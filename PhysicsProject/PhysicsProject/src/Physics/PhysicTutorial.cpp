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
	PlaneClass* newLine;
	BoxClass* newBox;
	SphereClass* newBall;

  
	newBox = new BoxClass(glm::vec3(10, 0,0 ), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false,0.10f,false);
	physicsScene->addActor(newBox);
  
	//newBox = new BoxClass(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 0.5f, false);
	//physicsScene->addActor(newBox);
  	//
	//newBox = new BoxClass(glm::vec3(30, 0, 0), glm::vec3(0, 0, 0), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 0.5f, false);
	//physicsScene->addActor(newBox);
  	//
	//newBox = new BoxClass(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1000), 5.0f, glm::vec3(3, 3, 3), glm::vec4(0, 1, 1, 1), false, 0.5f, false);
	//physicsScene->addActor(newBox);
  
  
	newBall = new SphereClass(glm::vec3(15, 45, 0), glm::vec3(0, 50, 0), 40.0f, 3,
		glm::vec4(1, 0, 0, 1), 0.9f,false);
	physicsScene->addActor(newBall);


	SphereClass* ball1;
	SphereClass* ball2;
	float ballRadius = 2;
	float mass = 2;
	ball1 = new SphereClass(glm::vec3(0, 40,0),glm::vec3(0,0,0),40.0f, 3,
		glm::vec4(1, 0, 0, 1),0.9f,true);
	
	
	physicsScene->addActor(ball1);

	int numberBalls = 2;
	for (int i = 1; i <= numberBalls; i++)
	{
		if (i == numberBalls)
		{
			ball2 = new SphereClass(glm::vec3(i * 6.0f, 40, 0), glm::vec3(0, 0, 0), 40.0f, 2,
				glm::vec4(0, 1, 0, 1), 0.9f, true);
		}
		else
		{
			ball2 = new SphereClass(glm::vec3(i * 6.0f, 40, 0), glm::vec3(0, 0, 0), 40.0f, 2,
				glm::vec4(0, 1, 0, 1), 0.9f, false);
		}
	

		physicsScene->addActor(ball2);
		SpringJoint* spring = new SpringJoint(ball1, ball2, 5, .9999f);
		physicsScene->addActor(spring);
		ball1 = ball2;	}	


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
	

	newLine = new PlaneClass(glm::vec3(0, 0, 1), -160);
	physicsScene->addActor(newLine);

	newLine = new PlaneClass(glm::vec3(0, 0, -1), -160);
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
