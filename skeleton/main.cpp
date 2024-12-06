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
Particle* particleA;
Particle* particleB;
std::vector<Proyectile*> projectiles;
ParticleSystem* particleSystem;
SpringForceGenerator* springGenerator = nullptr;
FloatationForceGenerator* floatationForceGenerator = nullptr;

SolidRigid* cube;



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

void CreateCube() {
	Vector3D anchorPosition = Vector3D(0, 0, 0);
	physx::PxBoxGeometry box = PxBoxGeometry(0.5, 0.5, 0.5);
	physx::PxShape* anchorShape = CreateShape(box);
	physx::PxTransform* anchorTransform = new PxTransform(anchorPosition.x, anchorPosition.y, anchorPosition.z);
	RenderItem* anchorRender = new RenderItem(anchorShape, anchorTransform, physx::PxVec4(1, 0, 0, 1));
}

void CreateWaterSurface(float waterLevel) {
	physx::PxBoxGeometry waterBox = PxBoxGeometry(10.0f, 0.1f, 10.0f);
	physx::PxShape* waterShape = CreateShape(waterBox);
	physx::PxTransform* waterTransform = new PxTransform(0.0f, waterLevel, 0.0f);
	RenderItem* waterSurface = new RenderItem(waterShape, waterTransform, physx::PxVec4(0, 0, 1, 0.5f));

	/*float boxHeight = 5.0f;  
	physx::PxBoxGeometry containerBox = PxBoxGeometry(10.0f, boxHeight, 10.0f); 
	physx::PxShape* containerShape = CreateShape(containerBox);
	physx::PxTransform* containerTransform = new PxTransform(0.0f, waterLevel - boxHeight, 0.0f); 
	RenderItem* container = new RenderItem(containerShape, containerTransform, physx::PxVec4(1, 0, 0, 0.1f)); */
}

void CreateFloatingObject(float objectHeight) {
	particleA = new Particle(Vector3(0, objectHeight + 2.0f, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.99, 100.0f, 75.0f);
	particleSystem->addParticle(particleA);

	floatationForceGenerator = new FloatationForceGenerator(1.0f, 1.0f, objectHeight); 
	particleSystem->addForceGenerator(floatationForceGenerator);
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

	physx::PxBoxGeometry boxGeometry(20.0f, 20.0f, 20.0f);
	physx::PxTransform boxTransform(PxVec3(0.0f, 10.0f, 0.0f)); 
	physx::PxVec3 linearVelocity(0.0f, 0.0f, 0.0f); 
	physx::PxVec3 angularVelocity(0.0f, 1.0f, 0.0f); 
	float boxMass = 10.0f; 

	cube = new SolidRigid(gScene,&boxGeometry, boxTransform, linearVelocity, angularVelocity, boxMass, gMaterial);
	//initExex();
	/*particleA = new Particle(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.99, 100, 10);
	particleB = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.99, 100, 10);*/
	/*CreateWaterSurface(0.0f);
	particleSystem = new ParticleSystem();*/
	/*floatationForceGenerator = new FloatationForceGenerator(1.0f, 2.0f, 0.0f);
	particleSystem->addForceGenerator(floatationForceGenerator);*/
	/*particleSystem->addParticle(particleA);  
	particleSystem->addParticle(particleB); */

	//particleSystem->addGenerator(Vector3(0, 0, 0), DistributionType::Gaussian, 100, 300, 10, true);
	//Gravedad
	/*particleSystem->addForceGenerator(new GravitationalForceGenerator(Vector3 (0, -9.8, 0)));*/
	//Viento
	//particleSystem->addForceGenerator(new WindForceGenerator(Vector3(50, 0, 0), 10, 0, Vector3(0, -100, 0), 50));
	//Torbellino
	//particleSystem->addForceGenerator(new TorbellinoForceGenerator(Vector3(50, 0, 0), 10, 0, Vector3(0, 0, 0), 50, 10));
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
// Function to configure what happens in each step of physics
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	// Integrar cada proyectil en la lista
	//for (auto it = projectiles.begin(); it != projectiles.end(); ) {
	//	(*it)->integrate(t);
	//		++it;
	//}

	/*particleSystem->update(t);*/

	gScene->simulate(t);
	gScene->fetchResults(true);
}


// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	///*DeregisterRenderItem(ejex);
	//DeregisterRenderItem(ejey);
	//DeregisterRenderItem(ejez);*/
	//DeregisterRenderItem(origen);
	/*for (auto proj : projectiles) {
		delete proj;
	}
	projectiles.clear();*/
	
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

void shootProyectile(Vector3 camPos, Vector3 camVel, double real_velocity, double simulated_velocity) {
	double mass = 1.0; 
	double Damping = 0.99; 
	projectiles.push_back(new Proyectile(mass, camPos, camVel, Vector3(0, 0, 0), Damping, real_velocity, simulated_velocity));
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'J': 
		CreateCube();
		if (!springGenerator) {
			Vector3 anchor = Vector3(0, 0, 0);  
			float k = 300.0f;                  
			float restLength = 5.0f;            

			springGenerator = new SpringForceGenerator(anchor, k, restLength);

			particleSystem->addForceGenerator(springGenerator);
		}
		break;

	case 'U': 
		if (springGenerator != nullptr) {
			springGenerator->AddKConstant(100);
		}
		break;
	case 'E': 
		if (springGenerator != nullptr) {
			springGenerator->SubKConstant(100);
		}
		/*particleSystem->ActiveExplosion(true);
		particleSystem->addForceGenerator(new ExplosionForceGenerator(Vector3(0, 0, 0), 150.0f, 100000.0f, 1.0f));*/
		break;
	case 'O':
	{
		particleSystem->addForceGenerator(new TemporaryForceGenerator(Vector3(5000,0,0), 1.0f));
		/*double real_velocity = 100.0;
		double simulated_velocity = 50.0;
		shootProyectile(GetCamera()->getTransform().p, GetCamera()->getDir() * simulated_velocity, real_velocity, simulated_velocity);*/
		break;
	}
	case 'G':
		particleSystem->addSpringForceBetweenParticles(particleA, particleB, 50.0f, 5.0f);
		//particleSystem->clearExplosionForceGenerators();
		break;
	case 'F':  
		if (!floatationForceGenerator) {
			CreateFloatingObject(0.0f);
		}
		break;
	case 'L':  
		if (floatationForceGenerator) {
			floatationForceGenerator->setVolume(0.1f);
		}
		break;

	case 'K': 
		if (floatationForceGenerator) {
			floatationForceGenerator->setVolume(-0.1f);
		}
		break;
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