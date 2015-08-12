#include "GLFW\glfw3.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <vector>
#include "glm\vec3.hpp"
class FlyCamera;
using namespace physx;

#pragma region Ragdoll

enum RagDollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM,
};

const PxVec3 X_AXIS = PxVec3(1, 0, 0);
const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
const PxVec3 Z_AXIS = PxVec3(0, 0, 1);

struct RagdollNode
{
	PxQuat globalRotation; //rotation of this link in model space - we could have done
	//this relative to the parent node but it's harder to visualize when setting up the data by
	//hand
	PxVec3 scaledGobalPos; //Position of the link centre in world space which is
	//calculated when we process the node.It's easiest if we store it here so we have it when we
	//transform the child
	int parentNodeIdx; //Index of the parent node
	float halfLength; //half length of the capsule for this node
	float radius; //radius of capsule for thisndoe
	float parentLinkPos; //relative position of link centre in parent to this node. 0 is the centre of hte node, -1 is left end of capsule and 1 is right end of capsule relative to x
	float childLinkPos; //relative position of link centre in child
	char* name; //name of link
	PxArticulationLink* linkPtr;
	//constructor
	RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float
		_radius, float _parentLinkPos, float _childLinkPos, char* _name){
		globalRotation =
			_globalRotation, parentNodeIdx = _parentNodeIdx; halfLength = _halfLength; radius = _radius;
		parentLinkPos = _parentLinkPos; childLinkPos = _childLinkPos; name = _name;
	};
};



#pragma endregion

struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		ePLATFORM = (1 << 1),
		eGROUND = (1 << 2)
	};
};

class MyControllerHitReport;
class MycollisionCallBack;

class Physics
{
public:
	Physics();
	~Physics();

	GLFWwindow* m_pWindow;

	void Update(float deltatime, FlyCamera &a_gameCamera);
	void Draw();
	void setUpPhysX();
	void upDatePhysx(float a_deltaTime);
	void Shutdown();
	void SetUpVisualDebugger();
	void ObjectSpawner();
	void CreateObjects();
	void addWidget(PxShape* pShape, PxRigidActor* actor);
	void addBox(PxShape* pShape, PxRigidActor* actor);
	void addSphere(PxShape* pShape, PxRigidActor* actor);
	void addCapsule(PxShape* pShape, PxRigidActor* actor);
	void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32
		filterMask);
	void setShapeAsTrigger(PxRigidActor* actorIn);
	static PxFilterFlags myFliterShader(PxFilterObjectAttributes attributes0, PxFilterData
		filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
	glm::vec3 Px2GlV3(PxVec3 convert);
	void PlayerController();
	void PlayerUpdate(float a_deltatime);
	float _characterYVelocity, _characterRotation, _playerGravity;
private:  
	std::vector<PxRigidActor*> g_PhysXActors;
	std::vector<PxArticulation*> g_PhysXActorsRagDolls;
	PxFoundation* g_PhysicsFoundation;
	PxPhysics* g_Physics;
	PxScene* g_PhysicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxMaterial* g_PhysicsMaterial;
	PxMaterial* g_boxMaterial;
	PxCooking* g_PhysicsCooker;
	MyControllerHitReport* myHitReport;
	PxControllerManager* gCharacterManager;
	PxController* gPlayerController;
};

class myAllocator : public PxAllocatorCallback
{
public:
	virtual ~myAllocator() {}
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}
	virtual void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}
};

class Ragdoll
{
public:
	Ragdoll();
	~Ragdoll();

	static PxArticulation* Ragdoll::makeRagdoll(PxPhysics* g_Physics, RagdollNode** nodeArray,
		PxTransform worldPos, float scaleFactor, PxMaterial* ragdollMaterial);

};

class MyControllerHitReport : public PxUserControllerHitReport
{
public:
	//overload the onShapeHit function
	virtual void onShapeHit(const PxControllerShapeHit &hit);
	//other collision functions which we must overload //these handle collision with other controllers and hitting obstacles
		virtual void onControllerHit(const PxControllersHit &hit){};
	//Called when current controller hits another controller. More...
	virtual void onObstacleHit(const PxControllerObstacleHit &hit){};
	//Called when current controller hits a user-defined obstacl
	MyControllerHitReport() :PxUserControllerHitReport(){};
	PxVec3 getPlayerContactNormal(){ return _playerContactNormal; };
	void clearPlayerContactNormal(){ _playerContactNormal = PxVec3(0, 0, 0); };
	PxVec3 _playerContactNormal;
};

using namespace std;

class MycollisionCallBack : public PxSimulationEventCallback
{
public:
	MycollisionCallBack();
	virtual void onContact(const PxContactPairHeader& pairHeader, const
	PxContactPair* pairs, PxU32 nbPairs) ;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs);
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32){};
	virtual void onWake(PxActor**, PxU32){};
	virtual void onSleep(PxActor**, PxU32){};

private:
	bool* m_trigger;// Swap this to trigger the triggers SHOTS FIRED OUT A TRIGGER
};



class MycollisionCallBack : public PxSimulationEventCallback
{
	virtual void onContact(const PxContactPairHeader& pairHeader, const
		PxContactPair* pairs, PxU32 nbPairs){};
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs){};
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32){};
	virtual void onWake(PxActor**, PxU32){};
	virtual void onSleep(PxActor**, PxU32){};
};
