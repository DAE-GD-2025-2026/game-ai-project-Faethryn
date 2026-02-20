// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringAgent.h"
#include <AIController.h>


// Sets default values
ASteeringAgent::ASteeringAgent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASteeringAgent::BeginPlay()
{
	Super::BeginPlay();
	CacheStartingValues();
}

void ASteeringAgent::BeginDestroy()
{
	Super::BeginDestroy();
	ResetCachedValues();
}

void ASteeringAgent::CacheStartingValues()
{
	m_CachedMaxSpeed = GetMaxLinearSpeed();
	m_CachedMaxAngularVelocity = GetMaxAngularSpeed();
}

// Called every frame
void ASteeringAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SteeringBehavior)
	{
		SteeringOutput output = SteeringBehavior->CalculateSteering(DeltaTime, *this);
		AddMovementInput(FVector{output.LinearVelocity, 0.f});
		if (!IsAutoOrienting())
		{
			AAIController* aiController = Cast<AAIController>(GetController());
			if (aiController)
			{
				float currentYaw = GetRotation();
				AddActorLocalRotation(FRotator{ 0,output.AngularVelocity, 0 });
				//aiController->SetControlRotation(FRotator(0, currentYaw + output.AngularVelocity, 0));
			}
		}
		//SetMaxAngularSpeed(output.AngularVelocity);
		//AddControllerYawInput(output.AngularVelocity);
		
	}
}

// Called to bind functionality to input
void ASteeringAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASteeringAgent::SetSteeringBehavior(ISteeringBehavior* NewSteeringBehavior)
{
	SteeringBehavior = NewSteeringBehavior;
	ResetCachedValues();
}

void ASteeringAgent::ResetCachedValues()
{
	SetMaxLinearSpeed(m_CachedMaxSpeed);
	SetMaxAngularSpeed(m_CachedMaxAngularVelocity);
	SetIsAutoOrienting(true);
}

float ASteeringAgent::GetMaxArriveDistance()
{
	return m_MaxTargetArriveDistance;
}

float ASteeringAgent::GetMinArriveDistance()
{
	return m_MinTargetArriveDistance;
}

FColor ASteeringAgent::GetMaxArriveDebugColor()
{
	return m_MaxArriveDebugColor;
}

FColor ASteeringAgent::GetMinArriveDebugColor()
{
	return m_MinArriveDebugColor;
}

FColor ASteeringAgent::GetDirectionLineDebugColor()
{
	return m_MoveDirectionDebugColor;
}

float ASteeringAgent::GetCachedMaxSpeed()
{
	return m_CachedMaxSpeed;
}

float ASteeringAgent::GetCachedMaxAngularVelocity()
{
	return m_CachedMaxAngularVelocity;
}

