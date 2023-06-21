#pragma once

#include "UserExtension.h"
#include "ECS_Framework.h"


// MAPPERS (STATE TO ANIMATION INDEX)
std::unordered_map<int, int> Mapper_MonsterUnit{

std::make_pair(StateSystem::_GenerateTotalState(user::State::IDLE, user::SubState::IDLE_DEFAULT, user::Direction::Facing_RIGHT),1),
std::make_pair(StateSystem::_GenerateTotalState(user::State::IDLE, user::SubState::IDLE_DEFAULT, user::Direction::Facing_LEFT),2),
std::make_pair(StateSystem::_GenerateTotalState(user::State::IDLE, user::SubState::IDLE_DEFAULT, user::Direction::Facing_DOWN),3),
std::make_pair(StateSystem::_GenerateTotalState(user::State::IDLE, user::SubState::IDLE_DEFAULT, user::Direction::Facing_UP),4),

std::make_pair(StateSystem::_GenerateTotalState(user::State::MOVING, user::SubState::MOVING_WALKING, user::Direction::Facing_RIGHT),5),
std::make_pair(StateSystem::_GenerateTotalState(user::State::MOVING, user::SubState::MOVING_WALKING, user::Direction::Facing_LEFT),6),
std::make_pair(StateSystem::_GenerateTotalState(user::State::MOVING, user::SubState::MOVING_WALKING, user::Direction::Facing_DOWN),7),
std::make_pair(StateSystem::_GenerateTotalState(user::State::MOVING, user::SubState::MOVING_WALKING, user::Direction::Facing_UP),8),

std::make_pair(StateSystem::_GenerateTotalState(user::State::TAKING_ACTION, user::SubState::ACTION_ATTACKING, user::Direction::Facing_RIGHT),9),
std::make_pair(StateSystem::_GenerateTotalState(user::State::TAKING_ACTION, user::SubState::ACTION_ATTACKING, user::Direction::Facing_LEFT),10),
std::make_pair(StateSystem::_GenerateTotalState(user::State::TAKING_ACTION, user::SubState::ACTION_ATTACKING, user::Direction::Facing_DOWN),11),
std::make_pair(StateSystem::_GenerateTotalState(user::State::TAKING_ACTION, user::SubState::ACTION_ATTACKING, user::Direction::Facing_UP),12),

std::make_pair(StateSystem::_GenerateTotalState(user::State::DYING, user::SubState::DYING_NORMAL, user::Direction::Facing_RIGHT),13),
std::make_pair(StateSystem::_GenerateTotalState(user::State::DYING, user::SubState::DYING_NORMAL, user::Direction::Facing_LEFT),14),
std::make_pair(StateSystem::_GenerateTotalState(user::State::DYING, user::SubState::DYING_NORMAL, user::Direction::Facing_DOWN),13),
std::make_pair(StateSystem::_GenerateTotalState(user::State::DYING, user::SubState::DYING_NORMAL, user::Direction::Facing_UP),14),

std::make_pair(StateSystem::_GenerateTotalState(user::State::DEAD, user::SubState::DEAD_NORMAL, user::Direction::Facing_RIGHT),15),
std::make_pair(StateSystem::_GenerateTotalState(user::State::DEAD, user::SubState::DEAD_NORMAL, user::Direction::Facing_LEFT),16),
std::make_pair(StateSystem::_GenerateTotalState(user::State::DEAD, user::SubState::DEAD_NORMAL, user::Direction::Facing_DOWN),15),
std::make_pair(StateSystem::_GenerateTotalState(user::State::DEAD, user::SubState::DEAD_NORMAL, user::Direction::Facing_UP),16)
};
std::unordered_map<int, int> Mapper_Mouse;


// FRAMES PER ANIMATION
std::vector<uint8_t> UnitAnimation(17, 4);
std::vector<uint8_t> ExtendedUnitAnimation{
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	8,
	8,
	4,
	4,
	4,
	4,
};
std::vector<uint8_t> MouseAnimation(2, 1);
std::vector<uint8_t> ButtonAnimation(1, 1);
std::vector<uint8_t> HealthBarAnimation(12, 1);


