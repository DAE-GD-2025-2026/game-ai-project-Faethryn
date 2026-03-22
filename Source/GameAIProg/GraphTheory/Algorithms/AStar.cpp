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
		float currentLowestCost =  GetHeuristicCost(open_list[0], pGoalNode);
		currentNodeID = open_list[0]->GetId();
		
		for (int i {0}; i < open_list.size(); ++i)
		{
			Node* currentNode = open_list[i];
			
			float cost = GetHeuristicCost(currentNode, pGoalNode);
			
			if (cost < currentLowestCost)
			{
				currentLowestCost = cost;
				currentNodeID = currentNode->GetId();
			}
		}
		
		if (currentNodeID == pGoalNode->GetId())
		{
			path = closed_list;
			
			return path;
		}
		
		for (int i{0}; i < pGraph->GetConnections().size(); i++)
		{
			if (pGraph->GetConnections()[i]->GetFromId() == currentNodeID)
			{
				bool isClosed = false;
				
				for (int j {0}; j < closed_list.size(); ++j)
				{
					if (pGraph->GetConnections()[i]->GetToId() == closed_list[j]->GetId())
					{
						isClosed = true;
					}
				}
				
				if (!isClosed)
				{
					Node* pCurrentNeighbour = pGraph->GetNode(pGraph->GetConnections()[i]->GetToId()).get();
					
					bool isInOpenList = false;
					for (int j {0}; j < open_list.size(); ++j)
					{
						if (pCurrentNeighbour == open_list[j])
						{
							isInOpenList = true;
						}
					}
					
					if (!isInOpenList)
					{
						open_list.push_back(pCurrentNeighbour);
					}
				}
			}
		}
		
		closed_list.push_back(pGraph->GetNode(currentNodeID).get());
		
		for (int i {0}; i < open_list.size(); ++i)
		{
			if (open_list[i]->GetId() == currentNodeID)
			{
				open_list.erase(open_list.begin() + i);
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