#pragma once
#include "WindForceGenerator.h"
class TorbellinoForceGenerator : public WindForceGenerator
{
private:
	float intensity;
public:
	TorbellinoForceGenerator(const Vector3& velocidad, float k1, float k2, Vector3 centro, float radio, float intensity);
	~TorbellinoForceGenerator() {};
	void calculateVelocity(Particle* p) override;
};