// NEW COMPONENT CLASS NEEDS TO BE INCLUDED IN THE SWITCH
void ComponentsManager::_DeleteComponents(std::unordered_map<user::ComponentType, uint64_t> IDsOfComponentsToBeDeleted)
{
	LegacyComponentsManager::_DeleteComponents(IDsOfComponentsToBeDeleted);


	for (const auto& component : IDsOfComponentsToBeDeleted)
	{
		switch (component.first)
		{	
		case user::ComponentType::TEST:
			RemoveComponent(Components_Test, component.second);
			break;
		case user::ComponentType::CAMERA:
			RemoveComponent(Components_Camera, component.second);
			break;
			// USER-DEFININED
			// USER-DEFININED
			// USER-DEFININED
			// USER-DEFININED
			// USER-DEFININED
			// USER-DEFININED
		}
	}
}

// NEW COMPONENT CLASS NEEDS TO BE UPDATED
void ComponentsManager::UpdateRepositories(uint64_t currenttime)
{
	LegacyComponentsManager::UpdateRepositories(currenttime);

	UpdateComponents(Components_Test, currenttime);
	UpdateComponents(Components_Camera, currenttime);
	// USER-DEFININED
	// USER-DEFININED
	// USER-DEFININED
	// USER-DEFININED
	// USER-DEFININED
	// USER-DEFININED
}

// NEW SYSTEM NEEDS TO BE IMPLEMENTED IN THE CONSTRUCTOR
SystemsManager::SystemsManager(EntitiesManager* entitiesmanager, ComponentsManager* componentsmanager) : LegacySystemsManager(entitiesmanager, componentsmanager)
{
	System_Test = new TestSystem(Manager_Components, Manager_Entities);
	System_Camera = new CameraSystem(Manager_Components, Manager_Entities, System_State);
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
}

// NEW STATES WITH TIMERS NEED TO BE INCLUDED BELOW (CHECKED EVERY FRAME)
void StateSystem::UpdateState_PreviousToCurrent(StateComponent* stateptr)
{
	if (stateptr->m_CurrentSubState == user::SubState::ACTION_ATTACKING && stateptr->m_Time_Current - stateptr->m_Time_StartOfCurrentState >= stateptr->m_Time_Attacking * 1000)
	{
		ReturnToIdle(stateptr);
	}
	else if (stateptr->m_CurrentSubState == user::SubState::ACTION_USING_POTION && stateptr->m_Time_Current - stateptr->m_Time_StartOfCurrentState >= stateptr->m_Time_UsingPotion * 1000)
	{
		ReturnToIdle(stateptr);
	}
	else if (stateptr->m_CurrentSubState == user::SubState::DYING_NORMAL && stateptr->m_Time_Current - stateptr->m_Time_StartOfCurrentState >= stateptr->m_Time_Dying * 1000)
	{
		ChangeCurrentState(stateptr, user::State::DEAD, user::SubState::DEAD_NORMAL, stateptr->m_CurrentDirection);
	}

	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED

	else
	{
		stateptr->m_StateFromPreviousFrame = stateptr->m_CurrentState;
		stateptr->m_SubStateFromPreviousFrame = stateptr->m_CurrentSubState;
		stateptr->m_DirectionFromPreviousFrame = stateptr->m_CurrentDirection;
	}
}

// BLUEPRINTS
UserBlueprints::UserBlueprints(MyFramework* game, ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
	Game = game;
}
UserBlueprints::~UserBlueprints() {}

uint64_t UserBlueprints::Create_MouseCursor()
{
	uint64_t Mouse = Manager_Entities->_CreateEntity();

	if (Mouse != NULL)
	{
		TransformComponent* Mouse_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Mouse), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Mouse_Transform->_Init(0, 0, false, false, Manager_Components->Components_Transform);

		AnimatedSpriteComponent* Mouse_Sprite = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Mouse), Manager_Components->Components_AnimatedSprite, user::ComponentType::ANIMATEDSPRITE1);
		Mouse_Sprite->_Init(Game->Get_Renderer(), Manager_Components->Components_AnimatedSprite, "Assets/Sprites/Sheets/MouseBMP.bmp", MouseAnimation, Mapper_MonsterUnit, 32, 32, 32, 32, 0, 0, Game->Get_CurrentTickInGame(), 0.125f, user::SpriteLayer::MOUSE, false, false);

		CollisionComponent* Mouse_CollisionBox = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Mouse), Manager_Components->Components_Collision, user::ComponentType::COLLISION1);
		Mouse_CollisionBox->_Init(Manager_Components->Components_Collision, Shape::Sphere, mfk::ColourList::GREY_OPAQUE, user::CollisionProfile::OVERLAP_MOUSE, 1, 1, 3, -3, -3);

		StateComponent* Mouse_State = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Mouse), Manager_Components->Components_State, user::ComponentType::STATE);
		Mouse_State->_Init(user::State::IDLE, user::SubState::IDLE_DEFAULT, user::Direction::Facing_RIGHT, Manager_Components->Components_State);


		Manager_Components->_SortSpritesByLayer();
		//Manager_Components->_SortCollisionsByType();
	}
	return Mouse;
}
uint64_t UserBlueprints::Create_Camera(float cameraspeed, float limitsleft, float limitsright, float limitsup, float limitsdown)
{
	uint64_t Camera = Manager_Entities->_CreateEntity();

	if (Camera != NULL)
	{
		TransformComponent* Camera_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Camera), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Camera_Transform->_Init(0, 0, false, false, Manager_Components->Components_Transform);

		CameraComponent* Camera_Camera = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Camera), Manager_Components->Components_Camera, user::ComponentType::CAMERA);
		Camera_Camera->_Init(Manager_Components->Components_Camera, 1.f, 0.25f, 4.0f, 0.5f, cameraspeed, limitsleft, limitsright, limitsup, limitsdown);
	}

	return Camera;
}

