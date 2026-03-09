#pragma once
#include <stack>
#include "Shared/Graph/Graph.h"

namespace GameAI
{
	enum class Eulerianity
	{
		notEulerian,
		semiEulerian,
		eulerian,
	};

	class EulerianPath final
	{
	public:
		EulerianPath(Graph* const pGraph);

		Eulerianity IsEulerian() const;
		std::vector<Node*> FindPath(Eulerianity& eulerianity) const;

	private:
		void VisitAllNodesDFS(const std::vector<Node*>& pNodes, std::vector<bool>& visited, int startIndex) const;
		bool IsConnected() const;
		Graph* m_pGraph;
	};

	inline EulerianPath::EulerianPath(Graph* const pGraph)
		: m_pGraph(pGraph)
	{
		
	}

	inline Eulerianity EulerianPath::IsEulerian() const
	{
		// TODO If the graph is not connected, there can be no Eulerian Trail
		if(IsConnected() == false)
		{
			return Eulerianity::notEulerian;
		}
		// TODO Count nodes with odd degree 
		int nodesOfOddDegree{0};
		for (auto node : m_pGraph->GetActiveNodes())
		{
			int numberOfConnections{0};
			
			for (int i{0}; i < m_pGraph->GetConnections().size(); i++)
			{
				if (node->GetId() == m_pGraph->GetConnections()[i]->GetFromId() ||
					node->GetId() == m_pGraph->GetConnections()[i]->GetToId())
				{
					numberOfConnections++;
				}
			}
			
			if (numberOfConnections % 2 != 0)
			{
				nodesOfOddDegree++;
			}
		}
		// TODO A connected graph with more than 2 nodes with an odd degree (an odd amount of connections) is not Eulerian
		if (nodesOfOddDegree > 2)
		{
			return Eulerianity::notEulerian;
		}
		
		// TODO A connected graph with exactly 2 nodes with an odd degree is Semi-Eulerian (unless there are only 2 nodes)
		// TODO An Euler trail can be made, but only starting and ending in these 2 nodes
		if (nodesOfOddDegree == 2)
		{
			if (m_pGraph->GetActiveNodes().size() == 2)
			{
				return Eulerianity::notEulerian;
			}
			return Eulerianity::semiEulerian;
		}
		// TODO A connected graph with no odd nodes is Eulerian
		if (nodesOfOddDegree == 0)
		{
			return Eulerianity::eulerian;
		}
		return Eulerianity::notEulerian;
	}

	inline std::vector<Node*> EulerianPath::FindPath(Eulerianity& eulerianity) const
	{
		// Get a copy of the graph because this algorithm involves removing edges
		Graph graphCopy = m_pGraph->Clone();
		std::vector<Node*> Path = {};
		std::vector<Node*> Nodes = graphCopy.GetActiveNodes();
		int currentNodeId{ Graphs::InvalidNodeId };
		
		// TODO Check if there can be an Euler path
		// TODO If this graph is not eulerian, return the empty path
		if (eulerianity == Eulerianity::notEulerian)
		{
			return Path;
		}
		// TODO Start algorithm loop
		std::stack<int> nodeStack;
		int CurrentIndex{ 0 };
		
		
		if (eulerianity == Eulerianity::semiEulerian)
		{
			if (Nodes.size() == 2)
			{
				CurrentIndex = 0;
			}
			
			for (int i{0}; i < Nodes.size(); i++)
			{
				int numberOfConnections{0};
			
				for (int j{0}; j < m_pGraph->GetConnections().size(); j++)
				{
					if (Nodes[j]->GetId() == m_pGraph->GetConnections()[j]->GetFromId() ||
						Nodes[j]->GetId() == m_pGraph->GetConnections()[j]->GetToId())
					{
						numberOfConnections++;
					}
				}
			
				if (numberOfConnections % 2 != 0)
				{
					CurrentIndex = i;
					break;
				}
			}
		}
		
		bool validPathFound = false;

		currentNodeId = Nodes[CurrentIndex]->GetId();
		
		while (validPathFound == false)
		{
			nodeStack.push(currentNodeId);
			currentNodeId = nodeStack.top();

			int neighbourCount {0};
			for (int i = 0; i < graphCopy.GetConnections().size(); ++i)
			{
				if (graphCopy.GetConnections()[i]->GetFromId() == currentNodeId)
				{
					neighbourCount++;
					graphCopy.RemoveConnection(graphCopy.GetConnections()[i].get());
					currentNodeId = graphCopy.GetConnections()[i]->GetToId();
					break;
				}
			}
			
			if (neighbourCount == 0)
			{
				nodeStack.pop();
				if (nodeStack.size() == 0)
				{
					validPathFound = true;
				}
			}
			
			
		}
		
		Path.push_back(m_pGraph->GetNode(currentNodeId).get());
		std::reverse(Path.begin(), Path.end());
		return Path;
	}

	inline void EulerianPath::VisitAllNodesDFS(const std::vector<Node*>& Nodes, std::vector<bool>& visited, int startIndex ) const
	{
		// TODO Mark the visited node
		visited[startIndex] = true;
		
		for (int j{0}; j< m_pGraph->GetConnections().size(); j++)
		{
			if (m_pGraph->GetConnections()[j]->GetFromId() == Nodes[startIndex]->GetId())
			{
				for (int i{0}; i < Nodes.size(); i++)
				{
					if (visited[i] == false)
					{
						if (m_pGraph->GetConnections()[j]->GetToId() == Nodes[i]->GetId())
						{
							VisitAllNodesDFS(Nodes, visited, i);
						}
					}
				}
			}
		}
		
		// TODO Ask the graph for the connections from that node
		// TODO recursively visit any valid connected nodes that were not visited before
		// TODO Tip: use an index-based for-loop to find the correct index
	}

	inline bool EulerianPath::IsConnected() const
	{
		std::vector<Node*> Nodes = m_pGraph->GetActiveNodes();
		if (Nodes.size() == 0)
			return false;

		std::vector<bool> visited(Nodes.size());
		// TODO choose a starting node
		int startingID = 0;
		
		VisitAllNodesDFS(Nodes, visited, startingID);
		
		// TODO start a depth-first-search traversal from the node that has at least one connection
		
		// TODO if a node was never visited, this graph is not connected
		
		for (bool gotVisited : visited)
		{
			if (gotVisited == false)
			{
				return false;
			}
		}
		
		return true;
	}
	
	
}