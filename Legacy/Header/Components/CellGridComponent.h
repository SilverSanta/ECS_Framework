#pragma once

#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;
class ComponentsManager;

class CellGridComponent : public IComponent
{
	// CONSTRUCTORS
public:
	CellGridComponent()
	{
		//std::cout << "     CELLGRID created\n";
	}
	~CellGridComponent() {
	
		//std::cout << "     CELLGRID: #" << m_Id << " deleted\n";
	}


	// OVERRIDES
public:
	virtual void _Update(uint64_t currentTime) override
	{
		if (this != nullptr)
		{

		}
	}
	virtual void _Render(SDL_Renderer* renderer) override
	{
		if (this != nullptr && bShowGrid == true)
		{
			//storing the background colour
			mfk::ColourTable backgroundcolour;
			SDL_GetRenderDrawColor(renderer, &backgroundcolour.red, &backgroundcolour.green, &backgroundcolour.blue, &backgroundcolour.alfa);

			//Changing draw colour
			mfk::ColourTable colourtable = mfk::Colour::Get_ColourTable(GridColour);
			SDL_SetRenderDrawColor(renderer, colourtable.red, colourtable.green, colourtable.blue, colourtable.alfa);

			//Display of the grid - a grid of lines
			if (bShowGrid) {

				float StartingXOnCameraView = X + OffsetFromOrigin.first;
				float StartingYOnCameraView = Y + OffsetFromOrigin.second;

				float CellWidth = (float)CellDimensions.first;
				float CellHeight = (float)CellDimensions.second;

				float EndingXOnCameraView = StartingXOnCameraView + CellWidth * NumberOfCellsOnX - 1;
				float EndingYOnCameraView = StartingYOnCameraView + CellHeight * NumberOfCellsOnY - 1;

				//DRAW THE GRID
			
				//Vertical lines
				SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView), (int)StartingYOnCameraView, (int)(StartingXOnCameraView), (int)EndingYOnCameraView);
				for (int i = 1; i < NumberOfCellsOnX; i++)
				{
					SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView + i * CellWidth - 1), (int)StartingYOnCameraView, (int)(StartingXOnCameraView + i * CellWidth - 1), (int)EndingYOnCameraView);
					SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView + i * CellWidth), (int)StartingYOnCameraView, (int)(StartingXOnCameraView + i * CellWidth), (int)EndingYOnCameraView);
				}
				SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView + NumberOfCellsOnX * CellWidth - 1), (int)StartingYOnCameraView, (int)(StartingXOnCameraView + NumberOfCellsOnX * CellWidth - 1), (int)EndingYOnCameraView);

				//Horizontal lines
				SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView), (int)EndingXOnCameraView, (int)(StartingYOnCameraView));
				for (int j = 1; j < NumberOfCellsOnY; j++)
				{
					SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight - 1), (int)EndingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight - 1));
					SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight), (int)EndingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight));
				}
				SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView + NumberOfCellsOnY * CellHeight - 1), (int)EndingXOnCameraView, (int)(StartingYOnCameraView + NumberOfCellsOnY * CellHeight - 1));
			}
			//returning to background colour
			SDL_SetRenderDrawColor(renderer, backgroundcolour.red, backgroundcolour.green, backgroundcolour.blue, backgroundcolour.alfa);
		}		
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, GridRepository);
	}


	// FUNCTIONS
	virtual void _Init(std::vector<CellGridComponent> &repo,  int cellsonx, int cellsony, int cellwidth, int cellheight, int offsetx, int offsety)
	{
		if (this != nullptr)
		{	
			// (2) REPOSITORY
			GridRepository = &repo;


			// (1) DATA
			NumberOfCellsOnX = cellsonx;
			NumberOfCellsOnY = cellsony;
			CellDimensions = std::make_pair(cellwidth, cellheight);
			OffsetFromOrigin = std::make_pair(offsetx, offsety);
			bShowGrid = true;
			GridColour = mfk::ColourList::WHITE_OPAQUE;
			X = 0;
			Y = 0;	
		}
	}


protected:
	// REPOSITORY
	std::vector<CellGridComponent>* GridRepository;
	// DATA
	float X;
	float Y;
	int NumberOfCellsOnX;
	int NumberOfCellsOnY;
	std::pair<int, int> CellDimensions;
	std::pair<int, int> OffsetFromOrigin;

	mfk::ColourList GridColour;

public:
	bool bShowGrid;

	// FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
	friend class RenderSystem;
};

