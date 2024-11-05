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
#include "ExplosionForceGenerator.h"
#include "TornadoForceGenerator.h"
#include "ForceGenerator.h"

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

double simulationTime = 0.0; // Tiempo de simulación en segundos

//Item list
RenderItem* ejex;
RenderItem* ejey;
RenderItem* ejez;
RenderItem* origen;
Particle* particle;
std::vector<Proyectile*> projectiles;
ParticleSystem* particleSystem;
ExplosionForceGenerator* explosionForce;



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
	particleSystem = new ParticleSystem();
	/*GravitationalForceGenerator* gravity = new GravitationalForceGenerator(Vector3(0, -9.8, 0));
	GravitationalForceGenerator* gravity1 = new GravitationalForceGenerator(Vector3(0, 9.8, 0));
	particleSystem->addForceGenerator(gravity);
	particleSystem->addForceGenerator(gravity1);*/

	//Vector3 windVel(10.0, 0.0, 0.0);  // Ejemplo de viento en dirección positiva del eje X
	//float k1 = 0.1f;  // Ajusta el coeficiente de resistencia según sea necesario
	//WindForceGenerator* windForce = new WindForceGenerator(windVel, k1);
	//particleSystem->addForceGenerator(windForce);

	Vector3 tornadoCenter(0, 0, 0);  // Centro del torbellino
	float tornadoIntensity = 10.0f;  // Ajusta la intensidad según sea necesario
	float tornadoRadius = 50.0f;     // Radio de acción del torbellino
	TornadoForceGenerator* tornadoForce = new TornadoForceGenerator(tornadoCenter, tornadoIntensity, tornadoRadius);
	particleSystem->addForceGenerator(tornadoForce);

	/*Vector3 explosionCenter(0, 0, 0);  
	float explosionRadius = 50.0f;
	float explosionIntensity = 1000.0f;
	float explosionTau = 1.0f;
	explosionForce = new ExplosionForceGenerator(explosionCenter, explosionRadius, explosionIntensity, explosionTau);
	particleSystem->addForceGenerator(explosionForce);*/
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
// Function to configure what happens in each step of physics
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	simulationTime += t;

	// Integrar cada proyectil en la lista
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		(*it)->integrate(t);
			++it;
	}

	particleSystem->update(t);

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
	for (auto proj : projectiles) {
		delete proj;
	}
	projectiles.clear();
	
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
	case 'E': 
		explosionForce->trigger(simulationTime);  
		break;
	case 'U':
	{
		/*double real_velocity = 100.0;
		double simulated_velocity = 50.0;
		shootProyectile(GetCamera()->getTransform().p, GetCamera()->getDir() * simulated_velocity, real_velocity, simulated_velocity);*/
		break;
	}
	case 'G':
		particleSystem->addGenerator(Vector3(0, 0, 0), DistributionType::Gaussian, 100, 300, 10);
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