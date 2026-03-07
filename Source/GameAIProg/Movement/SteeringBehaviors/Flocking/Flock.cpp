#include "Flock.h"
#include "FlockingSteeringBehaviors.h"
#include "Shared/ImGuiHelpers.h"


Flock::Flock(
	UWorld* pWorld,
	TSubclassOf<ASteeringAgent> AgentClass,
	int FlockSize,
	float WorldSize,
	ASteeringAgent* const pAgentToEvade,
	bool bTrimWorld)
	: pWorld{pWorld}
	, FlockSize{ FlockSize }
	, pAgentToEvade{pAgentToEvade}
{
	Agents.resize(FlockSize);

	Neighbors.resize(FlockSize);
 // TODO: initialize the flock and the memory pool

	
	pSeparationBehavior = new Separation(*this);
	pCohesionBehavior = new Cohesion(*this);
	pVelMatchBehavior = new VelocityMatch(*this);
	pEvadeBehavior = new Evade();
	pSeekBehavior = new Seek();

	pEvadeBehavior->SetMaxAvoidanceDistance(1000);

	pBoidBehaviour = new BlendedSteering(std::vector<BlendedSteering::WeightedBehavior>{BlendedSteering::WeightedBehavior(pSeparationBehavior, 0.2f),
		BlendedSteering::WeightedBehavior(pCohesionBehavior, 0.2f), BlendedSteering::WeightedBehavior(pVelMatchBehavior, 0.2f), BlendedSteering::WeightedBehavior(pSeekBehavior, 0.2f)});
	pEvadeBoidBehavior = new PrioritySteering(std::vector<ISteeringBehavior*>{pEvadeBehavior, pBoidBehaviour});


	for (int i{ 0 } ; i < Agents.capacity(); i++)
	{
		while (Agents[i] == nullptr)
		{
			float randomYOffset = FMath::FRandRange(-500.0f, 500.0f);
			float randomXOffset = FMath::FRandRange(-500.0f, 500.0f);

			Agents[i] = pWorld->SpawnActor<ASteeringAgent>(AgentClass, FVector{ randomXOffset, randomYOffset ,90 }, FRotator::ZeroRotator);
		}
		Agents[i]->SetActorTickEnabled(false);

		Agents[i]->SetSteeringBehavior(pEvadeBoidBehavior);
	}
}

Flock::~Flock()
{
 // TODO: Cleanup any additional data

	/*for (int i{ 0 }; i < FlockSize; i++)
	{
		delete Agents[i];
	}*/

	Agents.clear();

	delete pSeparationBehavior;
	delete pCohesionBehavior;
	delete pVelMatchBehavior;
	delete pSeekBehavior;
	delete pBoidBehaviour;
}

void Flock::Tick(float DeltaTime)
{
 // TODO: update the flock
 // TODO: for every agent:
  // TODO: register the neighbors for this agent (-> fill the memory pool with the neighbors for the currently evaluated agent)
  // TODO: update the agent (-> the steeringbehaviors use the neighbors in the memory pool)
  // TODO: trim the agent to the world

	UpdateEvadeTarget();

	for (ASteeringAgent* agent : Agents)
	{
		RegisterNeighbors(agent);
		agent->Tick(DeltaTime);
	}

}

void Flock::UpdateEvadeTarget()
{

	FTargetData Target;
	Target.Position = pAgentToEvade->GetPosition();
	Target.Orientation = pAgentToEvade->GetRotation();
	Target.LinearVelocity = pAgentToEvade->GetLinearVelocity();
	Target.AngularVelocity = pAgentToEvade->GetAngularVelocity();


	pEvadeBehavior->SetTarget(Target);
}

void Flock::RenderDebug()
{
 // TODO: Render all the agents in the flock
}

void Flock::ImGuiRender(ImVec2 const& WindowPos, ImVec2 const& WindowSize)
{
#ifdef PLATFORM_WINDOWS
#pragma region UI
	//UI
	{
		//Setup
		bool bWindowActive = true;
		ImGui::SetNextWindowPos(WindowPos);
		ImGui::SetNextWindowSize(WindowSize);
		ImGui::Begin("Gameplay Programming", &bWindowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
		ImGui::Text("LMB: place target");
		ImGui::Text("RMB: move cam.");
		ImGui::Text("Scrollwheel: zoom cam.");
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("STATS");
		ImGui::Indent();
		ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Unindent();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Flocking");
		ImGui::Spacing();

  // TODO: implement ImGUI checkboxes for debug rendering here


		ImGui::Text("Behavior Weights");
		ImGui::Spacing();

  // TODO: implement ImGUI sliders for steering behavior weights here
		ImGuiHelpers::ImGuiSliderFloatWithSetter("Cohesion",
			pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[1].Weight, 0.f, 1.f,
			[this](float InVal) { pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[1].Weight = InVal; }, "%.2f"); 
		ImGuiHelpers::ImGuiSliderFloatWithSetter("Evasion",
			pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[0].Weight, 0.f, 1.f,
			[this](float InVal) { pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[0].Weight = InVal; }, "%.2f");
		ImGuiHelpers::ImGuiSliderFloatWithSetter("VelocityMatch",
				pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[2].Weight, 0.f, 1.f,
				[this](float InVal) { pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[2].Weight = InVal; }, "%.2f");
		ImGuiHelpers::ImGuiSliderFloatWithSetter("Seek",
			pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[3].Weight, 0.f, 1.f,
			[this](float InVal) { pBoidBehaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[3].Weight = InVal; }, "%.2f");
		//End
		ImGui::End();
	}
#pragma endregion
#endif
}

void Flock::RenderNeighborhood()
{
 // TODO: Debugrender the neighbors for the first agent in the flock
}

#ifndef GAMEAI_USE_SPACE_PARTITIONING
void Flock::RegisterNeighbors(ASteeringAgent* const pAgent)
{
	NrOfNeighbors = 0;
	for (ASteeringAgent* agent : Agents)
	{
		if (pAgent == agent)
		{
			return;
		}

		float distance{ 0 };

		distance = FVector2D{}.Distance(pAgent->GetPosition(), agent->GetPosition());


		if (distance < NeighborhoodRadius)
		{
			Neighbors[NrOfNeighbors] = agent;
			NrOfNeighbors += 1;
		}
	}
}
#endif

FVector2D Flock::GetAverageNeighborPos() const
{
	FVector2D avgPosition = FVector2D::ZeroVector;

	if (NrOfNeighbors == 0)
	{
		return avgPosition;
	}

	for (int i{0}; i < NrOfNeighbors; i++)
	{
		avgPosition += Neighbors[i]->GetPosition();
	}

	avgPosition = avgPosition / NrOfNeighbors;
	
	return avgPosition;
}

FVector2D Flock::GetAverageNeighborVelocity() const
{
	FVector2D avgVelocity = FVector2D::ZeroVector;

	if (NrOfNeighbors == 0)
	{
		return avgVelocity;

	}

	for (int i{ 0 }; i < NrOfNeighbors; i++)
	{
		avgVelocity += FVector2D{ Neighbors[i]->GetVelocity().X, Neighbors[i]->GetVelocity().Y };
	}

	avgVelocity = avgVelocity / NrOfNeighbors;

	return avgVelocity;
}

void Flock::SetTarget_Seek(FSteeringParams const& Target)
{
 // TODO: Implement
	pSeekBehavior->SetTarget(Target);
}


