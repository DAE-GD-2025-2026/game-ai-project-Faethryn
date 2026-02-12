#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
//*******
// TODO: Do the Week01 assignment :^)
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	

	return Steering;
}

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	Steering.LinearVelocity = Agent.GetPosition() - Target.Position;

	return Steering;
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};
	float distance = FVector2D::Distance(Target.Position, Agent.GetPosition());
	float speed{ 0.0f };

	if (distance > m_MinDistance)
	{
		if (distance > m_MaxDistance)
		{
			speed = Agent.GetMaxLinearSpeed();
		}
		else
		{
			speed = ((distance - m_MinDistance) / (m_MaxDistance - m_MinDistance)) * Agent.GetMaxLinearSpeed();
		}
		Steering.LinearVelocity = (Target.Position - Agent.GetPosition()) * speed;
	}

	Steering.LinearVelocity = FVector2D::Zero();
	return Steering;
}