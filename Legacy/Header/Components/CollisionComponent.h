#pragma once

#include "SDL.h"
#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;
class ComponentsManager;

// SHAPE ENUM
enum class Shape
{
	m_CollisionBox,
	Sphere,
};

// SPHERE CLASS
struct Sphere {

	Sphere()
	{
		m_X = 0;
		m_Y = 0;
		R = 1;
		Chords = 8;
	}
	Sphere(int x, int y, int r)
	{
		m_X = x;
		m_Y = y;
		R = r;
		UpdateChords(R);
	}

	void Update()
	{
		UpdatePointCoordinates();
	}
	void UpdatePointCoordinates() {

		int xc = (int)(m_X + R);
		int yc = (int)(m_Y + R);


		if (ExternalPoints.size() > 0) {

			float AngularDelta = 360.f / (float)ExternalPoints.size();

			for (int i = 0; i < ExternalPoints.size(); i++) {

				float CurrentAngleValueInRadians = (((float)i * AngularDelta) * mfk::Pi / 180.f);

				ExternalPoints.at(i).first = xc + (int)(R * sin(CurrentAngleValueInRadians));
				ExternalPoints.at(i).second = yc + (int)(R * cos(CurrentAngleValueInRadians));
			}
		}
	}
	void RecreatePoints() {

		//(1)Create all points, then UpdatePoints()
		std::vector<std::pair<int, int>> NewSet(Chords, std::make_pair(0, 0));
		ExternalPoints = NewSet;

		//(2)Update all coordinates based on current X, Y, R and Chords
		UpdatePointCoordinates();
	}
	void DrawSphere(SDL_Renderer* renderer) {

		int chords = (int)ExternalPoints.size();

		//(1)Draw lines
		for (int i = 0; i < chords - 1; i++) {
			SDL_RenderDrawLine(
				renderer, ExternalPoints.at(i).first, ExternalPoints.at(i).second,
				ExternalPoints.at(i + 1).first, ExternalPoints.at(i + 1).second);
		}
		SDL_RenderDrawLine(renderer,
			ExternalPoints.at(chords - 1).first, ExternalPoints.at(chords - 1).second,
			ExternalPoints.at(0).first, ExternalPoints.at(0).second);
	}
	void UpdateChords(int radius)
	{
		//update chords
		Chords = (int)(8 + (8 * radius / 25));

		//update points			
		RecreatePoints();
	}

	int m_X;
	int m_Y;
	int R;
	int Chords;
	std::vector<std::pair<int, int>> ExternalPoints;
};

class CollisionComponent : public IComponent
{
	// CONSTRUCTORS
public:
	CollisionComponent()
	{
		//std::cout << "     COLLISION created\n";
	}
	~CollisionComponent(){
	
		//std::cout << "     COLLISION: #" << m_Id << " deleted\n";
	}

	
	//OVERRIDES
public:
	virtual void _Update(uint64_t currentTime) override
	{
		if (this != nullptr)
		{
			//Update rectangle
			

			//Update sphere???
			m_CollisionSphere.Update();
		}	
	}
	virtual void _Render(SDL_Renderer* renderer) override
	{
		if (this != nullptr)
		{
			//storing the background colour
			mfk::ColourTable backgroundcolour;
			SDL_GetRenderDrawColor(renderer, &backgroundcolour.red, &backgroundcolour.green, &backgroundcolour.blue, &backgroundcolour.alfa);

			//Changing draw colour
			mfk::ColourTable colourtable = mfk::Colour::Get_ColourTable(m_ShapeColour);
			SDL_SetRenderDrawColor(renderer, colourtable.red, colourtable.green, colourtable.blue, colourtable.alfa);


			if (m_CollisionShape == Shape::m_CollisionBox)
			{
				SDL_RenderDrawRect(renderer, &m_CollisionBox);
			}
			else
			{
				m_CollisionSphere.DrawSphere(renderer);
			}

			//returning to background colour
			SDL_SetRenderDrawColor(renderer, backgroundcolour.red, backgroundcolour.green, backgroundcolour.blue, backgroundcolour.alfa);
		}
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, m_CollisionRepository);
	}
public:
	virtual void _Init(std::vector<CollisionComponent> &repo, Shape shape, mfk::ColourList colour, user::CollisionProfile profile, int boxwidth, int boxheight, int sphereradius, int offsetx, int offsety)
	{
		//guard
		if (this != nullptr)
		{
			// (1) REPOSITORY
			m_CollisionRepository = &repo;

			// (2) COLLISION
			m_bShowShape = true;
			m_ShapeColour = colour;
			m_CollisionShape = shape;
			m_Profile = profile;
			m_CollisionBox.w = boxwidth;
			m_CollisionBox.h = boxheight;
			m_CollisionSphere.R = sphereradius;
			m_CollisionSphere.UpdateChords(m_CollisionSphere.R);
			m_OffsetFromOrigin = std::make_pair(offsetx, offsety);
		}		
	}


protected:
	// REPOSITORY
	std::vector<CollisionComponent>* m_CollisionRepository;
	// DATA
	std::pair<int, int> m_OffsetFromOrigin;
	Shape m_CollisionShape;
	SDL_Rect m_CollisionBox;
	Sphere m_CollisionSphere;
	bool m_bShowShape;
	mfk::ColourList m_ShapeColour;
	user::CollisionProfile m_Profile;


	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
	friend class RenderSystem;
	friend class CollisionSystem;
};