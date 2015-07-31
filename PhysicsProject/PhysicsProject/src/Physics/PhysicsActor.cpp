#include "PhysicsActor.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "Gizmos.h"
#include "GLFW\glfw3.h"
#include <cmath>
#include <math.h>
#include <iostream>
#define MIN_LINEAR_THRESHOLD 0.001f
#define MIN_ROTATION_THRESHOLD 0.001f


#pragma region DIYRigidBody

DIYRigidBody::DIYRigidBody(glm::vec3 position, glm::vec3 velocity, float rotation, float mass)
{
	linearDrag = 1;
	angularDrag = 1;
}

void DIYRigidBody::applyForce(glm::vec3 force)
{
	glm::vec3 accel;
	accel = force / mass;
	velocity += accel;
}

void DIYRigidBody::update(glm::vec3 gravity, float timeStep)
{

	 if (!make_static)
	{
		applyForce(gravity);
		position += velocity * timeStep;
		velocity *= drag;
		rotationMatrix = glm::rotate(rotation2D,  glm::vec3(0.0f, 0.0f, 1.0f));
		
		if (glm::length(velocity) < MIN_LINEAR_THRESHOLD)
		{
			velocity = glm::vec3(0, 0, 0);
		}
		
		if (abs(angularVelocity) < MIN_ROTATION_THRESHOLD)
		{
			angularVelocity = 0;
		}
	}

	
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

PlaneClass::PlaneClass(glm::vec3 a_normal, float a_distance)
{
	normal = a_normal;
	distance = a_distance; 
	_shapeID = ShapeType::PLANE;

}
//Incomplete Must complete

void PlaneClass::makeGizmo()
{
	float lineSegmentLength = distance;
	glm::vec3 centrePoint = normal * distance;
	glm::vec3 parallel = glm::vec3(-normal.z, -normal.x, -normal.y);
	//easy to rotate normal through 90degrees around z
	glm::vec4 colour(0, 0, 0, 1);
	glm::vec3 start = centrePoint + (parallel * lineSegmentLength);
	glm::vec3 end = centrePoint - (parallel * lineSegmentLength);

	Gizmos::addLine(start.xyz(), end.xyz(), colour);
	Gizmos::addLine(centrePoint, centrePoint+(normal * 3.0f), glm::vec4(0,1,0,1));
}


#pragma endregion

#pragma region BoxClass

///LWH = Length, Width, height of the box
BoxClass::BoxClass(glm::vec3 a_position, glm::vec3 a_velocity, float a_mass, glm::vec3 LWH, glm::vec4 a_colour, bool a_wireframe, float a_elasticity,bool a_static) : DIYRigidBody(a_position, a_velocity, 0, a_mass)
{
	position = a_position;
	velocity = a_velocity;
	mass = a_mass;
	m_colour = a_colour;

	width = LWH.x;
	height = LWH.y;

	depth = LWH.z;

	wireFrame = a_wireframe;
	_shapeID = ShapeType::BOX;
	make_static = a_static;

	drag = 0.99f;
	elasticity = a_elasticity;
}

void BoxClass::makeGizmo()
{
	if (wireFrame)
	{
		Gizmos::addAABB(position, glm::vec3(width, height, depth), m_colour);
	}
	else
	{
		Gizmos::addAABBFilled(position, glm::vec3(width, height, depth), m_colour);
	}

}
#pragma endregion

#pragma region SphereClass

SphereClass::SphereClass(glm::vec3 a_position, glm::vec3 a_velocity, float a_mass, float a_radius,
	glm::vec4 a_colour, float a_elasticity,bool a_static) :DIYRigidBody(a_position, a_velocity, 0, a_mass)
{
	position = a_position;
	velocity = a_velocity;
	mass = a_mass;
	m_colour = a_colour;
	_radius = a_radius;
	_shapeID = ShapeType::SPHERE;
	make_static = a_static;
	drag = 0.99f;
	elasticity = a_elasticity;
}

void SphereClass::makeGizmo()
{
	Gizmos::addSphere(glm::vec3(position), _radius, 10, 10, m_colour);
}

#pragma endregion

#pragma region SpringJoint
SpringJoint::SpringJoint(DIYRigidBody* connection1, DIYRigidBody* connection2, float
	springCoefficient, float damping) 
{
		_connections[0] = connection1;
		_connections[1] = connection2;
		_springCoefficient = springCoefficient;
		_damping = damping;
		_restLength = glm::length(_connections[0]->position - _connections[1]->position);
		_shapeID = JOINT;

		
		
}

void SpringJoint::makeGizmo()
{
	Gizmos::addLine(_connections[0]->position, _connections[1]->position, glm::vec4(0, 0, 0, 1));
}

void SpringJoint::update(glm::vec3 gravity, float timeStep)
{
	


	
		//force = -Stiffness of spring - Displacement of the spring
}


void SpringJoint::debug()
{

}

//Only actually move if the object is not Static
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
	for (auto actor : actors)
	{
		actor->update(gravity, timeStep);
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
	DIYPhysicScene::Plane2Plane, DIYPhysicScene::Plane2Sphere, DIYPhysicScene::Plane2AABB,
	DIYPhysicScene::Sphere2Plane, DIYPhysicScene::Sphere2Sphere, DIYPhysicScene::Sphere2AABB,
	DIYPhysicScene::AABB2Plane, DIYPhysicScene::AABB2Sphere, DIYPhysicScene::AABB2AABB,

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
			int functionIndex = (_shapeID1 * SHAPE_COUNT) + _shapeID2;
			fn collisionFunctionPtr = collisionfunctionArray[functionIndex];
			if (collisionFunctionPtr != NULL)
			{
				if (_shapeID1 != JOINT && _shapeID2 != JOINT)
				{
					collisionFunctionPtr(object1, object2);
				}
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

bool DIYPhysicScene::Plane2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
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

			float combinedElasticity = (sphere1->elasticity + sphere2->elasticity) / 2.0f;

			//Newton 3rd law
			sphere1->applyForceToActor(sphere2, forceVector + (forceVector * combinedElasticity));

			glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;

			if (!sphere1->make_static)
			{
				sphere1->position -= seperationVector;
			}
			if (!sphere2->make_static)
			{
				sphere2->position += seperationVector;
			}
			

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
		//Collsiion Detection
		float intersection = sphere->_radius - sphereToPlane;
		if (intersection > 0)
		{
			//Find the point in where the collision occured
			glm::vec3 planeNormal = plane->normal;
			if (sphereToPlane > 0)
			{
				planeNormal *= -1;
			}

			glm::vec3 forceVector = -1 * sphere->mass * planeNormal * std::abs(glm::dot(planeNormal, sphere->velocity));
			//sphere->applyForceToActor(sphere, sphere->velocity);
			sphere->applyForce(2.0f * forceVector * sphere->elasticity);
			sphere->position += collisionNormal * intersection * 0.5f;
			return true;

		}


	}
	return false;
}

bool DIYPhysicScene::Sphere2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool DIYPhysicScene::AABB2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	BoxClass *box = dynamic_cast<BoxClass*>(obj1);
	PlaneClass *plane = dynamic_cast<PlaneClass*>(obj2);
	std::vector<float> BoxPoints;
	//if we are successful then test for collision
	if (box != NULL && plane != NULL)
	{
		glm::vec3 collisionNormal = plane->normal;

		float BoxWidth = box->width / 2;
		float BoxHeight = box->height / 2;
		float BoxDepth = box->depth / 2;

		float Collision = 1.0f;

		glm::vec3 points[8] = {
			glm::vec3(box->position.x + BoxWidth, box->position.y + BoxHeight, box->position.z + BoxDepth),
			glm::vec3(box->position.x - BoxWidth, box->position.y - BoxHeight, box->position.z - BoxDepth),
			glm::vec3(box->position.x + BoxWidth, box->position.y - BoxHeight, box->position.z + BoxDepth),
			glm::vec3(box->position.x - BoxWidth, box->position.y - BoxHeight, box->position.z + BoxDepth),
			glm::vec3(box->position.x + BoxWidth, box->position.y - BoxHeight, box->position.z - BoxDepth),
			glm::vec3(box->position.x - BoxWidth, box->position.y + BoxHeight, box->position.z - BoxDepth),
			glm::vec3(box->position.x + BoxWidth, box->position.y + BoxHeight, box->position.z - BoxDepth),
			glm::vec3(box->position.x - BoxWidth, box->position.y + BoxHeight, box->position.z + BoxDepth)
		};

		float dotPoints[8];

		//Dot Product of each point on the box 
		for(int i = 0; i < 8; i++)
		{
			dotPoints[i] = glm::dot(points[i], plane->normal) - plane->distance;
		}
		//Loop through each point of the box to see if any collide
		for (int i = 0; i < 8; i++)
		{
			if (dotPoints[i] < 0.0f)
			{
				//Find the point in where the collision occured
				glm::vec3 planeNormal = plane->normal;

				glm::vec3 forceVector = 1*  box->mass * planeNormal * std::abs(glm::dot(planeNormal, box->velocity));
				//sphere->applyForceToActor(sphere, sphere->velocity);
				box->applyForce(2.0f * (forceVector * box->elasticity));
				//Separation
				box->position += collisionNormal * -dotPoints[i];
				return true;
			}

		}
	}
	return false;
}

