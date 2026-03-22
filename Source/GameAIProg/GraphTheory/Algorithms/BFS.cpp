#include "BFS.h"


#include <queue>

#include "Shared/Graph/Graph.h"

using namespace GameAI;

BFS::BFS(Graph* const pGraph)
	: pGraph(pGraph)
{
}

// TODO Breath First Search Algorithm searches for a path from the startNode to the destinationNode
std::vector<Node*> BFS::FindPath(Node* const pStartNode, Node* const pDestinationNode) const
{
	std::vector<Node*> path{};
	
	std::queue<Node*> queue{};
	
	queue.push(pStartNode);
	
	std::vector<Node*> Closed{};
	
	//visited.push_back(pStartNode);
	
	std::map<Node*, Node*> parent{};
	
	parent[pStartNode] = nullptr;
	
	while (!queue.empty())
	{
		Node* currentNode = queue.front();
		queue.pop_front();
		Closed.push_back(currentNode);
		
		if (currentNode == pDestinationNode)
		{
			return reconstructPath(parent, pStartNode, pDestinationNode);
		}
		
		std::vector<Node*> validNeighbours{};
		
		for (int i = 0; i < pGraph->GetConnections().size(); i++)
		{
			if (pGraph->GetConnections()[i]->GetFromId() == currentNode->GetId())
			{
				bool isAlreadyVisited = false;
				for (int j = 0; j < Closed.size(); j++)
				{
					if (pGraph->GetNode(pGraph->GetConnections()[i]->GetToId()).get() == Closed[j])
					{
						isAlreadyVisited = true;
					}
				}
				
				if (!isAlreadyVisited)
				{
					validNeighbours.push_back(currentNode);
				}
			}
		}
		
		for (int i = 0; i < validNeighbours.size(); i++)
		{
			queue.push(validNeighbours[i]);
		}
	}
	return path;
}

std::vector<Node*> BFS::reconstructPath(std::map<Node*, Node*>& parent,
	Node* const pStartNode, Node* const pDestinationNode) const
{
	std::vector<Node*> path;
	
	Node* pCurrentNode = pDestinationNode;
	
	path.push_back(pCurrentNode);
	while (parent[pCurrentNode] != nullptr)
	{
		path.push_back(parent[pCurrentNode]);
		pCurrentNode = parent[pCurrentNode];
	}
	std::reverse(path.begin(), path.end());
	
	return path;
}
