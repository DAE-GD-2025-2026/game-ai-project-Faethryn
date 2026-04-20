#include "NavGraph.h"

#include "NavGraphNode.h"

GameAI::NavGraph::NavGraph(std::unique_ptr<TriPolygon> && NavPoly)
	: Graph{false}
	, pNavPoly{std::move(NavPoly)}
{
	CreateNavigationGraph();
}

GameAI::NavGraph::NavGraph(const NavGraph& Other)
	: Graph(false)
{
	Nodes.reserve(Other.Nodes.size());
	for (std::unique_ptr<Node> const & OtherNode : Other.Nodes)
	{
		Nodes.push_back(std::make_unique<NavGraphNode>(*dynamic_cast<NavGraphNode*>(OtherNode.get())));
	}
        
	Connections.reserve(Other.Connections.size());
	for (std::unique_ptr<Connection> const & OtherConnection : Other.Connections)
	{
		Connections.push_back(std::make_unique<Connection>(*OtherConnection.get()));
	}
}

std::unique_ptr<GameAI::NavGraph> GameAI::NavGraph::Clone() const
{
	return std::make_unique<NavGraph>(*this);
}

int GameAI::NavGraph::GetNodeIdFromEdgeIndex(int EdgeIdx) const
{
	if (EdgeIdx >= 0)
	{
		for (auto const & pNode : Nodes)
		{
			if (reinterpret_cast<NavGraphNode*>(pNode.get())->GetEdgeIdx() == EdgeIdx)
			{
				return pNode->GetId();
			}
		}
	}
	
	return Graphs::InvalidNodeId;
}

void GameAI::NavGraph::CreateNavigationGraph()
{
	//1. Go over all the edges of the navigation mesh and create nodes
			// Create node here
	
	for (auto edge : pNavPoly->GetEdges())
	{
		FVector position1 = edge.GetP1(*pNavPoly.get());
		FVector position2 = edge.GetP2(*pNavPoly.get());
		
		FVector nodePosition = (position1 + position2)/2.f;
		
		int edgeId = pNavPoly->FindEdgeIndex(edge).value();
		
		AddNode(std::make_unique<NavGraphNode>(
			FVector2D(nodePosition.X, nodePosition.Y), edgeId));
		
		UE_LOG(LogTemp, Warning, TEXT("Created Node at triangle Index: %d"), edgeId);
	}

	std::vector<TriPolygon::Triangle> triangles = pNavPoly->GetTriangles();
	
	for (auto triangle : triangles)
	{
		std::vector<int> nodesInTriangle;
		
		for (auto edge : triangle.GetEdges())
		{
			int edgeId = pNavPoly->FindEdgeIndex(edge).value();
			
			nodesInTriangle.push_back(GetNodeIdFromEdgeIndex(edgeId));
		}
		
		if (nodesInTriangle.size() == 2)
		{
			AddConnection(nodesInTriangle[0], nodesInTriangle[1]);
		}
		if (nodesInTriangle.size() == 3)
		{
			AddConnection(nodesInTriangle[0], nodesInTriangle[1]);
			AddConnection(nodesInTriangle[0], nodesInTriangle[2]);
			AddConnection(nodesInTriangle[1], nodesInTriangle[2]);
		}
	}
	
	//2. Create connections now that every node is created	
		//2 valid nodes -> 1 connection
		//3 valid nodes -> 3 connections
		
	//3. Set the connections cost to the actual distance
	
	SetConnectionCostsToDistances();
}
