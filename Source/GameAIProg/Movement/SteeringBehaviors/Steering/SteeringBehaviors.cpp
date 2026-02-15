#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
//*******
// TODO: Do the Week01 assignment :^)
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();

	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
	return Steering;
}

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();

	Steering.LinearVelocity = Agent.GetPosition() - Target.Position;

	return Steering;
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();

	float distance = FVector2D::Distance(Target.Position, Agent.GetPosition());
	float speed{ 0.0f };

	float debugMaxRadius = Agent.GetMaxArriveDistance();

	float debugMinRadius = Agent.GetMinArriveDistance();

	FVector debugSphereCenter = FVector{ Target.Position.X, Target.Position.Y, Agent.GetActorLocation().Z };

	DrawDebugSphere(Agent.GetWorld(), debugSphereCenter, debugMaxRadius, 12, Agent.GetMaxArriveDebugColor());
	DrawDebugSphere(Agent.GetWorld(), debugSphereCenter, debugMinRadius, 12, Agent.GetMinArriveDebugColor());

	if (distance > Agent.GetMinArriveDistance())
	{
		if (distance >= Agent.GetMaxArriveDistance())
		{
			speed = Agent.GetCachedMaxSpeed();
		}
		else
		{
			speed = ((distance - Agent.GetMinArriveDistance()) / (Agent.GetMaxArriveDistance() - Agent.GetMinArriveDistance())) * Agent.GetCachedMaxSpeed();
		}

		Agent.SetMaxLinearSpeed(speed);

		FVector2D direction = (Target.Position - Agent.GetPosition());
		direction.Normalize();
		Steering.LinearVelocity = direction;

		/*UE_LOG(LogTemp, Warning, TEXT("The speed value is: %f"), speed);
		UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *velocity.ToString());*/

		return Steering;
	}

	Agent.SetMaxLinearSpeed(speed);
	return Steering;
}

SteeringOutput  Face::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();

	Agent.SetMaxLinearSpeed(0.0f);

	Steering.LinearVelocity = Target.Position - Agent.GetPosition();

	return Steering;
}