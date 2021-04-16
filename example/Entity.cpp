#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "..\Asteroid.h"
#include "j1Map.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"
#include "Player.h"

Entity::Entity()
{
}

Entity::Entity(int x, int y) : position(x, y)
{
}

Entity::~Entity()
{

}

const Collider * Entity::GetCollider() const
{
	return coll;
}

bool Entity::Update(float dt)
{
	return true;
}

void Entity::Draw(float dt)
{
	

}

void Entity::OnCollision(Collider* c2)
{
	
}




