#include "SpacePartitioning.h"

// --- Cell ---
// ------------
Cell::Cell(float Left, float Bottom, float Width, float Height)
{
	BoundingBox.Min = { Left, Bottom };
	BoundingBox.Max = { BoundingBox.Min.X + Width, BoundingBox.Min.Y + Height };
}

std::vector<FVector2D> Cell::GetRectPoints() const
{
	const float left = BoundingBox.Min.X;
	const float bottom = BoundingBox.Min.Y;
	const float width = BoundingBox.Max.X - BoundingBox.Min.X;
	const float height = BoundingBox.Max.Y - BoundingBox.Min.Y;

	std::vector<FVector2D> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(UWorld* pWorld,std::vector<ASteeringAgent*>& neighBourCollection , float Width, float Height, int Rows, int Cols, int MaxEntities)
	: pWorld{pWorld}
	  , SpaceWidth{Width}
	  , SpaceHeight{Height}
	  , NrOfRows{Rows}
	  , NrOfCols{Cols}
	  , Neighbors{neighBourCollection},
		NrOfNeighbors{0}
{
	Neighbors.resize(MaxEntities);

	//calculate bounds of a cell
	CellWidth = Width / Cols;
	CellHeight = Height / Rows;

	CellOrigin = FVector2d{0.0f - (CellWidth * (static_cast<float>(Rows) / static_cast<float>(2))),
		0.0f - (CellHeight * (Cols / 2))};
	
	// TODO create the cells

	for (int i {0}; i < Rows; i++ )
	{
		float left = CellOrigin.X + CellWidth * i;
		
		for (int j{0}; j < Cols; j++)
		{
			float bottom = CellOrigin.Y + CellHeight * j;
			
			
			Cells.push_back(new Cell(left, bottom, CellWidth, CellHeight));
		}
	}
}

void CellSpace::AddAgent(ASteeringAgent* Agent)
{
	// TODO Add the agent to the correct cell
	
	int index = PositionToIndex(Agent->GetPosition());
	
	Cells[index]->Agents.push_back(Agent);
}

void CellSpace::UpdateAgentCell(ASteeringAgent* Agent, const FVector2D& OldPos)
{
	//TODO Check if the agent needs to be moved to another cell.
	//TODO Use the calculated index for oldPos and currentPos for this
	
	int newIndex = PositionToIndex(Agent->GetPosition());
	int oldIndex = PositionToIndex(OldPos);
	
	if (newIndex != oldIndex)
	{
		Cells[oldIndex]->Agents.remove(Agent);
		Cells[newIndex]->Agents.push_back(Agent);
	}
}

void CellSpace::RegisterNeighbors(ASteeringAgent* Agent, float QueryRadius)
{
	// TODO Register the neighbors for the provided agent
	// TODO Only check the cells that are within the radius of the neighborhood
	
	
	FRect rectToCheck = FRect{FVector2D{Agent->GetPosition().X - QueryRadius, Agent->GetPosition().Y - QueryRadius},
		FVector2D{Agent->GetPosition().X + QueryRadius, Agent->GetPosition().Y + QueryRadius}};
	
	NrOfNeighbors = 0;

	for (auto cell : Cells)
	{
		if (DoRectsOverlap(cell->BoundingBox, rectToCheck))
		{
			for (auto agent : cell->Agents)
			{
				if (agent->GetName() != Agent->GetName())
				{
					Neighbors[NrOfNeighbors] = agent;
					NrOfNeighbors += 1;
				}
			}
		}
	}
}

void CellSpace::EmptyCells()
{
	for (Cell* c : Cells)
		c->Agents.clear();
}

void CellSpace::RenderCells() const
{
	// TODO Render the cells with the number of agents inside of it
}

int CellSpace::PositionToIndex(FVector2D const & Pos) const
{
	// TODO Calculate the index of the cell based on the position
	
	FVector2D relativePosition = Pos - CellOrigin;
	
	int column = FMath::FloorToInt(relativePosition.X / CellWidth);
	int row = FMath::FloorToInt(relativePosition.Y / CellHeight);
	
	int index = row * NrOfCols + column;
	
	return index;
}

bool CellSpace::DoRectsOverlap(FRect const & RectA, FRect const & RectB)
{
	// Check if the rectangles are separated on either axis
	if (RectA.Max.X < RectB.Min.X || RectA.Min.X > RectB.Max.X) return false;
	if (RectA.Max.Y < RectB.Min.Y || RectA.Min.Y > RectB.Max.Y) return false;
    
	// If they are not separated, they must overlap
	return true;
}