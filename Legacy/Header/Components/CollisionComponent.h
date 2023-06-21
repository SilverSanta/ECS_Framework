#pragma once

#include "SDL.h"
#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;
class ComponentsManager;

// SHAPE ENUM
enum class Shape
{
	CollisionBox,
	Sphere,
};

// SPHERE CLASS
struct Sphere {

	Sphere()
	{
		X = 0;
		Y = 0;
		R = 1;
		Chords = 8;
	}
	Sphere(int x, int y, int r)
	{
		X = x;
		Y = y;
		R = r;
		UpdateChords(R);
	}

	void Update()
	{
		UpdatePointCoordinates();
	}
	void UpdatePointCoordinates() {

		int xc = (int)(X + R);
		int yc = (int)(Y + R);


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

	int X;
	int Y;
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
			CollisionSphere.Update();
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
			mfk::ColourTable colourtable = mfk::Colour::Get_ColourTable(ShapeColour);
			SDL_SetRenderDrawColor(renderer, colourtable.red, colourtable.green, colourtable.blue, colourtable.alfa);


			if (CollisionShape == Shape::CollisionBox)
			{
				SDL_RenderDrawRect(renderer, &CollisionBox);
			}
			else
			{
				CollisionSphere.DrawSphere(renderer);
			}

			//returning to background colour
			SDL_SetRenderDrawColor(renderer, backgroundcolour.red, backgroundcolour.green, backgroundcolour.blue, backgroundcolour.alfa);
		}
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, CollisionRepository);
	}
public:
	virtual void _Init(std::vector<CollisionComponent> &repo, Shape shape, mfk::ColourList colour, user::CollisionProfile profile, int boxwidth, int boxheight, int sphereradius, int offsetx, int offsety)
	{
		//guard
		if (this != nullptr)
		{
			// (1) REPOSITORY
			CollisionRepository = &repo;

			// (2) COLLISION
			bShowShape = true;
			ShapeColour = colour;
			CollisionShape = shape;
			Profile = profile;
			CollisionBox.w = boxwidth;
			CollisionBox.h = boxheight;
			CollisionSphere.R = sphereradius;
			CollisionSphere.UpdateChords(CollisionSphere.R);
			OffsetFromOrigin = std::make_pair(offsetx, offsety);
		}		
	}


protected:
	// REPOSITORY
	std::vector<CollisionComponent>* CollisionRepository;
	// DATA
	std::pair<int, int> OffsetFromOrigin;
	Shape CollisionShape;
	SDL_Rect CollisionBox;
	Sphere CollisionSphere;
	bool bShowShape;
	mfk::ColourList ShapeColour;
	user::CollisionProfile Profile;


	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
	friend class RenderSystem;
	friend class CollisionSystem;
};