#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
//*******
// TODO: Do the Week01 assignment :^)
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	FVector2D targetDirection = Target.Position - Agent.GetPosition();

	targetDirection.Normalize();

	FVector targetDebugLineStart{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z };
	FVector targetDebugLineEnd{ Agent.GetPosition().X + Agent.GetLinearVelocity().X, Agent.GetPosition().Y + Agent.GetLinearVelocity().Y, Agent.GetActorLocation().Z};
	DrawDebugLine(Agent.GetWorld(), targetDebugLineStart, targetDebugLineEnd, Agent.GetDirectionLineDebugColor());


	float distanceThisFrame = Agent.GetMaxLinearSpeed();

	Steering.LinearVelocity = targetDirection * distanceThisFrame;

	return Steering;
}

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	FVector2D targetDirection = Agent.GetPosition() - Target.Position;

	targetDirection.Normalize();

	FVector targetDebugLineStart{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z };
	FVector targetDebugLineEnd{ Agent.GetPosition().X + Agent.GetLinearVelocity().X, Agent.GetPosition().Y + Agent.GetLinearVelocity().Y, Agent.GetActorLocation().Z };
	DrawDebugLine(Agent.GetWorld(), targetDebugLineStart, targetDebugLineEnd, Agent.GetDirectionLineDebugColor());

	float distanceThisFrame = Agent.GetMaxLinearSpeed();

	Steering.LinearVelocity = targetDirection * distanceThisFrame;

	return Steering;
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	float distance = FVector2D::Distance(Target.Position, Agent.GetPosition());
	float speed{ 0.0f };

	float debugMaxRadius = Agent.GetMaxArriveDistance();

	float debugMinRadius = Agent.GetMinArriveDistance();

	FVector debugSphereCenter = FVector{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z};

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

		FVector targetDebugLineStart{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z };
		FVector targetDebugLineEnd{ Agent.GetPosition().X + Agent.GetLinearVelocity().X, Agent.GetPosition().Y + Agent.GetLinearVelocity().Y, Agent.GetActorLocation().Z };
		DrawDebugLine(Agent.GetWorld(), targetDebugLineStart, targetDebugLineEnd, Agent.GetDirectionLineDebugColor());

		float distanceThisFrame = Agent.GetMaxLinearSpeed();

		Steering.LinearVelocity = direction * distanceThisFrame;

		/*UE_LOG(LogTemp, Warning, TEXT("The speed value is: %f"), speed);
		UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *velocity.ToString());*/

		return Steering;
	}

	Agent.SetMaxLinearSpeed(speed);

	Steering.LinearVelocity = FVector2D{}.Zero();
	return Steering;
}

SteeringOutput  Face::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	//Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();

	Agent.SetMaxLinearSpeed(0.0f);

	FVector2D targetDirection = Target.Position - Agent.GetPosition();

	targetDirection.Normalize();

	float targetRotation =  (atan2f(targetDirection.Y, targetDirection.X) * 180) / PI;

	//UE_LOG(LogTemp, Warning, TEXT("The target angle value is: %f"), targetRotation);
	
	float agentRotation = Agent.GetRotation();

	//UE_LOG(LogTemp, Warning, TEXT("The agent angle value is: %f"), agentRotation);


	float rotationDifference = agentRotation - targetRotation;

	Steering.AngularVelocity = rotationDifference / Agent.GetMaxAngularSpeed();

	Steering.LinearVelocity = targetDirection;


	return Steering;
}

SteeringOutput Pursuit::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	//Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();

	float timeToReachTarget = FVector2D{}.Distance(Agent.GetPosition(), Target.Position) / Agent.GetMaxLinearSpeed();

	FVector2D targetPosition = ( Target.LinearVelocity * timeToReachTarget) + Target.Position;

	FVector2D targetDirection = targetPosition - Agent.GetPosition();

	FVector debugSphereCenter = FVector{ targetPosition.X, targetPosition.Y, Agent.GetActorLocation().Z };

	DrawDebugSphere(Agent.GetWorld(), debugSphereCenter, 10.0f, 12, Agent.GetDirectionLineDebugColor());

	targetDirection.Normalize();

	FVector targetDebugLineStart{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z };
	FVector targetDebugLineEnd{ Agent.GetPosition().X + Agent.GetLinearVelocity().X, Agent.GetPosition().Y + Agent.GetLinearVelocity().Y, Agent.GetActorLocation().Z };
	DrawDebugLine(Agent.GetWorld(), targetDebugLineStart, targetDebugLineEnd, Agent.GetDirectionLineDebugColor());

	float distanceThisFrame = Agent.GetMaxLinearSpeed();

	Steering.LinearVelocity = targetDirection * distanceThisFrame;

	return Steering;
}

SteeringOutput Evade::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	//Steering.AngularVelocity = Agent.GetCachedMaxAngularVelocity();
	FVector2D PredictedPosition = (Target.LinearVelocity * DeltaT) + Target.Position;

	float timeToReachTarget = FVector2D{}.Distance(Agent.GetPosition(), PredictedPosition) / Agent.GetMaxLinearSpeed();

	FVector2D targetPosition = (Target.LinearVelocity * timeToReachTarget) + Target.Position;

	FVector2D targetDirection = Agent.GetPosition() - targetPosition;

	FVector debugSphereCenter = FVector{ targetPosition.X, targetPosition.Y, Agent.GetActorLocation().Z };

	DrawDebugSphere(Agent.GetWorld(), debugSphereCenter, 10.0f, 12, Agent.GetDirectionLineDebugColor());

	targetDirection.Normalize();

	FVector targetDebugLineStart{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z };
	FVector targetDebugLineEnd{ Agent.GetPosition().X + Agent.GetLinearVelocity().X, Agent.GetPosition().Y + Agent.GetLinearVelocity().Y, Agent.GetActorLocation().Z };
	DrawDebugLine(Agent.GetWorld(), targetDebugLineStart, targetDebugLineEnd, Agent.GetDirectionLineDebugColor());

	float distanceThisFrame = Agent.GetMaxLinearSpeed();

	Steering.LinearVelocity = targetDirection * distanceThisFrame;

	return Steering;
}

SteeringOutput Wander::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	FVector2D targetDirection = Target.Position - Agent.GetPosition();

	targetDirection.Normalize();

	FVector targetDebugLineStart{ Agent.GetPosition().X, Agent.GetPosition().Y, Agent.GetActorLocation().Z };
	FVector targetDebugLineEnd{ Agent.GetPosition().X + Agent.GetLinearVelocity().X, Agent.GetPosition().Y + Agent.GetLinearVelocity().Y, Agent.GetActorLocation().Z };
	DrawDebugLine(Agent.GetWorld(), targetDebugLineStart, targetDebugLineEnd, Agent.GetDirectionLineDebugColor());


	float distanceThisFrame = Agent.GetMaxLinearSpeed();

	Steering.LinearVelocity = targetDirection * distanceThisFrame;

	return Steering;
}