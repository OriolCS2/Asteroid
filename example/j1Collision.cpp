#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"

#include "..\Asteroid.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL_RIGHT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL_LEFT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_BAT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_SPIDER] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL_UP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_MOVINGPLATFORM] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND_WATER] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WATER] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_COIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CLIMB] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ROPE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PARTICLE] = false;
	matrix[COLLIDER_PARTICLE][COLLIDER_ENEMY_BAT] = true;
	matrix[COLLIDER_PARTICLE][COLLIDER_ENEMY_SPIDER] = true;
	matrix[COLLIDER_PARTICLE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PARTICLE][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PARTICLE][COLLIDER_WALL_RIGHT] = true;
	matrix[COLLIDER_PARTICLE][COLLIDER_WALL_LEFT] = true;

	matrix[COLLIDER_COIN][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_ENEMY_BAT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_ENEMY_BAT] = false;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_WALL_LEFT] = true;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_WALL_RIGHT] = true;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_WALL_UP] = true;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_GROUND] = true;
	matrix[COLLIDER_ENEMY_BAT][COLLIDER_PARTICLE] = true;

	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_ENEMY_SPIDER] = false;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_WALL_LEFT] = true;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_WALL_RIGHT] = true;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_WALL_UP] = true;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_ENEMY_SPIDER][COLLIDER_PARTICLE] = true;

	

}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	ASTEROID_FUNCTION();
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Test all collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	ASTEROID_FUNCTION();
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;
	DebugDraw();


	return true;
}

void j1Collision::DebugDraw()
{

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL_RIGHT: // GREEN
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_WALL_LEFT: // GREEN
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER: // BLUE
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha,true);
			break;
		case COLLIDER_GROUND: // WHITE
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLATFORM: // WHITE
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_MOVINGPLATFORM: // WHITE
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_SPIKES: // RED
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WATER: // BLUEGREEn
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_FALL: // RED
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WIN: // BLACK
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha);
			break;
		case COLLIDER_CLIMB:
			App->render->DrawQuad(colliders[i]->rect, 0, 130, 130, alpha);
			break;
		case COLLIDER_ROPE:
			App->render->DrawQuad(colliders[i]->rect, 0, 130, 130, alpha);
			break;
		case COLLIDER_PARTICLE:
			App->render->DrawQuad(colliders[i]->rect, 150, 150, 150, alpha);
			break;
		case COLLIDER_ENEMY_BAT:
			App->render->DrawQuad(colliders[i]->rect, 155, 155, 155, alpha);
			break;
		case COLLIDER_ENEMY_SPIDER:
			App->render->DrawQuad(colliders[i]->rect, 155, 155, 155, alpha);
			break;
		case COLLIDER_COIN:
			App->render->DrawQuad(colliders[i]->rect, 255, 155, 155, alpha);
			break;
		}
		
	}
	
}

// Called before quitting
bool j1Collision::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}
bool j1Collision::ColliderCleanUp()
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && (colliders[i]->type != COLLIDER_PLAYER))
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
bool j1Collision::ColliderCleanUpPlayer()
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type == COLLIDER_PLAYER)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}



// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}