#include "Physics.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include "Gizmos.h"
#include "Assets\Camera\FlyCamera.h"
using namespace physx;

#pragma region Ragdoll

RagdollNode* ragdollData[] =
{
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT, 1.0f, 3.0f, 1.0f, 1.0f, "lower spine"),
	new RagdollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE, 1.0f, 1.0f, -1.0f, 1.0f, "left pelvis"),
	new RagdollNode(PxQuat(0, Z_AXIS), LOWER_SPINE, 1.0f, 1.0f, -1.0f, 1.0f, "right pelvis"),
	new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_PELVIS, 5.0f, 2.0f, -1.0f, 1.0f, "L upper leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_PELVIS, 5.0f, 2.0f, -1.0f, 1.0f, "R upper leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_UPPER_LEG, 5.0f, 1.75f, -1.0f, 1.0f, "L lower leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_UPPER_LEG, 5.0f, 1.75f, -1.0f, 1.0f, "R lowerleg"),
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE, 1.0f, 3.0f, 1.0f, -1.0f, "upper spine"),
	new RagdollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE, 1.0f, 1.5f, 1.0f, 1.0f, "left clavicle"),
	new RagdollNode(PxQuat(0, Z_AXIS), UPPER_SPINE, 1.0f, 1.5f, 1.0f, 1.0f, "right clavicle"),
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE, 1.0f, 1.0f, 1.0f, -1.0f, "neck"),
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK, 1.0f, 3.0f, 1.0f, -1.0f, "HEAD"),
	new RagdollNode(PxQuat(PxPi - .3f, Z_AXIS), LEFT_CLAVICLE, 3.0f, 1.5f, -1.0f, 1.0f, "left upper arm"),
	new RagdollNode(PxQuat(0.3f, Z_AXIS), RIGHT_CLAVICLE, 3.0f, 1.5f, -1.0f, 1.0f, "right upper arm"),
	new RagdollNode(PxQuat(PxPi - .3f, Z_AXIS), LEFT_UPPER_ARM, 3.0f, 1.0f, -1.0f, 1.0f, "left lower arm"),
	new RagdollNode(PxQuat(0.3f, Z_AXIS), RIGHT_UPPER_ARM, 3.0f, 1.0f, -1.0f, 1.0f, "right lower arm"),
	NULL
};


PxArticulation* Ragdoll::makeRagdoll(PxPhysics* g_Physics, RagdollNode** nodeArray,
	PxTransform worldPos, float scaleFactor, PxMaterial* ragdollMaterial )
{
	//create the articulation for our ragdoll
	PxArticulation *articulation = g_Physics->createArticulation();
	RagdollNode** currentNode = nodeArray;
	//while there are more nodes to process...
	while (*currentNode != NULL)
	{


		RagdollNode* currentNodePtr = *currentNode;
		//create a pointer ready to hold the parent node pointer if there is one
		RagdollNode* parentNode = nullptr;
		//get scaled values for capsule
		float radius = currentNodePtr->radius*scaleFactor;
		float halfLength = currentNodePtr->halfLength*scaleFactor;
		float childHalfLength = radius + halfLength;
		float parentHalfLength = 0; //will be set later if there is a parent
		//get a pointer to the parent
		PxArticulationLink* parentLinkPtr = NULL;
		currentNodePtr->scaledGobalPos = worldPos.p;

		if (currentNodePtr->parentNodeIdx != -1)
		{
			//if there is a parent then we need to work out our local position for the link
			//get a pointer to the parent node
			parentNode = *(nodeArray + currentNodePtr->parentNodeIdx);
			//get a pointer to the link for the parent
			parentLinkPtr = parentNode->linkPtr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) *scaleFactor;
			//work out the local position of the node
			PxVec3 currentRelative = currentNodePtr->childLinkPos * currentNodePtr->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));
			PxVec3 parentRelative = -currentNodePtr->parentLinkPos * parentNode->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));
			currentNodePtr->scaledGobalPos = parentNode->scaledGobalPos - (parentRelative +
				currentRelative);
		}

		//build the transform for the link
		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGobalPos, currentNodePtr->globalRotation);
		//create the link in the articulation
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);
		//add the pointer to this link into the ragdoll data so we have it for later when we wantto link to it
		currentNodePtr->linkPtr = link;
		float jointSpace = .01f; //gap between joints
		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + .01f;
		PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *ragdollMaterial); //adds a capsule collider to the link
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f);


		if (currentNodePtr->parentNodeIdx != -1)
		{
			//get the pointer to the joint from the link
			PxArticulationJoint *joint = link->getInboundJoint();
			//get the relative rotation of this link
			PxQuat frameRotation = parentNode->globalRotation.getConjugate() *
				currentNodePtr->globalRotation;
			//set the parent contraint frame
			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentLinkPos * parentHalfLength, 0, 0), frameRotation);
			//set the child constraint frame (this the constraint frame of the newly addedlink)
			PxTransform thisConstraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos * childHalfLength, 0, 0));
			//set up the poses for the joint so it is in the correct place
			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);
			//set up some constraints to stop it flopping around
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}
		//get a pointer to the current node
		currentNode++;
	}

	for (int i = 0; i < RIGHT_LOWER_ARM + 1; ++i)
	{
		physx::PxRigidActor* m_articulation = ragdollData[i]->linkPtr;
	
	}

	return articulation;
}


