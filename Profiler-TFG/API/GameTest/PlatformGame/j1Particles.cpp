#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Particles.h"
#include "j1Audio.h"
#include "j1Particles.h"
#include "Player.h"
#include "j1Collision.h"
#include "PugiXml\src\pugiconfig.hpp"
#include "PugiXml\src\pugixml.hpp"
#include "EntityManager.h"

#define MARGIN 20


#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	name.create("particles");
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles");


	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

bool j1Particles::Awake(pugi::xml_node& config) 
{
	Doublejump.anim = LoadPushbacks(config, "DoubleJump");
	smokeBottom.anim = LoadPushbacks(config, "smokeBottom");
	laserL.anim = LoadPushbacks(config, "laserL");
	laserL.speed.x = config.child("laserL").child("speedX").attribute("value").as_int();
	laserL.life = config.child("laserL").child("life").attribute("value").as_int();
	laserR.anim = LoadPushbacks(config, "laserR");
	laserR.speed.x = config.child("laserR").child("speedX").attribute("value").as_int();
	laserR.life = config.child("laserR").child("life").attribute("value").as_int();
	return true;
}

// Update: draw background
bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

			if (p->Update() == false && !App->scene->GamePaused)
			{
				delete p;
				active[i] = nullptr;
			}
			
			else if (SDL_GetTicks() >= p->born)
			{
				if (!App->scene->GamePaused)
					App->render->Blit(App->entitymanager->GetPlayerData()->texture, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(App->entitymanager->GetPlayerData()->DT)));
				else {
					p->born = SDL_GetTicks();
					App->render->Blit(App->entitymanager->GetPlayerData()->texture, p->position.x, p->position.y, &(p->anim.frames[p->anim.SeeCurrentFrame()]));
				}
				if (p->fx_played == false)
				{
					p->fx_played = true;
				}
			}
		
		
	}

	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			if (collider_type != COLLIDER_NONE) {
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(App->entitymanager->GetPlayerData()->DT), collider_type, this);

			}
				
			active[i] = p;
			break;
		}
	}

}

// TODO 5: Make so every time a particle hits a wall it triggers an explosion particle
void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update()
{
	bool ret = true;
	
	
	if (life > 0 && !App->scene->GamePaused)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished() && !App->scene->GamePaused)
			ret = false;
	if (!App->scene->GamePaused) {
		position.x += (speed.x * App->entitymanager->GetPlayerData()->DT);
		position.y += (speed.y * App->entitymanager->GetPlayerData()->DT);
	}
	
	



	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}

Animation j1Particles::LoadPushbacks(pugi::xml_node& config, p2SString NameAnim) const
{
	SDL_Rect rect;
	Animation anim;

	for (pugi::xml_node frames = config.child(NameAnim.GetString()).child("frame"); frames; frames = frames.next_sibling("frame")) {
		rect.x = frames.attribute("x").as_int();
		rect.y = frames.attribute("y").as_int();
		rect.w = frames.attribute("w").as_int();
		rect.h = frames.attribute("h").as_int();
		anim.PushBack({ rect.x,rect.y,rect.w,rect.h });
	}
	anim.loop = config.child(NameAnim.GetString()).attribute("loop").as_bool();
	anim.speed = config.child(NameAnim.GetString()).attribute("speed").as_float();
	

	return anim;
}