#pragma once

class DIYPhysicScene;


class PhysicTutorial
{
public:
	PhysicTutorial();
	~PhysicTutorial();

	void DIYPhysicsSetup1();
	void onUpdate(float deltatime);
	void Update(float deltatime);

	void DrawSpheres(DIYPhysicScene *that);
	void DrawAABB(DIYPhysicScene *that);
	void DrawPlanes(DIYPhysicScene *that);

private: 
	DIYPhysicScene* physicsScene;
};