#pragma endregion

Physics::Physics()
{
	setUpPhysX();
	PlayerController();
	SetUpVisualDebugger();
	ObjectSpawner();
	
}

Physics::~Physics()
{
}

#pragma region PhysX

void Physics::setUpPhysX()
{
	PxAllocatorCallback *myCallback = new myAllocator();
	g_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback,
		gDefaultErrorCallback);
	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PhysicsFoundation,
		PxTolerancesScale());
	PxInitExtensions(*g_Physics);
	//create physics material
	g_PhysicsMaterial = g_Physics->createMaterial(0.5f, 0.5f,.5f);
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	g_PhysicsScene = g_Physics->createScene(sceneDesc);
}

void Physics::upDatePhysx(float a_deltaTime)
{
	if (a_deltaTime <= 0)
	{
		return;
	}
	g_PhysicsScene->simulate(a_deltaTime);
	while (g_PhysicsScene->fetchResults() == false)
	{
		// don’t need to do anything here yet but we have to fetch results
	}
}

void Physics::Shutdown()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
}

void Physics::SetUpVisualDebugger()
{
	// check if PvdConnection manager is available on this platform
	if (g_Physics->getPvdConnectionManager() == NULL)
		return;
	// setup connection parameters
	const char* pvd_host_ip = "127.0.0.1";
	// IP of the PC which is running PVD
	int port = 5425;
	// TCP port to connect to, where PVD is listening
	unsigned int timeout = 100;
	// timeout in milliseconds to wait for PVD to respond,
	//consoles and remote PCs need a higher timeout.
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	// and now try to connectPxVisualDebuggerExt
	auto theConnection = PxVisualDebuggerExt::createConnection(
		g_Physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void Physics::ObjectSpawner()

{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi*1.0f,
	PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(),
		*g_PhysicsMaterial);
	//add it to the physX scene
	g_PhysicsScene->addActor(*plane);
	g_PhysXActors.push_back(plane);
	//add a box
	float density = 10;
	PxBoxGeometry box(2, 2, 2);
	PxBoxGeometry box1(2, 4, 2);

	PxSphereGeometry sphere(1);

	PxTransform transform(PxVec3(0, 5, 0));

		PxRigidDynamic* dynamicActor = PxCreateDynamic(*g_Physics, transform, box1,
		*g_PhysicsMaterial, density);

	//add it to the physX scene
	g_PhysicsScene->addActor(*dynamicActor);
	g_PhysXActors.push_back(dynamicActor);

	 dynamicActor = PxCreateDynamic(*g_Physics, transform, box,
		*g_PhysicsMaterial, density);
	//add it to the physX scene
	g_PhysicsScene->addActor(*dynamicActor);
	g_PhysXActors.push_back(dynamicActor);

	dynamicActor = PxCreateDynamic(*g_Physics, transform, sphere ,
		*g_PhysicsMaterial, density);

	g_PhysicsScene->addActor(*dynamicActor);
	g_PhysXActors.push_back(dynamicActor);

	PxArticulation* ragDollArticulation;
	ragDollArticulation = Ragdoll::makeRagdoll(g_Physics,
		ragdollData, PxTransform(PxVec3(0, 0, 0)), .1f, g_PhysicsMaterial);
	g_PhysicsScene->addArticulation(*ragDollArticulation);
	g_PhysXActorsRagDolls.push_back(ragDollArticulation);

	PxCapsuleGeometry capsule(1, 1);
	dynamicActor = PxCreateDynamic(*g_Physics, PxTransform(PxVec3(0, 300, 0)), capsule,
		*g_PhysicsMaterial, density);

	g_PhysicsScene->addActor(*dynamicActor);
	g_PhysXActors.push_back(dynamicActor);




}

void Physics::PlayerController()
{
	myHitReport = new MyControllerHitReport();
	gCharacterManager = PxCreateControllerManager(*g_PhysicsScene);
	//describe our controller...
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = g_PhysicsMaterial;
	desc.reportCallback = myHitReport; //connect it to our collision detection routine
	desc.density = 10;
	//create the layer controller
	gPlayerController = gCharacterManager->createController(desc);
	gPlayerController->setPosition(PxExtendedVec3(0,1,0));
	//set up some variables to control our player with
	_characterYVelocity = 0; //initialize character velocity
	_characterRotation = 0; //and rotation
	_playerGravity = -0.5f; //set up the player gravity
	myHitReport->clearPlayerContactNormal(); //initialize the contact normal (what weare in contact with)
	
		g_PhysicsScene->addActor(*gPlayerController->getActor());
		g_PhysXActors.push_back(gPlayerController->getActor());

}


void Physics::PlayerUpdate(float a_deltatime)
{
	bool onGround; //set to true if we are on the ground
	float movementSpeed = 20.0f; //forward and back movement speed
	float rotationSpeed = 1.0f; //turn speed
	//check if we have a contact normal. if y is greater than .3 we assume this is
	//solid ground.This is a rather primitive way to do this.Can you do better ?
	if (myHitReport->getPlayerContactNormal().y > 0.3f)
	{
		_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		_characterYVelocity += _playerGravity * a_deltatime;
		onGround = false;
	}
	myHitReport->clearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);
	//scan the keys and set up our intended velocity based on a global transform
	PxVec3 velocity(0, _characterYVelocity, 0);
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
	{
		velocity.z -= movementSpeed * a_deltatime;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		velocity.z += movementSpeed* a_deltatime;
	}
	//To do.. add code to control z movement and jumping
	float minDistance = 0.001f;
	PxControllerFilters filter;
	//make controls relative to player facing
	PxQuat rotation(_characterRotation, PxVec3(0, 1, 0));
	//velocity = PxVec3(0, _characterYVelocity, 0);
	//move the controller
	gPlayerController->move(rotation.rotate(velocity), minDistance, a_deltatime,
		filter);

}
#pragma endregion


