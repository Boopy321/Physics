#include "PhysicsActor.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Gizmos.h"
#include "GLFW\glfw3.h"
#include <cmath>

#pragma region DIYRigidBody

DIYRigidBody::DIYRigidBody(glm::vec3 position, glm::vec3 velocity, float rotation, float mass)
{

}

void DIYRigidBody::applyForce(glm::vec3 force)
{
	glm::vec3 accel;
	accel = force / mass;
	velocity += accel;
}

void DIYRigidBody::update(glm::vec3 gravity, float timeStep)
{
	//1st law
	position += velocity * timeStep;

	applyForce(gravity);
	//2nd law basically

}

void DIYRigidBody::applyForceToActor(DIYRigidBody* actor2, glm::vec3 force)
{

	applyForce(-force);
	actor2->applyForce(force);
}

void DIYRigidBody::debug()
{

}


#pragma endregion

#pragma region PlaneClass

PlaneClass::PlaneClass(glm::vec3 a_normal , float a_distance) 
{
	normal = a_normal;
	distance = a_distance;
	_shapeID = ShapeType::PLANE;

}
//Incomplete Must complete

void PlaneClass::makeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec3 centrePoint = normal * distance;
	glm::vec3 parallel = glm::vec3(normal.y, -normal.x, normal.z);
	//easy to rotate normal through 90degrees around z
	glm::vec4 colour(0, 0, 0, 1);
	glm::vec3 start = centrePoint + (parallel * lineSegmentLength);
	glm::vec3 end = centrePoint - (parallel * lineSegmentLength);
	Gizmos::addLine(start.xyz(), end.xyz(), colour);
}


#pragma endregion

#pragma region BoxClass

BoxClass::BoxClass(glm::vec3 a_position, glm::vec3 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) : DIYRigidBody(a_position, a_velocity, 0, a_mass)
{
	position = a_position;
	velocity = a_velocity;
	mass = a_mass;
	m_colour = a_colour;
	
	_shapeID = ShapeType::BOX;
}

void BoxClass::makeGizmos()
{
	Gizmos::addAABB(position,)
}
#pragma endregion

#pragma region SphereClass

SphereClass::SphereClass(glm::vec3 a_position, glm::vec3 a_velocity, float a_mass, float a_radius,
	glm::vec4 a_colour) :DIYRigidBody(a_position,a_velocity,0,a_mass)
{
	position = a_position;
	velocity = a_velocity;
	mass = a_mass;
	m_colour = a_colour;
	_radius = a_radius;
	_shapeID = ShapeType::SPHERE;
}

void SphereClass::makeGizmo()
{
	Gizmos::addSphere(glm::vec3(position), _radius, 10, 10, m_colour);
}

#pragma endregion

#pragma region DIYPhysicScene

void DIYPhysicScene::addActor(PhysicsObject* a_object)
{
	actors.push_back(a_object);
}

void DIYPhysicScene::removeActor(PhysicsObject* a_object)
{
	auto target = std::find(actors.begin(), actors.end(), a_object);

	if (target == actors.end())
	{
		actors.erase(target);
	}
}

void DIYPhysicScene::upDate()
{
	addGizmos();
	checkForCollision();
	for(auto actor : actors)
	{
		actor->update(gravity,timeStep);
	}
}

void DIYPhysicScene::addGizmos()
{
	for each(PhysicsObject* actor in actors)
	{
		actor->makeGizmo();
	}
}


typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
//function pointer array for doing our collisions
static fn collisionfunctionArray[] =
{
	DIYPhysicScene::Plane2Plane, DIYPhysicScene::Plane2Sphere,
	DIYPhysicScene::Sphere2Plane, DIYPhysicScene::Sphere2Sphere,
};

void DIYPhysicScene::checkForCollision()
{
	int actorCount = actors.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = actors[outer];
			PhysicsObject* object2 = actors[inner];
			int _shapeID1 = object1->_shapeID;
			int _shapeID2 = object2->_shapeID;
			//using function pointers
			int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2;
			fn collisionFunctionPtr = collisionfunctionArray[functionIndex];
			if (collisionFunctionPtr != NULL)
			{
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool DIYPhysicScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool DIYPhysicScene::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	PlaneClass *plane = dynamic_cast<PlaneClass*>(obj1);
	SphereClass *sphere = dynamic_cast<SphereClass*>(obj2);
	//if we are successful then test for collision
	if (plane != NULL && sphere != NULL)
	{

	}
	return false;
}

bool DIYPhysicScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	SphereClass *sphere1 = dynamic_cast<SphereClass*>(obj1);
	SphereClass *sphere2 = dynamic_cast<SphereClass*>(obj2);
	//if we are successful then test for collision
	if (sphere1 != NULL && sphere2 != NULL)
	{
		//this is where the collision detection happens
		//you need code which sets the velocity of the two spheres to zero
		//if they are overlapping
		glm::vec3 delta = sphere2->position - sphere1->position;
		float Distance = glm::distance(sphere2->position, sphere1->position);
		float intersection = sphere1->_radius + sphere2->_radius - Distance;

		//Work out the force needed to push the value out. Basically the relative velocity keeps swapping each other
		if (Distance < (sphere1->_radius + sphere2->_radius))
		{

			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = sphere1->velocity - sphere2->velocity;
			glm::vec3 collisionVector = collisionNormal * std::abs(glm::dot(relativeVelocity, collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.0f / (1 / sphere1->mass + 1 / sphere2->mass);

			//Newton 3rd law
			sphere1->applyForceToActor(sphere2,  forceVector * 2.0f);

			glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;

			sphere1->position -= seperationVector;
			sphere2->position += seperationVector; 

			return true;
		}
	}
	return false;
}

bool DIYPhysicScene::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	SphereClass *sphere = dynamic_cast<SphereClass*>(obj1);
	PlaneClass *plane = dynamic_cast<PlaneClass*>(obj2);
	//if we are successful then test for collision
	if (sphere != NULL && plane != NULL)
	{
		glm::vec3 collisionNormal = plane->normal;
		float sphereToPlane = glm::dot(sphere->position, plane->normal) - plane->distance;
		// if we are behind plane then we flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->_radius - sphereToPlane;
		if (intersection > 0)
		{
			//Find the point in where the collision occured
			glm::vec3 planeNormal = plane->normal;
			if (sphereToPlane < 0)
			{
				planeNormal *= -1;
			}
			
			glm::vec3 forceVector = -1 * sphere->mass * planeNormal * std::abs(glm::dot(planeNormal, sphere->velocity));
			//sphere->applyForceToActor(sphere, sphere->velocity);
			sphere->applyForce(2.0f * forceVector);
			sphere->position += collisionNormal * intersection * 0.5f;
			return true;

		}


	}
	return false;
}


#pragma endregion



