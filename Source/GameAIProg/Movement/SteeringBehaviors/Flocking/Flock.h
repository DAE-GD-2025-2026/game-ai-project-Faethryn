#pragma once

// Toggle this define to enable/disable spatial partitioning
 #define GAMEAI_USE_SPACE_PARTITIONING

#include "FlockingSteeringBehaviors.h"
#include "Movement/SteeringBehaviors/SteeringAgent.h"
#include "Movement/SteeringBehaviors/SteeringHelpers.h"
#include "Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include <memory>
#include "imgui.h"
#ifdef GAMEAI_USE_SPACE_PARTITIONING
#include "../SpacePartitioning/SpacePartitioning.h"
#endif

class Flock final
{
public:
	Flock(
	UWorld* pWorld,
	TSubclassOf<ASteeringAgent> AgentClass,
	int FlockSize = 10, 
	float WorldSize = 100.f, 
	ASteeringAgent* const pAgentToEvade = nullptr, 
	bool bTrimWorld = false);

	~Flock();

	void Tick(float DeltaTime);
	void RenderDebug();
	void ImGuiRender(ImVec2 const& WindowPos, ImVec2 const& WindowSize);

#ifdef GAMEAI_USE_SPACE_PARTITIONING
	const std::vector<ASteeringAgent*>&  GetNeighbors() const { return pPartitionedSpace->GetNeighbors(); }
	int GetNrOfNeighbors() const { return pPartitionedSpace->GetNrOfNeighbors(); }
	void RegisterNeighbors(ASteeringAgent* pAgent, float radius);
#else // No space partitioning
	void RegisterNeighbors(ASteeringAgent* const Agent);
	int GetNrOfNeighbors() const { return NrOfNeighbors; }
	const std::vector<ASteeringAgent*>& GetNeighbors() const { return Neighbors; }
#endif // USE_SPACE_PARTITIONING

	FVector2D GetAverageNeighborPos() const;
	FVector2D GetAverageNeighborVelocity() const;

	void SetTarget_Seek(FSteeringParams const & Target);

private:
	// For debug rendering purposes
	UWorld* pWorld{nullptr};
	
	int FlockSize{20};
	std::vector<ASteeringAgent*> Agents{};
#ifdef GAMEAI_USE_SPACE_PARTITIONING
	std::unique_ptr<CellSpace> pPartitionedSpace{};
	int NrOfCellsX{ 40 };
	std::vector<FVector2D> OldPositions{};
	

	const float m_cellWidth{ 100.f };
	const float m_cellHeight{ 100.f };
#else // No space partitioning
#endif // USE_SPACE_PARTITIONING
	std::vector<ASteeringAgent*> Neighbors{};
	
	float NeighborhoodRadius{500.f};
	int NrOfNeighbors{0};

	ASteeringAgent* pAgentToEvade{nullptr};
	
	//Steering Behaviors

	BlendedSteering* pBoidBehaviour{ nullptr };
	PrioritySteering* pEvadeBoidBehavior{ nullptr };
	Separation* pSeparationBehavior{ nullptr };
	Cohesion* pCohesionBehavior{ nullptr };
	Evade* pEvadeBehavior{ nullptr };
	Seek* pSeekBehavior{ nullptr };

	VelocityMatch* pVelMatchBehavior{nullptr};

	// UI and rendering
	bool DebugRenderSteering{false};
	bool DebugRenderNeighborhood{true};
	bool DebugRenderPartitions{true};

	void RenderNeighborhood();
	void UpdateEvadeTarget();

};