#pragma region Draw/Update
void Physics::Update(float deltatime, FlyCamera &a_gameCamera)
{
	upDatePhysx(deltatime);
	
	for (auto actor : g_PhysXActors)
	{
		{
			PxU32 nShapes = actor->getNbShapes();
			PxShape** shapes = new PxShape*[nShapes];
			actor->getShapes(shapes, nShapes);
			// Render all the shapes in the physx actor (for early tutorials there is just one)
				while (nShapes--)
				{
					addWidget(shapes[nShapes], actor);
				}
			delete[] shapes;
		}
	}

	for (auto articulation : g_PhysXActorsRagDolls)
	{
		PxU32 nLinks = articulation->getNbLinks();
		PxArticulationLink** links = new PxArticulationLink*[nLinks];
		articulation->getLinks(links, nLinks);

		while (nLinks--)
		{
			PxArticulationLink* link = links[nLinks];
			PxU32 nShapes = link->getNbShapes();
			PxShape** shapes = new PxShape*[nShapes];
			link->getShapes(shapes, nShapes);
			while (nShapes--)
			{
				addWidget(shapes[nShapes], link);
			}
		}
		delete[] links;

	}

	PlayerUpdate(deltatime);

	m_pWindow = glfwGetCurrentContext();
	//Pew pew Sphere Creater
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		
		PxSphereGeometry sphere(2);
		
		glm::vec3 startingPosition(a_gameCamera.getWorldTransform()[3]);

		glm::vec3 direction(a_gameCamera.getWorldTransform()[2]);

		float density = 10;
		float muzzleSpeed = -50;

		PxTransform transform(PxVec3(startingPosition.x, startingPosition.y, startingPosition.z));

		PxRigidDynamic* dynamicActor = PxCreateDynamic(*g_Physics, transform, sphere,
			*g_PhysicsMaterial, density);

		physx::PxVec3 velocity = physx::PxVec3(direction.x, direction.y, direction.z)* muzzleSpeed;
		dynamicActor->setLinearVelocity(velocity, true);


		//add it to the physX scene
		g_PhysicsScene->addActor(*dynamicActor);
		g_PhysXActors.push_back(dynamicActor);

	}

}

