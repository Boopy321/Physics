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

private: 
	DIYPhysicScene* physicsScene;
};

