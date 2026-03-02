#include "FlockingSteeringBehaviors.h"
#include "Flock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"


Separation::Separation(Flock& assignedFlock)
{
	pFlock = &assignedFlock;
}

SteeringOutput Separation::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput output{};
	FVector2D averagePosition{};

	averagePosition = pFlock->GetAverageNeighborPos();

	FVector2D directionToMove{};

	directionToMove = Agent.GetPosition() - averagePosition;

	directionToMove.Normalize();

	output.LinearVelocity = directionToMove;

	return output;
}

void Separation::SetCohesionDistance(float newDistance)
{
	m_CohesionDistance = newDistance;
}

Cohesion::Cohesion(Flock& assignedFlock)
{
	pFlock = &assignedFlock;
}

void Cohesion::SetCohesionDistance(float newDistance)
{
	m_CohesionDistance = newDistance;
}

SteeringOutput Cohesion::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput output{};
	FVector2D averagePosition{};

	averagePosition = pFlock->GetAverageNeighborPos();

	FVector2D directionToMove{};

	directionToMove = averagePosition - Agent.GetPosition();

	directionToMove.Normalize();

	output.LinearVelocity = directionToMove;

	return output;
}

VelocityMatch::VelocityMatch(Flock& assignedFlock)
{
	pFlock = &assignedFlock;
}

SteeringOutput VelocityMatch::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput output{};
	FVector2D averageVelocity{};

	averageVelocity = pFlock->GetAverageNeighborVelocity();

	output.LinearVelocity = averageVelocity;

	return output;
}
