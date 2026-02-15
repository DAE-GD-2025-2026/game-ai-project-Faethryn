// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAIProg/Shared/BaseAgent.h"
#include "Steering/SteeringBehaviors.h"
#include "SteeringAgent.generated.h"

/*
 * Simple agent which will run a steering behavior and move according to its output
 *
 * ┌────────────────────┐                                     
 * │                    │                                     
 * │      ABaseAgent    │                                     
 * │                    │                                     
 * └─────────▲──────────┘                                     
 *           │                                                
 *           │inherits                                        
 * ┌─────────┼──────────┐            ┌───────────────────────┐
 * │                    │   uses     │                       │
 * │   ASteeringAgent   ├────────────►   ISteeringBehavior   │
 * │                    │            │                       │
 * └────────────────────┘            └─┬─────────────────────┘
 *                                     │                      
 *                                     │ inherits   ┌────────┐
 *                                     │            │        │
 *                                     ├───────────►│  Seek  │
 *                                     │            │        │
 *                                     │            └────────┘
 *                                     │                      
 *                                     │            ┌────────┐
 *                                     │            │        │
 *                                     ├───────────►│  Flee  │
 *                                     │            │        │
 *                                     │            └────────┘
 *                                     │                      
 *                                     │            ┌────────┐
 *                                     │            │        │
 *                                     └───────────►│  ....  │
 *                                                  │        │
 *                                                  └────────┘
 * 
 */

UCLASS()
class GAMEAIPROG_API ASteeringAgent : public ABaseAgent
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASteeringAgent();

	UPROPERTY(EditAnywhere)
	float m_MaxTargetArriveDistance{ 500.0f };

	UPROPERTY(EditAnywhere)
	float m_MinTargetArriveDistance{ 100.0f };

	UPROPERTY(EditAnywhere)
	FColor m_MinArriveDebugColor{ 255, 0, 0 };

	UPROPERTY(EditAnywhere)
	FColor m_MaxArriveDebugColor{ 0, 255, 0 };

	UPROPERTY(EditAnywhere)
	FColor m_MoveDirectionDebugColor{ 0, 0, 255 };

	float m_CachedMaxSpeed{ 500.0f };

	float m_CachedMaxAngularVelocity{ 1.0f };

	void CacheStartingValues();

	void ResetCachedValues();

protected:

	ISteeringBehavior* SteeringBehavior{nullptr}; // non-owning
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the object is being destroyed
	virtual void BeginDestroy() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SetSteeringBehavior(ISteeringBehavior* NewSteeringBehavior);

	float GetMaxArriveDistance();

	float GetMinArriveDistance();

	FColor GetMaxArriveDebugColor();

	FColor GetMinArriveDebugColor();

	FColor GetDirectionLineDebugColor();

	float GetCachedMaxSpeed();

	float GetCachedMaxAngularVelocity();

};