uint64_t UserBlueprints::Create_Goblin(float coordinates_x, float coordinates_y, std::string filepath, std::string filepathhealthbar)
{
	uint64_t Goblin = Manager_Entities->_CreateEntity();

	if (Goblin != NULL)
	{
		TransformComponent* Goblin_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Goblin_Transform->_Init(coordinates_x, coordinates_y, true, true, Manager_Components->Components_Transform);

		MovementComponent* Goblin_Movement = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_Movement, user::ComponentType::MOVEMENT);
		Goblin_Movement->_Init(1, 1, 200.f, 200.f, 200.f, 200.f, Manager_Components->Components_Movement);

		StateComponent* Goblin_State = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_State, user::ComponentType::STATE);
		Goblin_State->_Init(user::State::IDLE, user::SubState::IDLE_DEFAULT, user::Direction::Facing_RIGHT, Manager_Components->Components_State);

		AnimatedSpriteComponent* Goblin_Sprite = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_AnimatedSprite, user::ComponentType::ANIMATEDSPRITE1);
		Goblin_Sprite->_Init(Game->Get_Renderer(), Manager_Components->Components_AnimatedSprite, filepath, UnitAnimation, Mapper_MonsterUnit, 16, 16, 48, 48, -24, -32, Game->Get_CurrentTickInGame(), 0.125f, user::SpriteLayer::UNIT, false, true);

		AnimatedSpriteComponent* Goblin_HealthBarSprite = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_AnimatedSprite, user::ComponentType::ANIMATEDSPRITE2);
		Goblin_HealthBarSprite->_Init(Game->Get_Renderer(), Manager_Components->Components_AnimatedSprite, filepathhealthbar, HealthBarAnimation, Mapper_MonsterUnit, 22, 5, 22, 5, -11, -24, Game->Get_CurrentTickInGame(), 0.125f, user::SpriteLayer::UNIT, false, false);

		CollisionComponent* Goblin_CollisionBox = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_Collision, user::ComponentType::COLLISION1);
		Goblin_CollisionBox->_Init(Manager_Components->Components_Collision, Shape::CollisionBox, mfk::ColourList::GREEN_OPAQUE, user::CollisionProfile::BLOCK_ENEMYUNIT, 24, 32, 1, -12, -16);

		ActionComponent* Goblin_Action = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Goblin), Manager_Components->Components_Action, user::ComponentType::ACTION);
		Goblin_Action->_Init(Manager_Components->Components_Action);


		Manager_Components->_SortSpritesByLayer();
		//Manager_Components->_SortCollisionsByType();
	}

	return Goblin;
}
uint64_t UserBlueprints::Create_Grid(float coordinates_x, float coordinates_y, int cellson_x, int cellson_y, int cellwidth, int cellheight)
{
	uint64_t Grid = Manager_Entities->_CreateEntity();

	if (Grid != NULL)
	{
		TransformComponent* Grid_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Grid), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Grid_Transform->_Init(coordinates_x, coordinates_y, true, true, Manager_Components->Components_Transform);

		CellGridComponent* Grid_Grid = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Grid), Manager_Components->Components_CellGrid, user::ComponentType::CELLGRID);
		Grid_Grid->_Init(Manager_Components->Components_CellGrid, cellson_x, cellson_y, cellwidth, cellheight, 0, 0);
	}

	return Grid;
}
uint64_t UserBlueprints::Create_Tile(float coordinates_x, float coordinates_y, std::string filepath)
{
	uint64_t Tile = Manager_Entities->_CreateEntity();

	if (Tile != NULL)
	{
		TransformComponent* Tile_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Tile), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Tile_Transform->_Init(coordinates_x, coordinates_y, true, true, Manager_Components->Components_Transform);

		AnimatedSpriteComponent* Tile_Sprite = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Tile), Manager_Components->Components_AnimatedSprite, user::ComponentType::ANIMATEDSPRITE1);
		Tile_Sprite->_Init(Game->Get_Renderer(), Manager_Components->Components_AnimatedSprite, filepath, UnitAnimation, Mapper_MonsterUnit, 64, 64, 64, 64, 0, 0, Game->Get_CurrentTickInGame(), 0.125f, user::SpriteLayer::TILE_GROUND_CLOSE, false, false);


		Manager_Components->_SortSpritesByLayer();
		//Manager_Components->_SortCollisionsByType();
	}

	return Tile;
}

