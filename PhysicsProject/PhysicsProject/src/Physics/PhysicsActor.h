#pragma once 

#include "glm\glm.hpp"
#include <vector>

enum ShapeType
{
	PLANE,
	SPHERE,
	BOX,
	SHAPE_COUNT,
	JOINT,
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
	glm::vec4 colour;

	float rotateInertia;
	float mass;
	float elasticity;
	float linearDrag, angularDrag, angularVelocity, drag;
	
	glm::mat4 rotationMatrix;
	bool make_static;
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
	
public:
	float width;
	float height;
	float depth;

	bool wireFrame;
	BoxClass(glm::vec3 position, glm::vec3 velocity, float mass, glm::vec3 LWH,
		glm::vec4 colour, bool a_wireframe, float a_elasticity,bool a_static);

	virtual void makeGizmo();
};


class SphereClass : public DIYRigidBody
{
public:
	float _radius;
	SphereClass(glm::vec3 position, glm::vec3 velocity, float mass, float radius,
		glm::vec4 colour, float a_elasticity, bool a_static);
	virtual void makeGizmo();
};

class SpringJoint :public PhysicsObject
{
public:
	SpringJoint(DIYRigidBody* connection1, DIYRigidBody* connection2,
		float springCoefficient, float damping);
private:
	void virtual update(glm::vec3 gravity, float timeStep);
	void virtual debug();
	void virtual makeGizmo();
	DIYRigidBody* _connections[2];
	float _damping;
	float _restLength;
	float _springCoefficient;
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

	static bool Plane2Plane(PhysicsObject* , PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Plane2AABB(PhysicsObject*, PhysicsObject*);


	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2AABB(PhysicsObject*, PhysicsObject*);

	static bool AABB2Sphere(PhysicsObject*, PhysicsObject*);
	static bool AABB2Plane(PhysicsObject*, PhysicsObject*);
	static bool AABB2AABB(PhysicsObject*, PhysicsObject*);

	
	void checkForCollision();
};

