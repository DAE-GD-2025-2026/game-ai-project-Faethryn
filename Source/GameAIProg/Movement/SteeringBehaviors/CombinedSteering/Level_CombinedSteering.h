// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombinedSteeringBehaviors.h"
#include "GameAIProg/Shared/Level_Base.h"
#include "GameAIProg/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"
#include <memory>
#include "Level_CombinedSteering.generated.h"

UCLASS()
class GAMEAIPROG_API ALevel_CombinedSteering : public ALevel_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevel_CombinedSteering();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

private:

	enum class CombinedBehaviourType
	{
		Drunk,
		RangedEvade,
		Count
	};

	struct CombinedSteeringAgent final
	{
		ASteeringAgent* Agent{ nullptr };

		std::unique_ptr<ISteeringBehavior> Behaviour{ nullptr };

		int TargetAgent{-1};

		int SelectedBehavior{ static_cast<int>(CombinedBehaviourType::Drunk) };
	};

	//Datamembers
	bool UseMouseTarget = false;
	bool CanDebugRender = false;

	std::vector<CombinedSteeringAgent> m_CombinedSteeringAgents{};

	void SetAgentBehavior(CombinedSteeringAgent& Agent);

	void UpdateTarget(CombinedSteeringAgent& Agent);

	bool AddAgent(CombinedBehaviourType BehaviorType = CombinedBehaviourType::Drunk);
	void RemoveAgent(unsigned int Index);

};


