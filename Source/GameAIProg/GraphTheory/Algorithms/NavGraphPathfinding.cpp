#include "NavGraphPathfinding.h"

#include "AStar.h"
#include "PathSmoothing.h"
#include "VectorTypes.h"
#include "Shared/Graph/NavGraph/NavGraph.h"
#include "Shared/Graph/NavGraph/NavGraphNode.h"

using namespace GameAI;

std::vector<FVector2D> NavMeshPathfinding::FindPath(const FVector2D& startPos, const FVector2D& endPos,
	NavGraph* const pNavGraph, std::vector<FVector2D>& debugNodePositions, std::vector<NavLine>& debugPortals) 
{
	//Create the path to return
	std::vector<FVector2D> finalPath{};

	//Get the start and endTriangle
	
	FVector2D startPosOutPut{};
	const TriPolygon::Triangle * startTriangle = pNavGraph->GetNavPolygon()
	->GetClosestTriangleToPosition(startPos, startPosOutPut);
	
	FVector2D endPosOutPut{};
	const TriPolygon::Triangle * endTriangle = pNavGraph->GetNavPolygon()
	->GetClosestTriangleToPosition(endPos, endPosOutPut);
	
	if (startTriangle == endTriangle)
	{
		return finalPath;
	}

	//We have valid start/end triangles and they are not the same
	//=> Start looking for a path
	//Copy the graph
	
	std::unique_ptr<NavGraph> tempGraph = pNavGraph->Clone();

	//Create Extra node for the Start Node (Agent's position)
	
	int startNodeID = tempGraph->AddNode(std::make_unique<NavGraphNode>(startPos, -1));
	
	for (auto edge : startTriangle->GetEdges())
	{
		int edgeIndex = pNavGraph->GetNavPolygon()->FindEdgeIndex(edge).value();
		int tempNode = tempGraph->GetNodeIdFromEdgeIndex(edgeIndex);
		
		tempGraph->AddConnection(startNodeID, tempNode);
	}

	//Create extra node for the endNode

	int endNodeID = tempGraph->AddNode(std::make_unique<NavGraphNode>(endPos, -1));
	
	for (auto edge : endTriangle->GetEdges())
	{
		int edgeIndex = pNavGraph->GetNavPolygon()->FindEdgeIndex(edge).value();
		int tempNode = tempGraph->GetNodeIdFromEdgeIndex(edgeIndex);
		
		tempGraph->AddConnection(endNodeID, tempNode);
	}
	
	tempGraph->SetConnectionCostsToDistances();
	
	//Run A star on new graph
	AStar* aStarPathFinder = new AStar{tempGraph.get(), HeuristicFunctions::Euclidean};
	
	std::vector<Node*> pathNodes = aStarPathFinder->FindPath(tempGraph->GetNode(startNodeID).get(),
		tempGraph->GetNode(endNodeID).get());
	//Debug Visualisation

	for (auto node : pathNodes)
	{
		finalPath.push_back(node->GetPosition());
	}

	// Extra: Run optimiser on new graph (First check if everything works without SSFA!)
	debugPortals = SSFA::FindPortals(pathNodes, *pNavGraph->GetNavPolygon());
	finalPath = SSFA::OptimizePortals(debugPortals, *pNavGraph->GetNavPolygon());
	
	return finalPath;
}

std::vector<FVector2D> NavMeshPathfinding::FindPath(const FVector2D& startPos, const FVector2D& endPos, NavGraph* const pNavGraph)
{
	std::vector<FVector2D> debugNodePositions{};
	std::vector<NavLine> debugPortals{};

	return FindPath(startPos, endPos, pNavGraph, debugNodePositions, debugPortals);
}