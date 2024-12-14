#pragma once
#include "Player.h"
#include "Scene.h"
#include "ParticleSystem.h"
#include "SolidRigidSystem.h"
#include "WindForceGenerator.h"
#include "GravitationalForceGenerator.h"
#include "SpringForceGenerator.h"
#include <iostream>
class GameScene
{
private:
	std::vector<RenderItem*> renderItems;
	physx::PxScene* scene;
	physx::PxPhysics* physics;
	physx::PxMaterial* material;
	PxRigidStatic* techo;
	PxRigidStatic* suelo;
	Player* player;
	ParticleSystem* particleSystem_player;
	ParticleSystem* particleSystem_stars1;
	ParticleSystem* particleSystem_stars2;
	ParticleSystem* particleSystem_stars3;
	ParticleSystem* particleSystem_stars4;
	ParticleSystem* particleSystem_stars5;
	ParticleSystem* particleSystem_stars6;
	SolidRigidSystem* solidSystem_obstacles;

public:
	GameScene(physx::PxScene* scene_, physx::PxPhysics* physics_, physx::PxMaterial* material_) :
		scene(scene_), physics(physics_), material(material_) {
		CreateSueloTecho();
		CreatePlayer();
		CreateObstacles();
	}

	~GameScene() {
		if (techo) {
			scene->removeActor(*techo);
			techo->release();
			techo = nullptr;
		}
		if (suelo) {
			scene->removeActor(*suelo);
			suelo->release();
			suelo = nullptr;
		}

		for (auto item : renderItems) {
			delete item;
		}
		renderItems.clear();

	    delete player;
		player = nullptr;

		delete particleSystem_player;
		particleSystem_player = nullptr;

		delete particleSystem_stars1;
		particleSystem_stars1 = nullptr;

		delete particleSystem_stars2;
		particleSystem_stars2 = nullptr;

		delete particleSystem_stars3;
		particleSystem_stars3 = nullptr;

		delete particleSystem_stars4;
		particleSystem_stars3 = nullptr;

		delete particleSystem_stars5;
		particleSystem_stars3 = nullptr;

		delete particleSystem_stars6;
		particleSystem_stars3 = nullptr;

		delete solidSystem_obstacles;
		solidSystem_obstacles = nullptr;
	}



	void CreateSueloTecho() {
		PxMaterial* noFrictionMaterial = physics->createMaterial(0.0f, 0.0f, 0.0f);

		// Techo
		techo = physics->createRigidStatic(PxTransform{ 0, 120, 0 });
		PxShape* shape = CreateShape(PxBoxGeometry(1000, 10, 10), noFrictionMaterial);
		techo->attachShape(*shape);
		scene->addActor(*techo);
		RenderItem* item = new RenderItem(shape, techo, { 0, 0, 1, 1 });
		renderItems.push_back(item);

		// Suelo
		suelo = physics->createRigidStatic(PxTransform{ 0, -50, 0 });
		PxShape* shape1 = CreateShape(PxBoxGeometry(1000, 10, 10), noFrictionMaterial);
		suelo->attachShape(*shape1);
		scene->addActor(*suelo);
		RenderItem* item1 = new RenderItem(shape1, suelo, { 0, 0, 1, 1 });
		renderItems.push_back(item1);

		particleSystem_stars1 = new ParticleSystem();

		auto starsPositionCallback = []() -> Vector3 {
			static std::default_random_engine generator;
			std::uniform_real_distribution<float> distributionX(-1000.0f, 1000.0f); 
			std::uniform_real_distribution<float> distributionY(-500.0f, 300.0f);     
			std::uniform_real_distribution<float> distributionZ(-500.0f, -200.0f);   
			return Vector3(distributionX(generator), distributionY(generator), distributionZ(generator));
			};

		// Generador de estrellas 1
		int dispersion_area_x = 1000; 
		int dispersion_area_y = 500;  
		double particleLifetime = 10000.0; 
		particleSystem_stars1->addGenerator(
			starsPositionCallback,
			DistributionType::Uniform,
			dispersion_area_x,
			dispersion_area_y,
			particleLifetime,
			false,   
			false,
			true,    
			60       
		);

		particleSystem_stars1->addForceGenerator(new SpringForceGenerator(Vector3(0.0f, -50.0f, -200.0f), 0.2f, 1.0f));


		// Generador de estrellas 2
		particleSystem_stars2 = new ParticleSystem();

		particleSystem_stars2->addGenerator(
			starsPositionCallback,
			DistributionType::Uniform,
			dispersion_area_x,
			dispersion_area_y,
			particleLifetime,
			false,
			false,
			true,
			60
		);

		particleSystem_stars2->addForceGenerator(new SpringForceGenerator(Vector3(300.0f, 100.0f, -200.0f), 0.4f, 2.5f));


		// Generador de estrellas 3
		particleSystem_stars3 = new ParticleSystem();


		particleSystem_stars3->addGenerator(
			starsPositionCallback,
			DistributionType::Uniform,
			dispersion_area_x,
			dispersion_area_y,
			particleLifetime,
			false,
			false,
			true,
			60
		);

		particleSystem_stars3->addForceGenerator(new SpringForceGenerator(Vector3(-300.0f, 50.0f, -200.0f), 0.6f, 5.0f));


		// Generador de estrellas 4
		particleSystem_stars4 = new ParticleSystem();


		particleSystem_stars4->addGenerator(
			starsPositionCallback,
			DistributionType::Uniform,
			dispersion_area_x,
			dispersion_area_y,
			particleLifetime,
			false,
			false,
			true,
			60
		);

		particleSystem_stars4->addForceGenerator(new SpringForceGenerator(Vector3(0.0f, 250.0f, -200.0f), 0.1f, 3.0f));

		// Generador de estrellas 5
		particleSystem_stars5 = new ParticleSystem();


		particleSystem_stars5->addGenerator(
			starsPositionCallback,
			DistributionType::Uniform,
			dispersion_area_x,
			dispersion_area_y,
			particleLifetime,
			false,
			false,
			true,
			60
		);

		particleSystem_stars5->addForceGenerator(new SpringForceGenerator(Vector3(-330.0f, -250.0f, -200.0f), 0.8f, 4.0f));

		// Generador de estrellas 6
		particleSystem_stars6 = new ParticleSystem();


		particleSystem_stars6->addGenerator(
			starsPositionCallback,
			DistributionType::Uniform,
			dispersion_area_x,
			dispersion_area_y,
			particleLifetime,
			false,
			false,
			true,
			60
		);

		particleSystem_stars6->addForceGenerator(new SpringForceGenerator(Vector3(300.0f, -250.0f, -200.0f), 0.6f, 5.0f));
	}