void Physics::Draw()
{
	//Useless at the moment
}

void Physics::addWidget(PxShape* shape, PxRigidActor* actor)
{
	PxGeometryType::Enum type = shape->getGeometryType();
	switch (type)
	{
	case PxGeometryType::eBOX:
		addBox(shape, actor);
		break;
	case  PxGeometryType::eSPHERE:
		addSphere(shape, actor);
		break;
	case  PxGeometryType::eCAPSULE:
		addCapsule(shape, actor);
		break;
	
	}
}

void Physics::addBox(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1, height = 1, length = 1;
	bool status = pShape->getBoxGeometry(geometry);
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;
	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(1, 0, 0, 1);
	if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
	{
		colour = glm::vec4(0, 1, 0, 1);
	}
		//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void Physics::addSphere(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxSphereGeometry geometry;
	float radius = 1;
	int cols = 10;
	int rows = 10;
	//get the geometry for this PhysX collision volume
	bool status = pShape->getSphereGeometry(geometry);
	if (status)
	{
		radius = geometry.radius;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec4 colour = glm::vec4(1, 0, 0, 1);
	if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) 
	{
		colour = glm::vec4(0, 1, 0, 1);
	}
	//create our Sphere gizmo
	if (status)
	{
		Gizmos::addSphere(position, radius, rows, cols, colour, &M);
	}
}

void Physics::addCapsule(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxCapsuleGeometry geometry;
	float radius = 1;

	float halfHeight;

	//get the geometry for this PhysX collision volume
	bool status = pShape->getCapsuleGeometry(geometry);
	if (status)
	{
		radius = geometry.radius; //copy out capsule radius
		halfHeight = geometry.halfHeight; //copy out capsule half length
	}
	PxTransform transform = PxShapeExt::getGlobalPose(*pShape, *actor);
	//get the transform for this PhysX collision volume
	PxMat44 m(transform); //Create a rotation matrix from transform
	glm::mat4* M = (glm::mat4*)(&m);
	//get the world position from the PhysX tranform
	glm::vec3 position = Px2GlV3(transform.p);
	glm::vec4 axis(halfHeight, 0, 0, 0); //axis for the capsule
	axis = *M * axis; //rotate axis to correct orientation
	glm::vec4 colour = glm::vec4(0, 0, 0, 1);
	//add our 2 end cap spheres...
	Gizmos::addSphere(position + axis.xyz(), radius, 10, 10, colour);
	Gizmos::addSphere(position - axis.xyz(), radius, 10, 10, colour);
	//Fix the gizmo rotation
	glm::mat4 m2 = glm::rotate(*M, 11 / 7.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	Gizmos::addCylinderFilled(position, radius, halfHeight, 10, colour, &m2);

}

glm::vec3 Physics::Px2GlV3(PxVec3 convert)
{
	glm::vec3 newvector;
	newvector.x = convert.x;
	newvector.y = convert.y;
	newvector.z = convert.z;

	return newvector;
}
#pragma endregion

#pragma region HitControllerReport

void MyControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	//gets a reference to a structure which tells us what has been hit and where
	//get the acter from the shape we hit
	PxRigidActor* actor = hit.shape->getActor();
	//get the normal of the thing we hit and store it so the player controller can
	//respond correctly
		_playerContactNormal = hit.worldNormal;
	//try to cast to a dynamic actor
	PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();
	if (myActor)
	{
		//this is where we can apply forces to things we hit
	}
}
#pragma endregion