uint64_t UserBlueprints::Create_Button(float coordinates_x, float coordinates_y, int buttonwwidth, int buttonheight, int spritewidth, int spriteheight, std::string filepath)
{
	uint64_t Button = Manager_Entities->_CreateEntity();

	if (Button != NULL)
	{
		TransformComponent* Button_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Button), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Button_Transform->_Init(coordinates_x, coordinates_y, true, true, Manager_Components->Components_Transform);

		AnimatedSpriteComponent* Button_Sprite = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Button), Manager_Components->Components_AnimatedSprite, user::ComponentType::ANIMATEDSPRITE1);
		Button_Sprite->_Init(Game->Get_Renderer(), Manager_Components->Components_AnimatedSprite, filepath, ButtonAnimation, Mapper_MonsterUnit, spritewidth, spriteheight, buttonwwidth, buttonheight, -buttonwwidth/2, -buttonheight/2, Game->Get_CurrentTickInGame(), 0.125f, user::SpriteLayer::BACKGROUND_CLOSE, false, false);

		CollisionComponent* Button_CollisionBox = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Button), Manager_Components->Components_Collision, user::ComponentType::COLLISION1);
		Button_CollisionBox->_Init(Manager_Components->Components_Collision, Shape::CollisionBox, mfk::ColourList::GREEN_OPAQUE, user::CollisionProfile::BLOCK_STATICOBJECT, buttonwwidth - 10, buttonheight, 1, -(buttonwwidth - 10)/2, -buttonheight/2);
	
		Manager_Components->_SortSpritesByLayer();
	}

	std::cout << "BLUEPRINT GENERATED: " << Button << std::endl;
	
	return Button;
}
uint64_t UserBlueprints::Create_Background(float coordinates_x, float coordinates_y, user::Resolution resolution, std::string incompletefilepath, std::unordered_map<user::Resolution, std::pair<uint16_t, uint16_t>> mapper_resolution)
{
	//calculate actual filepath based on resolution

	std::pair<uint16_t, uint16_t> dimensions = mapper_resolution.at(resolution);
	std::string fullpath = incompletefilepath + std::to_string(dimensions.first) + "x" + std::to_string(dimensions.second) + ".bmp";
	std::cout << fullpath << "\n";

	uint64_t Background = Manager_Entities->_CreateEntity();

	if (Background != NULL)
	{
		TransformComponent* Background_Transform = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Background), Manager_Components->Components_Transform, user::ComponentType::TRANSFORM);
		Background_Transform->_Init(coordinates_x, coordinates_y, true, true, Manager_Components->Components_Transform);

		AnimatedSpriteComponent* Background_Sprite = Manager_Components->_CreateComponent(Manager_Entities->Get_EntityById(Background), Manager_Components->Components_AnimatedSprite, user::ComponentType::ANIMATEDSPRITE1);
		Background_Sprite->_Init(Game->Get_Renderer(), Manager_Components->Components_AnimatedSprite, fullpath, ButtonAnimation, Mapper_MonsterUnit, dimensions.first, dimensions.second, dimensions.first, dimensions.second, -dimensions.first / 2, -dimensions.second / 2, Game->Get_CurrentTickInGame(), 0.125f, user::SpriteLayer::BACKGROUND_FAR, false, false);
	
		Manager_Components->_SortSpritesByLayer();
	}

	return Background;
}





