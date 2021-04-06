#include "Bat.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Collision.h"
#include "EntityManager.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "Player.h"
#include "p2DynArray.h"
#include "j1Map.h"
#include "Player.h"
#include "..\..\Profiler.h"

Bat::Bat(int x, int y) : Entity(x, y)
{
	position.x = x;
	position.y = y;
	original_pos.x = x;
	original_pos.y = y;
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
	pugi::xml_node		Bat;

	config = App->LoadConfig(config_file);
	app_config = config.child("entity_manager").child("enemies");
	Bat = app_config.child("Bat");

	sprites = Bat.child("sprite").text().as_string();
	range = Bat.child("range").attribute("value").as_int();
	gravity = Bat.child("gravity").attribute("value").as_float();
	GoLeft = LoadPushbacks(Bat, "GoLeft");
	GoRight = LoadPushbacks(Bat, "GoRight");
	Speed = Bat.child("speed").attribute("value").as_float();
	Idle = LoadPushbacks(Bat, "Idle");
	DieLeft = LoadPushbacks(Bat, "DieLeft");
	DieRight = LoadPushbacks(Bat, "DieRight");
	HitLeft = LoadPushbacks(Bat, "HitLeft");
	HitRight = LoadPushbacks(Bat, "HitRight");
	current_animation = &Idle;
	texture = App->tex->Load(sprites.GetString());

}

Bat::~Bat()
{
}

bool Bat::PreUpdate()
{
	PROFILER_FUNCTION();


	return true;
}

bool Bat::PostUpdate()
{
	PROFILER_FUNCTION();
	if (coll != nullptr) {
		if (coll->CanBeDeleted) {
			coll->CanBeDeleted = false;
			coll->type = COLLIDER_PLAYER;
			death = true;
		}
	}
	
	if (death) {
		if (position.x < App->render->camera.x)
			App->entitymanager->DeleteEntity(this);
	}

	return true;
}

bool Bat::Update(float dt)
{
	PROFILER_FUNCTION();
	if (!App->scene->GamePaused) {
		AnimationLogic();

		float x = App->entitymanager->GetPlayerData()->position.x;
		float y = App->entitymanager->GetPlayerData()->position.y;

		if (position.x - x < 400 && x - position.x < 400 && !death) {
			iPoint origin = App->map->WorldToMap(position.x, position.y);
			iPoint destination = App->map->WorldToMap(App->entitymanager->GetPlayerData()->position.x, App->entitymanager->GetPlayerData()->position.y - App->entitymanager->GetPlayerData()->coll->rect.h);
			if (position.DistanceTo(App->entitymanager->GetPlayerData()->position)) {
				App->pathfinding->CreatePath(origin, destination);
				const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
				for (int i = 0; i < path->Count(); i++) {
					PATH.PushBack(*path->At(i));
				}
			}
			if (PATH.Count() > 1) {
				velocity.x = -Speed;
				velocity.y = Speed;
			}
			if (App->entitymanager->GetPlayerData()->position.x < position.x) {
				position.x += velocity.x * dt;
			}
			else {
				position.x += -velocity.x * dt;
			}
			if (App->entitymanager->GetPlayerData()->position.y > position.y) {
				position.y += velocity.y * dt;
			}
			if (App->entitymanager->GetPlayerData()->position.y < position.y) {
				position.y -= velocity.y * dt;
			}
		}
		else {
			iPoint origin = App->map->WorldToMap(position.x, position.y);
			iPoint destination = App->map->WorldToMap(original_pos.x, original_pos.y);
			x = original_pos.x;
			y = original_pos.y;
			fPoint originalpos{ x,y };
			if (position.DistanceTo(originalpos)) {
				App->pathfinding->CreatePath(origin, destination);
				const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
				for (int i = 0; i < path->Count(); i++) {
					PATH.PushBack(*path->At(i));
				}
			}
			if (PATH.Count() > 1) {
				velocity.x = -Speed;
				velocity.y = Speed;
			}
			if (original_pos.x < position.x) {
				position.x += velocity.x * dt;
			}
			else {
				position.x += -velocity.x * dt;
			}
			if (original_pos.y > position.y) {
				position.y += velocity.y * dt;
			}
			if (original_pos.y < position.y) {
				position.y -= velocity.y * dt;
			}
		}
		if (death) {
			position.y += gravity * dt;
		}
		if (position.x + range < -App->render->camera.x)
			App->entitymanager->DeleteEntity(this);
	}
	

	return true;
}

bool Bat::Load(pugi::xml_node & Bat)
{
	return true;
}

bool Bat::Save(pugi::xml_node & Bat) const
{

	Bat.append_child("position").append_attribute("x") = position.x;
	Bat.child("position").append_attribute("y") = position.y;

	return true;
}

void Bat::Draw(float dt)
{
	if (!App->scene->GamePaused) {
		if (current_animation == &DieLeft || current_animation == &DieRight)
			App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_VERTICAL);
		else if (!death)
			App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));
	}
	else App->render->Blit(texture, position.x, position.y, &(current_animation->frames[current_animation->SeeCurrentFrame()]));
	if (firsttime) {
		firsttime = false;
		coll = App->collision->AddCollider({ 0,0,70,47 }, COLLIDER_ENEMY_BAT, (j1Module*)App->entitymanager);
	}
		
	coll->SetPos(position.x, position.y);
}

bool Bat::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void Bat::OnCollision(Collider * c2)
{
	switch (c2->type) {
	case COLLIDER_PARTICLE:
		coll->CanBeDeleted = true;
		break;	
	}
}

Animation Bat::LoadPushbacks(pugi::xml_node &config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	XML_Name_Player_Anims = "AnimationsBat";

	for (pugi::xml_node frames = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).child("frame"); frames; frames = frames.next_sibling("frame")) {
		rect.x = frames.attribute("x").as_int();
		rect.y = frames.attribute("y").as_int();
		rect.w = frames.attribute("w").as_int();
		rect.h = frames.attribute("h").as_int();
		anim.PushBack({ rect.x,rect.y,rect.w,rect.h });
	}
	anim.speed = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("speed").as_float();
	anim.loop = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("loop").as_bool();

	return anim;
}


void Bat::AnimationLogic() {
	if (App->entitymanager->GetPlayerData()->position.x + 5 <= position.x&&!death) {
		current_animation = &GoLeft;
	}
	if (App->entitymanager->GetPlayerData()->position.x > position.x&&!death) {
		current_animation = &GoRight;
	}
	if (death) {
		if (current_animation == &HitLeft || current_animation == &GoLeft)
			current_animation = &DieLeft;
		if (current_animation == &HitRight || current_animation == &GoRight)
			current_animation = &DieRight;
	}
}