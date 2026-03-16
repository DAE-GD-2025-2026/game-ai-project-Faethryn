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
	std::vector<Node*> path;
	
	std::queue<Node*> queue{};
	
	queue.push(pStartNode);
	
	std::vector<Node*> visited{};
	
	visited.push_back(pStartNode);
	
	std::map<Node*, Node*> parent{};
	
	parent[pStartNode] = nullptr;
	
	while(!queue.empty())
	{
		Node* pCurrentNode = queue.front();
		queue.pop();
		//visited.push_back(pCurrentNode);
		
		if (pCurrentNode == pDestinationNode)
		{
			//path.push_back(pCurrentNode);
			return reconstructPath(parent, pStartNode, pDestinationNode);
		}
		else
		{
			for (int i{0}; i < pGraph->GetConnections().size(); ++i)
			{
				if (pGraph->GetConnections()[i]->GetFromId() == pCurrentNode->GetId())
				{
					bool isInVisited = false;
					for (int j{0}; j < visited.size(); ++j)
					{
						if (visited[j] == pCurrentNode)
						{
							isInVisited = true;
						}
					}
					
					if (!isInVisited)
					{
						queue.push(pGraph->GetNode(pGraph->GetConnections()[i]->GetToId()).get());
						visited.push_back(queue.front());
						parent[queue.front()] = pCurrentNode;
					}
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
	}
	std::reverse(path.begin(), path.end());
	
	return path;
}