	void CreatePlayer() {
		PxMaterial* noFrictionMaterial = physics->createMaterial(0.0f, 0.0f, 0.0f);
		particleSystem_player = new ParticleSystem();
		particleSystem_player->addForceGenerator(new GravitationalForceGenerator(Vector3(0, -50, 0)));
		PxGeometry* playerGeometry = new PxBoxGeometry(5, 5, 5);
		PxTransform playerTransform{ PxVec3(-75, 50, 0) };
		PxVec3 playerLinVel{ 0, 0, 0 };
		PxVec3 playerAngVel{ 0, 0, 0 };
		float playerMass = 1.0f;
		player = new Player(scene, playerGeometry, playerTransform, playerLinVel, playerAngVel, playerMass, noFrictionMaterial);
		player->setSolidInScene();
		PxRigidDynamic* playerRigidBody = static_cast<PxRigidDynamic*>(player->getSolid());
		playerRigidBody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
		playerRigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	}

	void CreateObstacles() {
		solidSystem_obstacles = new SolidRigidSystem(scene, physics, material, 10000, 5.0f);
		PxBoxGeometry* cubeGeometry = new PxBoxGeometry(10.0f, 10.0f, 10.0f);
		SolidRigidGenerator* ObstacleGenerator = new SolidRigidGenerator(
			cubeGeometry,
			10000,
			10.0f, 20.0f,
			1.0f, 5.0f,
			0.0f, 2.0f,
			200.0f, 200.0f,
			-15.0f, 100.0f,
			0.0f, 0.0f,
			0.2f, 0.6f,
			0.4f, 0.9f
		);
		solidSystem_obstacles->addGenerator(ObstacleGenerator);
		Vector3 windVelocity(-50.0f, 0.0f, 0.0f);
		float k1 = 0.3f;
		float k2 = 0.7f;
		Vector3 center(0.0f, 0.0f, 0.0f);
		float radius = 1000.0f;
		solidSystem_obstacles->addForceGenerator(new WindForceGenerator(windVelocity, k1, k2, center, radius));
	}

	void IncreaseObstacleMovement(float& timeSinceLastWindGenerator_) {
		if (timeSinceLastWindGenerator_ >= 3.0f) {
			Vector3 newWindVelocity(-50.0f, 0.0f, 0.0f);
			float newK1 = 0.3f;
			float newK2 = 0.7f;
			Vector3 newCenter(0.0f, 0.0f, 0.0f);
			float newRadius = 1000.0f;

			solidSystem_obstacles->addForceGenerator(new WindForceGenerator(newWindVelocity, newK1, newK2, newCenter, newRadius));
			timeSinceLastWindGenerator_ = 0.0f;
		}
	}

	void PlayerJump() {
		player->jump(Vector3(0, 35, 0), 0.05);


		auto playerPositionCallback = [&]() -> Vector3 {
			return player->getPosition();
			};

		DistributionType distributionType = DistributionType::Uniform;
		int dispersion_area_x = 100;
		int dispersion_area_y = 100;
		double particleLifetime = 1.0f;

		particleSystem_player->addGenerator(playerPositionCallback, distributionType, dispersion_area_x, 
			dispersion_area_y, particleLifetime, false, true, false, 20);
	}

	bool onCollisionGame(physx::PxRigidActor* actor1, physx::PxRigidActor* actor2) {
		if (player->getSolid() == actor1 || player->getSolid() == actor2) {
			PxRigidActor* otherActor = (player->getSolid() == actor1) ? actor2 : actor1;

			if (otherActor->getType() != PxActorType::eRIGID_STATIC) {
				return true;
			}
		}
		return false;
	}

	void update(float& timeSinceLastWindGenerator, double t) {
		particleSystem_stars1->update(t);
		particleSystem_stars2->update(t);
		particleSystem_stars3->update(t);
		particleSystem_stars4->update(t);
		particleSystem_stars5->update(t);
		particleSystem_stars6->update(t);
		particleSystem_player->update(t);
		solidSystem_obstacles->update(t);
		IncreaseObstacleMovement(timeSinceLastWindGenerator);
	}
};

