#ifndef __j1Collision_H__
#define __j1Collision_H__

#define MAX_COLLIDERS 500

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE,
	COLLIDER_PARTICLE,
	COLLIDER_WALL_RIGHT,
	COLLIDER_WALL_LEFT,
	COLLIDER_PLAYER,
	COLLIDER_MOVINGPLATFORM,
	COLLIDER_GROUND,
	COLLIDER_GROUND_WATER,
	COLLIDER_PLATFORM,
	COLLIDER_WALL_UP,
	COLLIDER_FALL,
	COLLIDER_WATER,
	COLLIDER_SPIKES,
	COLLIDER_WIN,
	COLLIDER_CLIMB,
	COLLIDER_ROPE,
	COLLIDER_ENEMY_BAT,
	COLLIDER_ENEMY_SPIDER,
	COLLIDER_COIN,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	bool CanBeDeleted = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	bool enable = true;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void SetShape(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	void SetType(COLLIDER_TYPE type)
	{
		this->type = type;
	}
	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision :
	public j1Module
{
public:
	j1Collision();
	~j1Collision();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	bool ColliderCleanUp();
	bool ColliderCleanUpPlayer();
	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	
	void DebugDraw();
	bool debug = false;

private:

	Collider * colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	
};

#endif // __ModuleCollision_H__
