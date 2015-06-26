#pragma once 

#include "glm\glm.hpp"
#include <vector>

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
};


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
	glm::vec2 position;
	glm::vec2 velocity;

	float mass;
	float rotation2D;//2D so we only need a single float to represent our rotation

	//Ask matt why these are recongised as member variables instead of Arguments
	DIYRigidBody(glm::vec2 position, glm::vec2 velocity, float rotation, float mass);

	virtual void update(glm::vec2 gravity, float timeStep);
	virtual void debug();

	void applyForce(glm::vec2 force);
	void applyForceToActor(DIYRigidBody* actor2, glm::vec2 force);
};

class PlaneClass :public DIYRigidBody
{
	glm::vec2 normal;
	PlaneClass(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
	glm::vec4 colour);
	float distanceFromOrigin();
	virtual void makeGizmo();
};

class BoxClass : public DIYRigidBody
{
	float length;
	float height;
	BoxClass(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
	glm::vec4 colour);
	virtual void makeGizmo();
};

class SphereClass : public DIYRigidBody
{
public:
	float _radius;
	SphereClass(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
		glm::vec4 colour);
	virtual void makeGizmo();
	//void update();
};

class DIYPhysicScene
{
public:

	glm::vec2 gravity;
	float timeStep;
	std::vector<PhysicsObject*> actors;
	void addActor(PhysicsObject*);
	void removeActor(PhysicsObject*);
	void upDate();
	void debugScene();
	void addGizmos();


};