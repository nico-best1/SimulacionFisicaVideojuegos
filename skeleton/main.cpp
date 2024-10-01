#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particle.h"

#include <iostream>

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//Item list
RenderItem* ejex;
RenderItem* ejey;
RenderItem* ejez;
RenderItem* origen;
Particle* particle;

void initExex() {
	physx::PxSphereGeometry sphere = PxSphereGeometry(1);
	physx::PxShape* shape1 = CreateShape(sphere);
	//Origen
	Vector3D v_origen = Vector3D(0, 0, 0);
	physx::PxTransform* trans_origen = new PxTransform(v_origen.x, v_origen.y, v_origen.z);
	origen = new RenderItem(shape1, trans_origen, physx::PxVec4(1, 1, 1, 1));
	//Eje x
	Vector3D x = Vector3D(10, 0, 0);
	physx::PxTransform* trans_x = new PxTransform(x.x, x.y, x.z);
	ejex = new RenderItem(shape1, trans_x, physx::PxVec4(1, 0, 0, 1));
	//Eje y
	Vector3D y = Vector3D(0, 10, 0);
	physx::PxTransform* trans_y = new PxTransform(y.x, y.y, y.z);
	ejey = new RenderItem(shape1, trans_y, physx::PxVec4(0, 1, 0, 1));
	//Eje z
	Vector3D z = Vector3D(0, 0, 10);
	physx::PxTransform* trans_z = new PxTransform(z.x, z.y, z.z);
	ejez = new RenderItem(shape1, trans_z, physx::PxVec4(0, 0, 1, 1));
}


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	initExex();

	particle = new Particle(Vector3(0,20,0), Vector3(5,0,0), Vector3(5, 0, 0), 0.99);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	particle->integrate(t);

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	DeregisterRenderItem(ejex);
	DeregisterRenderItem(ejey);
	DeregisterRenderItem(ejez);
	DeregisterRenderItem(origen);
	delete particle;
	
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}