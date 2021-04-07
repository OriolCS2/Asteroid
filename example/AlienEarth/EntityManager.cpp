#include "j1App.h"
#include "j1Render.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "Spider.h"
#include"MovingPlatform.h"
#include "Coin.h"
#include "Bat.h"

#include "..\..\Asteroid.h"

EntityManager::EntityManager()
{
	name.create("entity_manager");
	
	

	player = CreateEntity(EntityType::PLAYER);
}

// Destructor
EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr) 
			entities[i]->Awake(config);
	}

	return ret;
}

bool EntityManager::Start()
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr)
			entities[i]->Start();
	}


	return ret;
}

bool EntityManager::PreUpdate()
{
	PROFILER_FUNCTION();
	if (ActiveGame) {
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr)
				entities[i]->PreUpdate();
		}
	}

	return true;
}

// Called before render is available
bool EntityManager::Update(float dt)
{
	PROFILER_FUNCTION();
	if (ActiveGame) {
		
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr && entities[i]->type == PLAYER)
				entities[i]->Update(dt);
		}
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr && entities[i]->type != PLAYER)
				entities[i]->Update(dt);
		}
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr && entities[i]->type == PLAYER)
				entities[i]->Draw(dt);
		}
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr && entities[i]->type != PLAYER)
				entities[i]->Draw(dt);
		}
	}
	
	return true;
}

bool EntityManager::PostUpdate()
{
	PROFILER_FUNCTION();
	if (ActiveGame) {
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr)
				entities[i]->PostUpdate();
		}
	}
	return true;
}

void EntityManager::Draw(float dt)
{
}

// Called before quitting
bool EntityManager::CleanUp()
{
	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}

	entities.Clear();

	return true;
}


void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->GetCollider() == c1)
			entities[i]->OnCollision(c2);
}

Entity* EntityManager::CreateEntity(EntityType type, int x, int y)
{
	Entity* ret = nullptr;
	while (ret == nullptr) {
		switch (type) {
		case EntityType::SPIDER: {ret = new Spider(x, y);
			ret->type = SPIDER;
			break;
		}
		case EntityType::BAT: {ret = new Bat(x, y);
			ret->type = BAT;
			break;
		}
		case EntityType::MOVING_PLATFORM: {ret = new MovingPlatform(x, y);
			ret->type = MOVING_PLATFORM;
			break;
		}
		case EntityType::COIN: {
			ret = new Coin(x, y);
			ret->type = COIN;
			break;
		case EntityType::PLAYER: {ret = new Player();
			ret->type = PLAYER;
			break; }
	
		}
		}
		if (ret != nullptr)
			entities.PushBack(ret);
	}
	return ret;
}

void EntityManager::DeleteEntities()
{

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}

	entities.Clear();

}
void EntityManager::DeleteEnemies() {
	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr && entities[i]->type != PLAYER) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}
}

bool EntityManager::DeleteEntity(Entity* entity)
{
	bool ret = true;

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] == entity) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}
	return ret;
}


bool EntityManager::Load(pugi::xml_node& load)
{
	bool ret = true;

	DeleteEnemies();
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type == PLAYER) {
			pugi::xml_node& player = load.child("player");
			entities[i]->Load(player);
		}
	}

	for (pugi::xml_node spider = load.child("spider"); spider; spider = spider.next_sibling("spider")) {
		CreateEntity(EntityType::SPIDER, spider.child("position").attribute("x").as_float(), spider.child("position").attribute("y").as_float() - 80.0f);
	}
	for (pugi::xml_node bat = load.child("bat"); bat; bat = bat.next_sibling("bat")) {
		CreateEntity(EntityType::BAT, bat.child("position").attribute("x").as_float(), bat.child("position").attribute("y").as_float());
	}
	for (pugi::xml_node coin = load.child("coin"); coin; coin = coin.next_sibling("coin")) {
		CreateEntity(EntityType::COIN, coin.child("position").attribute("x").as_float(), coin.child("position").attribute("y").as_float());
	}

	CreateEntity(EntityType::MOVING_PLATFORM, 7556, 900);
	CreateEntity(EntityType::MOVING_PLATFORM, 8342, 900);

	return ret;
}

bool EntityManager::Save(pugi::xml_node& save) const
{
	bool ret = true;
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type == SPIDER) {
			pugi::xml_node& spider = save.append_child("spider");
			entities[i]->Save(spider);
		}	
	}
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type == BAT) {
			pugi::xml_node& bat = save.append_child("bat");
			entities[i]->Save(bat);
		}
	}
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type == COIN) {
			pugi::xml_node& coin = save.append_child("coin");
			entities[i]->Save(coin);
		}
	}
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type == PLAYER) {
			pugi::xml_node& player = save.append_child("player");
			entities[i]->Save(player);
		}
	}

	return ret;
}

Player* EntityManager::GetPlayerData() const {

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}




