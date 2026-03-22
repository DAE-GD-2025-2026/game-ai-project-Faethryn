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
	
	std::vector<Node*> Visited{};
	//std::vector<Node*> Next{};
	
	std::map<Node*, Node*> parents{};
	
	parents[pStartNode] = nullptr;
	Visited.push_back(pStartNode);
	queue.push(pStartNode);
	
	while (!queue.empty())
	{
		Node* pCurrentNode = queue.front();
		queue.pop();
		
		if (pCurrentNode == pDestinationNode)
		{
			return reconstructPath(parents, pStartNode, pDestinationNode);
		}
		
		for (int i{0}; i < pGraph->GetConnections().size(); ++i)
		{
			if (pGraph->GetConnections()[i]->GetFromId() == pCurrentNode->GetId())
			{
				int currentNeighbourID = pGraph->GetConnections()[i]->GetToId();
				
				Node* currentNeighbourNode = pGraph->GetNode(currentNeighbourID).get();
				
				bool isVisited = false;
				for (int j{0}; j < Visited.size(); ++j)
				{
					if (Visited[j] == currentNeighbourNode)
					{
						isVisited = true;
					}
				}
				
				if (!isVisited)
				{
					Visited.push_back(currentNeighbourNode);
					parents[currentNeighbourNode] = pCurrentNode;
					queue.push(currentNeighbourNode);
				}
			}
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
