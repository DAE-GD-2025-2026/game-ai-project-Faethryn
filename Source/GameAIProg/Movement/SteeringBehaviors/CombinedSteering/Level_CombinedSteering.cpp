#include "Level_CombinedSteering.h"

#include "imgui.h"


// Sets default values
ALevel_CombinedSteering::ALevel_CombinedSteering()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevel_CombinedSteering::BeginPlay()
{
	Super::BeginPlay();

	AddAgent();
	AddAgent(CombinedBehaviourType::RangedEvade);

	m_CombinedSteeringAgents[1].TargetAgent = 0;
}

void ALevel_CombinedSteering::BeginDestroy()
{
	Super::BeginDestroy();

}

void ALevel_CombinedSteering::SetAgentBehavior(CombinedSteeringAgent& Agent)
{
	switch ((static_cast<CombinedBehaviourType>(Agent.SelectedBehavior)))
	{
	case CombinedBehaviourType::Drunk:
		Agent.Behaviour = std::make_unique<BlendedSteering>(std::vector<BlendedSteering::WeightedBehavior>{BlendedSteering::WeightedBehavior(new Seek(), 0.5f), BlendedSteering::WeightedBehavior(new Wander(), 0.5f)});
		break;
	case CombinedBehaviourType::RangedEvade:
		Agent.Behaviour = std::make_unique<PrioritySteering>(std::vector<ISteeringBehavior*>{new Evade(), new Wander()});
		break;
	default:
		Agent.Behaviour = std::make_unique<BlendedSteering>(std::vector<BlendedSteering::WeightedBehavior>{BlendedSteering::WeightedBehavior(new Seek(), 0.5f), BlendedSteering::WeightedBehavior(new Wander(), 0.5f)});
		break;
	}

	UpdateTarget(Agent);

	Agent.Agent->SetSteeringBehavior(Agent.Behaviour.get());
}

bool ALevel_CombinedSteering::AddAgent(CombinedBehaviourType BehaviorType)
{
	CombinedSteeringAgent agent{};
	agent.Agent = GetWorld()->SpawnActor<ASteeringAgent>(SteeringAgentClass, FVector{ 0,0,90 }, FRotator::ZeroRotator);
	if (IsValid(agent.Agent))
	{
		agent.SelectedBehavior = static_cast<int>(BehaviorType);

		agent.TargetAgent = -1;

		SetAgentBehavior(agent);

		m_CombinedSteeringAgents.push_back(std::move(agent));

		return true;
	}
	
	return false;
}

void ALevel_CombinedSteering::RemoveAgent(unsigned int Index)
{
	m_CombinedSteeringAgents[Index].Agent->Destroy();
	
	m_CombinedSteeringAgents.erase(m_CombinedSteeringAgents.begin() + Index);
}

void ALevel_CombinedSteering::UpdateTarget(CombinedSteeringAgent& Agent)
{
	bool const bUseMouseAsTarget = Agent.TargetAgent < 0;
	if (!bUseMouseAsTarget)
	{
		ASteeringAgent* const TargetAgent = m_CombinedSteeringAgents[Agent.TargetAgent].Agent;

		FTargetData Target;
		Target.Position = TargetAgent->GetPosition();
		Target.Orientation = TargetAgent->GetRotation();
		Target.LinearVelocity = TargetAgent->GetLinearVelocity();
		Target.AngularVelocity = TargetAgent->GetAngularVelocity();

		Agent.Behaviour->SetTarget(Target);
	}
	else
	{
		Agent.Behaviour->SetTarget(MouseTarget);
	}
}

// Called every frame
void ALevel_CombinedSteering::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
#pragma region UI
	//UI
	{
		//Setup
		bool windowActive = true;
		ImGui::SetNextWindowPos(WindowPos);
		ImGui::SetNextWindowSize(WindowSize);
		ImGui::Begin("Game AI", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
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
		ImGui::Spacing();
	
		ImGui::Text("Flocking");
		ImGui::Spacing();
		ImGui::Spacing();
	
		if (ImGui::Checkbox("Debug Rendering", &CanDebugRender))
		{
   // TODO: Handle the debug rendering of your agents here :)
		}
		ImGui::Checkbox("Trim World", &TrimWorld->bShouldTrimWorld);
		if (TrimWorld->bShouldTrimWorld)
		{
			ImGuiHelpers::ImGuiSliderFloatWithSetter("Trim Size",
				TrimWorld->GetTrimWorldSize(), 1000.f, 3000.f,
				[this](float InVal) { TrimWorld->SetTrimWorldSize(InVal); });
		}
		
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
	
		ImGui::Text("Behavior Weights");
		ImGui::Spacing();


		 ImGuiHelpers::ImGuiSliderFloatWithSetter("Seek",
			 m_CombinedSteeringAgents[0].Behaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[0].Weight, 0.f, 1.f,
		 	[this](float InVal) { m_CombinedSteeringAgents[0].Behaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[0].Weight = InVal; }, "%.2f");
		
		 ImGuiHelpers::ImGuiSliderFloatWithSetter("Wander",
			 m_CombinedSteeringAgents[0].Behaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[1].Weight, 0.f, 1.f,
		 [this](float InVal) { m_CombinedSteeringAgents[0].Behaviour->As<BlendedSteering>()->GetWeightedBehaviorsRef()[1].Weight = InVal; }, "%.2f");
	
		//End
		ImGui::End();

		for (CombinedSteeringAgent& agent : m_CombinedSteeringAgents)
		{
			if (agent.Agent)
			{
				UpdateTarget(agent);
			}
		}
	}
#pragma endregion
	
	// Combined Steering Update
 // TODO: implement handling mouse click input for seek
 // TODO: implement Make sure to also evade the wanderer
}