bool DIYPhysicScene::AABB2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	BoxClass *box = dynamic_cast<BoxClass*>(obj1);
	SphereClass *sphere = dynamic_cast<SphereClass*>(obj2);
	//if we are successful then test for collision
	if (box != NULL && sphere != NULL)
	{
		//Apply Sat
		float distance_squared = pow(sphere->_radius, 2);

		glm::vec3 delta = sphere->position - box->position;

		float BoxWidth = box->width /2 ;
		float BoxHeight = box->height /2 ;
		float BoxDepth = box->depth /2;


		glm::vec3 boxMax = glm::vec3(box->position.x + BoxWidth, box->position.y + BoxHeight, box->position.z + BoxDepth);
		glm::vec3 boxMin = glm::vec3(box->position.x - BoxWidth, box->position.y - BoxHeight, box->position.z - BoxDepth);
		//X Component
		//Detects wiether there is not a collision. Works in reverse :D
		if (sphere->position.x > boxMax.x)
		{
			distance_squared -= pow(sphere->position.x - boxMax.x, 2);
		}
		else if (sphere->position.x < boxMin.x)
		{
			distance_squared -= pow(sphere->position.x - boxMin.x, 2);
		}
		//Y Component
		if (sphere->position.y > boxMax.y)
		{
			distance_squared -= pow(sphere->position.y-boxMax.y, 2);
		}
		else if (sphere->position.y < boxMin.y)
		{
			distance_squared -= pow(sphere->position.y-boxMin.y, 2);
		}
		//Z Component
		if (sphere->position.z > boxMax.z)
		{
			distance_squared -= pow(sphere->position.z-boxMax.z, 2);
		}
		else if (sphere->position.z < boxMin.z)
		{
			distance_squared -= pow(sphere->position.z-boxMin.z, 2);
		}

		if (distance_squared > 0)
		{
			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = box->velocity - sphere->velocity;
			glm::vec3 collisionVector = collisionNormal * std::abs(glm::dot(relativeVelocity, collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.0f / (1 / box->mass + 1 / sphere->mass);

			float combinedElasticity = (box->elasticity + sphere->elasticity) / 2.0f;
			//Newton 3rd law
			box->applyForceToActor(sphere, forceVector + (forceVector*combinedElasticity));

			glm::vec3 seperationVector = collisionNormal * distance_squared * 0.5f;
			
			if (!box->make_static)
			{
				box->position -= seperationVector;
			}
			if (!sphere->make_static)
			{
				sphere->position += seperationVector;
			}
			
			return true;
		}
	}
	return false;
}

bool DIYPhysicScene::AABB2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	BoxClass *boxMax = dynamic_cast<BoxClass*>(obj1);
	BoxClass *boxMin = dynamic_cast<BoxClass*>(obj2);
	//if we are successful then test for collision
	if (boxMax != NULL && boxMin != NULL)
	{

		glm::vec3 delta = boxMin->position - boxMax->position;

		glm::vec3 boxMaxXYZmax(boxMax->position.x + boxMax->width,
			boxMax->position.y + boxMax->height,
			boxMax->position.z + boxMax->depth);


		glm::vec3 boxMinXYZmax(boxMin->position.x + boxMin->width,
			boxMin->position.y + boxMin->height,
			boxMin->position.z + boxMin->depth);

		glm::vec3 boxMaxoverlap = boxMaxXYZmax - boxMin->position;
		glm::vec3 boxMinoverlap = boxMinXYZmax - boxMax->position;

		//AABB Collision check
		if (//Length checks //x
			boxMax->position.x < boxMinXYZmax.x &&
			boxMax->position.x + boxMax->width > boxMin->position.x &&
			//Width Checks // y
			boxMax->position.y < boxMinXYZmax.y &&
			boxMax->position.y + boxMax->height > boxMin->position.y &&
			//Height Checks //z
			boxMax->position.z < boxMinXYZmax.z &&
			boxMax->position.z + boxMax->depth > boxMin->position.z)
		{

			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = boxMax->velocity - boxMin->velocity;
			glm::vec3 collisionVector = collisionNormal * std::abs(glm::dot(relativeVelocity, collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.0f / (1 / boxMax->mass + 1 / boxMin->mass);
			//Apply Force if successful
			float combinedElasticty = (boxMax->elasticity, boxMin->elasticity) / 2.0f; 

			boxMax->applyForceToActor(boxMin, (forceVector+ (forceVector * combinedElasticty)));
			//Seperate the boxes based of the smallest overlap
			glm::vec3 intersection;
			float min = glm::min(boxMaxoverlap.x, glm::min(boxMaxoverlap.y, boxMaxoverlap.z));

			glm::vec3 seperationVector = collisionNormal * min * 0.5f;

			//Split the mofo's
			if (!boxMax->make_static)
			{
				boxMax->position -= seperationVector;
			}
			
			if (!boxMin->make_static)
			{
				boxMin->position += seperationVector;
			}
			return true;
		}

	}
	return false;
}




//glm::vec3 DIYPhysicScene::SphereToSpherePoint(PhysicsObject* obj1, PhysicsObject* obj2)
//{
//	SphereClass *sphere1 = dynamic_cast<SphereClass*>(obj1);
//	SphereClass *sphere2 = dynamic_cast<SphereClass*>(obj2);
//
//	if (sphere1 != NULL && sphere2 != NULL)
//	{
//		glm::vec3 Point(sphere2->position - sphere1->position);
//		Point = glm::normalize(Point);
//		Point *= sphere1->_radius;
//		Point = Point + sphere1->position;
//		return Point;
//	}
//
//}
//
//float DIYPhysicScene::SphereToPlanePoint(PhysicsObject* obj1, PhysicsObject* obj2)
//{
//	SphereClass *sphere = dynamic_cast<SphereClass*>(obj1);
//	PlaneClass *plane = dynamic_cast<PlaneClass*>(obj2);
//
//	if (sphere != NULL && plane != NULL)
//	{
//
//	}
//}
//
//float DIYPhysicScene::SphereToAABBPoint(PhysicsObject* obj1, PhysicsObject* obj2)
//{
//	SphereClass *sphere = dynamic_cast<SphereClass*>(obj1);
//	BoxClass *box = dynamic_cast<BoxClass*>(obj2);
//
//	if (sphere != NULL && box != NULL)
//	{
//
//	}
//}
#pragma endregion
