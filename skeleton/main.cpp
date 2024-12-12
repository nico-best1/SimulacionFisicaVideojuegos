#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particle.h"
#include "Proyectile.h"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "GravitationalForceGenerator.h"
#include "WindForceGenerator.h"
#include "TorbellinoForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "ForceGenerator.h"
#include "SpringForceGenerator.h"
#include "TemporaryForceGenerator.h"
#include "FloatationForceGenerator.h"
#include "SolidRigid.h"
#include "SolidRigidGenerator.h"
#include "SolidRigidSystem.h"
#include "TorqueForceGenerator.h"
#include "Scene.h"
#include "GameScene.h"

#include "Player.h"

#include <iostream>

std::string display_text = "";


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


enum class GameStates {
	INICIO,
	JUEGO,
	FIN
};

//Items Juego
float timeSinceLastWindGenerator = 0.0f;
float gameTime = 0.0f;
GameStates gamestateActive = GameStates::JUEGO;
GameScene* gameScene;

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
	sceneDesc.gravity = PxVec3(0.0f, -50.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	gamestateActive = GameStates::INICIO;
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
// Function to configure what happens in each step of physics
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	//timeSinceLastWindGenerator += static_cast<float>(t);
	//IncreaseObstacleMovement(timeSinceLastWindGenerator);
	if (gameScene != nullptr && gamestateActive == GameStates::JUEGO) {
		gameScene->update(timeSinceLastWindGenerator, t);
		gameTime += static_cast<float>(t);
	}

	gScene->simulate(t);
	gScene->fetchResults(true);
}


// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	
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

	switch (toupper(key))
	{
	case ' ':
	{
		if (gamestateActive == GameStates::JUEGO && gameScene != nullptr) {
			gameScene->PlayerJump();
		}
		if (gamestateActive == GameStates::INICIO) {
			gamestateActive = GameStates::JUEGO;
			gameScene = new GameScene(gScene, gPhysics, gMaterial);
		}
		if (gamestateActive == GameStates::FIN) {
			gameTime = 0.0f;
			gamestateActive = GameStates::INICIO;
		}
		break;
	}
	default:
		break;
	}
}


void onCollision(physx::PxRigidActor* actor1, physx::PxRigidActor* actor2)
{
	if (gamestateActive == GameStates::JUEGO) {
		if (gameScene->onCollisionGame(actor1, actor2)) {
			gamestateActive == GameStates::FIN;
			delete gameScene;
			gameScene = nullptr;
		}
	}
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