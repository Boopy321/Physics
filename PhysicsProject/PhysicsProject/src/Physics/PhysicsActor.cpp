#include "PhysicsActor.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#pragma region DIYRigidBody
void DIYRigidBody::applyForce(glm::vec2 force)
{
	//Apply Newton 2nd law f= Mass * Accel
}

void DIYRigidBody::update(glm::vec2 gravity, float timeStep)
{
	position += velocity;
	position += gravity;
}

void DIYRigidBody::applyForceToActor(DIYRigidBody* actor2, glm::vec2 force)
{
	//Apply Newton 3rd law Force applied to both objects 
}
#pragma endregion

#pragma region PlaneClass

#pragma endregion

#pragma region BoxClass

#pragma endregion

#pragma region SphereClass
//void SphereClass::update()
//{
//	DIYRigidBody::update(glm::vec2(0),0);
//}

void SphereClass::makeGizmo()
{
	//Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / AR, 100 / AR, -1.0f, 1.0f));
}
#pragma endregion

#pragma region DIYPhysicScene


#pragma endregion



