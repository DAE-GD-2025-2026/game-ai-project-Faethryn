#include "AStar.h"

using namespace GameAI;

AStar::AStar(Graph* const pGraph, HeuristicFunctions::Heuristic hFunction)
	: pGraph(pGraph)
	, HeuristicFunction(hFunction)
{
}

std::vector<Node*>AStar::FindPath(Node* const pStartNode, Node* const pGoalNode)
{
	std::vector<Node*> path{};
	
	std::vector<Node*> open_list{};
	std::vector<Node*> closed_list{};
	
	open_list.push_back(pStartNode);

	int currentNodeID = pStartNode->GetId();
	
	while (!open_list.empty())
	{
		std::vector<int> neighbourCosts{};
		std::vector<Node*> neighbours{};
		
		for (int i{0}; i < pGraph->GetConnections().size(); i++)
		{
			if (pGraph->GetConnections()[i]->GetFromId() == currentNodeID)
			{
			    neighbours.push_back(pGraph->GetNode(pGraph->GetConnections()[i]->GetToId()).get());
				FVector2D currentPos = pGraph->GetNode(currentNodeID)->GetPosition();
				FVector2D OtherPos = pGraph->GetNode(pGraph->GetConnections()[i]->GetToId())->GetPosition();
				
			}
		}
	}
	
	return path;
}

float AStar::GetHeuristicCost(Node* const pStartNode, Node* const pEndNode) const
{
	FVector2D toDestination = pGraph->GetNode(pEndNode->GetId())->GetPosition() - pGraph->GetNode(pStartNode->GetId())->GetPosition();
	return HeuristicFunction(abs(toDestination.X), abs(toDestination.Y));
}