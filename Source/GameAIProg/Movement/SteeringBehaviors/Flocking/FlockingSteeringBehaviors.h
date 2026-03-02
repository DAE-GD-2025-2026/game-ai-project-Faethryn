#pragma once
#include "Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
class Flock;

class Separation : public ISteeringBehavior
{
public:
	virtual ~Separation() override = default;
	Separation(Flock& assignedFlock);

	//Steering
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;

	void SetCohesionDistance(float newDistance);

private:
	float m_CohesionDistance = { 200.0f };

	Flock* pFlock{ nullptr };
};

class Cohesion : public ISteeringBehavior
{
public:
	virtual ~Cohesion() override = default;

	Cohesion(Flock& assignedFlock);

	//Steering
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;

	void SetCohesionDistance(float newDistance);

private:

	float m_CohesionDistance = { 200.0f };
	Flock* pFlock{ nullptr };

};

class VelocityMatch : public ISteeringBehavior
{
public:
	virtual ~VelocityMatch() override = default;
	VelocityMatch(Flock& assignedFlock);

	//Steering
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
	Flock* pFlock{ nullptr };
};