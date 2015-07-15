#pragma once 

#include "glm\glm.hpp"
#include <vector>

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
};

//Base
class PhysicsObject
{
public:
	ShapeType _shapeID;
	void virtual update(glm::vec3 gravity, float timeStep) = 0;
	void virtual debug() = 0;
	void virtual makeGizmo() = 0;
	void virtual resetPosition(){};
};


//RigidBody Object
class DIYRigidBody : public PhysicsObject
{
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 m_colour;
	float mass;
	float rotation2D;//2D so we only need a single float to represent our rotation

	//Ask matt why these are recongised as member variables instead of Arguments
	DIYRigidBody(glm::vec3 position, glm::vec3 velocity, float rotation, float mass);

	virtual void update(glm::vec3 gravity, float timeStep);
	virtual void debug();

	void applyForce(glm::vec3 force);
	void applyForceToActor(DIYRigidBody* actor2, glm::vec3 force);

};


class PlaneClass :public PhysicsObject
{
public:
	glm::vec3 normal;
	float distance;
	void virtual update(glm::vec3 gravity, float timeStep){};
	void virtual debug(){};
	void virtual makeGizmo();
	PlaneClass(glm::vec3 normal, float distance);
	PlaneClass();
};


class BoxClass : public DIYRigidBody
{
	float length;
	float height;
	BoxClass(glm::vec3 position, glm::vec3 velocity, float mass, float radius,
	glm::vec4 colour);

	virtual void makeGizmo();
};


class SphereClass : public DIYRigidBody
{
public:
	float _radius;
	SphereClass(glm::vec3 position, glm::vec3 velocity, float mass, float radius,
		glm::vec4 colour);
	virtual void makeGizmo();
	//void update(glm::vec2 gravity, float timeStep);
};


class DIYPhysicScene
{
public:
	glm::vec3 gravity;
	float timeStep;
	std::vector<PhysicsObject*> actors;
	void addActor(PhysicsObject*);
	void removeActor(PhysicsObject*);
	void upDate();
	void debugScene();
	void addGizmos();

	static const int NUMBERSHAPE = 2;

	static bool Plane2Plane(PhysicsObject* , PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);

	void checkForCollision();
};