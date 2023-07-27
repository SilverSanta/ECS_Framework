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
		if (this != nullptr && m_bShowGrid == true)
		{
			//storing the background colour
			mfk::ColourTable backgroundcolour;
			SDL_GetRenderDrawColor(renderer, &backgroundcolour.red, &backgroundcolour.green, &backgroundcolour.blue, &backgroundcolour.alfa);

			//Changing draw colour
			mfk::ColourTable colourtable = mfk::Colour::Get_ColourTable(m_GridColour);
			SDL_SetRenderDrawColor(renderer, colourtable.red, colourtable.green, colourtable.blue, colourtable.alfa);

			//Display of the grid - a grid of lines
			if (m_bShowGrid) {

				float StartingXOnCameraView = m_X + m_OffsetFromOrigin.first;
				float StartingYOnCameraView = m_Y + m_OffsetFromOrigin.second;

				float CellWidth = (float)m_CellDimensions.first;
				float CellHeight = (float)m_CellDimensions.second;

				float EndingXOnCameraView = StartingXOnCameraView + CellWidth * m_NumberOfCellsOnX - 1;
				float EndingYOnCameraView = StartingYOnCameraView + CellHeight * m_NumberOfCellsOnY - 1;

				//DRAW THE GRID
			
				//Vertical lines
				SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView), (int)StartingYOnCameraView, (int)(StartingXOnCameraView), (int)EndingYOnCameraView);
				for (int i = 1; i < m_NumberOfCellsOnX; i++)
				{
					SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView + i * CellWidth - 1), (int)StartingYOnCameraView, (int)(StartingXOnCameraView + i * CellWidth - 1), (int)EndingYOnCameraView);
					SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView + i * CellWidth), (int)StartingYOnCameraView, (int)(StartingXOnCameraView + i * CellWidth), (int)EndingYOnCameraView);
				}
				SDL_RenderDrawLine(renderer, (int)(StartingXOnCameraView + m_NumberOfCellsOnX * CellWidth - 1), (int)StartingYOnCameraView, (int)(StartingXOnCameraView + m_NumberOfCellsOnX * CellWidth - 1), (int)EndingYOnCameraView);

				//Horizontal lines
				SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView), (int)EndingXOnCameraView, (int)(StartingYOnCameraView));
				for (int j = 1; j < m_NumberOfCellsOnY; j++)
				{
					SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight - 1), (int)EndingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight - 1));
					SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight), (int)EndingXOnCameraView, (int)(StartingYOnCameraView + j * CellHeight));
				}
				SDL_RenderDrawLine(renderer, (int)StartingXOnCameraView, (int)(StartingYOnCameraView + m_NumberOfCellsOnY * CellHeight - 1), (int)EndingXOnCameraView, (int)(StartingYOnCameraView + m_NumberOfCellsOnY * CellHeight - 1));
			}
			//returning to background colour
			SDL_SetRenderDrawColor(renderer, backgroundcolour.red, backgroundcolour.green, backgroundcolour.blue, backgroundcolour.alfa);
		}		
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, m_GridRepository);
	}


	// FUNCTIONS
	virtual void _Init(std::vector<CellGridComponent> &repo,  int cellsonx, int cellsony, int cellwidth, int cellheight, int offsetx, int offsety)
	{
		if (this != nullptr)
		{	
			// (2) REPOSITORY
			m_GridRepository = &repo;


			// (1) DATA
			m_NumberOfCellsOnX = cellsonx;
			m_NumberOfCellsOnY = cellsony;
			m_CellDimensions = std::make_pair(cellwidth, cellheight);
			m_OffsetFromOrigin = std::make_pair(offsetx, offsety);
			m_bShowGrid = true;
			m_GridColour = mfk::ColourList::WHITE_OPAQUE;
			m_X = 0;
			m_Y = 0;	
		}
	}


protected:
	// REPOSITORY
	std::vector<CellGridComponent>* m_GridRepository;
	// DATA
	float m_X;
	float m_Y;
	int m_NumberOfCellsOnX;
	int m_NumberOfCellsOnY;
	std::pair<int, int> m_CellDimensions;
	std::pair<int, int> m_OffsetFromOrigin;

	mfk::ColourList m_GridColour;

public:
	bool m_bShowGrid;

	// FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
	friend class RenderSystem;
};

