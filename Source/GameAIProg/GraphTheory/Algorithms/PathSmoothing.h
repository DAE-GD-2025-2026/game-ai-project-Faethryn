#pragma once
#include <vector>

#include "NavGraphPathfinding.h"
#include "Movement/Pathfinding/Navmesh/TriPolygon.h"
#include "Shared/Graph/Graph.h"
#include "Shared/Graph/NavGraph/NavGraphNode.h"

namespace GameAI
{
	class SSFA final
{
public:
	//=== SSFA Functions ===
	//--- References ---
	//http://digestingduck.blogspot.be/2010/03/simple-stupid-funnel-algorithm.html
	//https://gamedev.stackexchange.com/questions/68302/how-does-the-simple-stupid-funnel-algorithm-work
	static std::vector<NavLine> FindPortals(std::vector<Node*> const & Path, TriPolygon const & NavPoly)
	{
		//Container
		std::vector<NavLine> Portals = {};
		
		Portals.push_back(NavLine{Path[0]->GetPosition(), Path[0]->GetPosition()});
		
		//For each node received, get it's corresponding line
		for (int i{0}; i < Path.size(); i++)
		{
			NavGraphNode* pNode = static_cast<NavGraphNode*>(Path[i]);
			
			int edgeIndex = pNode->GetEdgeIdx();
			
			for (auto edge : NavPoly.GetEdges())
			{
				for (auto edgeIndexInEdge : edge.EdgeIndices)
				{
					if (edgeIndex == edgeIndexInEdge)
					{
						FVector point1 = edge.GetP1(NavPoly);
						FVector point2 = edge.GetP2(NavPoly);
						
						FVector2D point1Diff = FVector2D{point1.X, point1.Y} - Path[i]->GetPosition();
						FVector2D point2Diff = FVector2D{point2.X, point2.Y} - Path[i]->GetPosition();
						
						float crossOfDiff = FVector2D::CrossProduct(point1Diff, point2Diff);
						if (crossOfDiff < 0)
						{
							FVector point2Storage = point2;
							
							point2 = point1;
							point1 = point2Storage;
						}
						NavLine thisLine = NavLine{FVector2D{point1.X, point1.Y}, FVector2D{point1.X, point1.Y}};
						Portals.push_back(thisLine);
					}
				}
			}
		}
		
		Portals.push_back(NavLine{Path[Path.size()-1]->GetPosition(), Path[Path.size()-1]->GetPosition()});
		
			//Redetermine it's "orientation" based on the required path (left-right vs right-left) - p1 should be right point
		
			//Store portal

		//Add degenerate portal to force end evaluation

		return Portals;
	}

	static std::vector<FVector2D> OptimizePortals( std::vector<NavLine> const & Portals, TriPolygon const & NavPoly)
	{
		std::vector<FVector2D> Path{};
		
		FVector2D currentApex = Portals[0].P2;
		int currentApexIndex = 0;
		
		int currentRightLegIndex = 1;
		int currentLeftLegIndex = 1;
		
		int currentPortalIndex = 2;
		
		bool hasReachedEnd = false;
		while (!hasReachedEnd)
		{
			//Right check
			
			FVector2D rightLeg = Portals[currentRightLegIndex].P1 - currentApex;
			FVector2D newRightLeg = Portals[currentPortalIndex].P1 - currentApex;
			
			float rightCross = FVector2D::CrossProduct(rightLeg, newRightLeg);
			
			if (rightCross > 0)
			{
				
			}
			
			//Left Check
			
			FVector2D leftLeg = Portals[currentRightLegIndex].P2 - currentApex;
			FVector2D newLeftLeg = Portals[currentPortalIndex].P2 - currentApex;
			
			float leftCross = FVector2D::CrossProduct(leftLeg, newLeftLeg);
			
			if (leftCross < 0)
			{
				
			}
		}
		return Path;
	}
private:
	SSFA() {};
	~SSFA() {};
};
}
