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
	//pFlock->RegisterNeighbors(&Agent);

	FVector2D directionToMove{};

	for (int i{ 0 }; i < pFlock->GetNrOfNeighbors(); i++)
	{
		float distance = FVector2D{}.Distance(Agent.GetPosition(), pFlock->GetNeighbors()[i]->GetPosition());

		float weight = m_CohesionDistance / distance;

		FVector2D tempVector = (Agent.GetPosition() - pFlock->GetNeighbors()[i]->GetPosition());

		directionToMove += (tempVector * weight);
	}

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
	//pFlock->RegisterNeighbors(&Agent);


	averagePosition = pFlock->GetAverageNeighborPos();

	FVector2D directionToMove{};

	directionToMove = averagePosition - Agent.GetPosition();

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
	//pFlock->RegisterNeighbors(&Agent);
	averageVelocity = pFlock->GetAverageNeighborVelocity();

	output.LinearVelocity = averageVelocity;

	return output;
}
