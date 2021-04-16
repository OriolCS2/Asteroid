#include "Spider.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Collision.h"
#include "j1PerfTimer.h"
#include "EntityManager.h"
#include "Player.h"
#include "..\Asteroid.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Scene.h"

Spider::Spider(int x, int y) : Entity(x,y)
{
	gravity = -433.333333f;
	position.x = x;
	position.y = y;
	original_pos.x = x;
	original_pos.y = y;
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
	pugi::xml_node		spider;

	config = App->LoadConfig(config_file);
	app_config = config.child("entity_manager").child("enemies");
	spider = app_config.child("Spider");

	sprites = spider.child("sprite").text().as_string();
	range = spider.child("range").attribute("value").as_int();
	SpiderFx = spider.child("SpiderFx").text().as_string();
	Speed = spider.child("speed").attribute("value").as_float();
	GoLeft = LoadPushbacks(spider, "GoLeft");
	GoRight = LoadPushbacks(spider, "GoRight");
	IdleLeft = LoadPushbacks(spider, "IdleLeft");
	IdleRight = LoadPushbacks(spider, "IdleRight");
	DieLeft = LoadPushbacks(spider, "DieLeft");
	DieRight = LoadPushbacks(spider, "DieRight");
	HitLeft = LoadPushbacks(spider, "HitLeft");
	HitRight = LoadPushbacks(spider, "HitRight");
	current_animation = &IdleLeft;
	texture = App->tex->Load(sprites.GetString());
	spiderfx = App->audio->LoadFx(SpiderFx.GetString());
}

Spider::~Spider()
{
}


bool Spider::PreUpdate()
{
	PROFILER_FUNCTION();

	
	return true;
}

bool Spider::PostUpdate()
{
	PROFILER_FUNCTION();
	if (coll != nullptr) {
		if (coll->CanBeDeleted) {
			coll->CanBeDeleted = false;
			coll->to_delete = true;
			death = true;
		}
	}
	
	if (death) {
		if (position.x<App->render->camera.x)
			App->entitymanager->DeleteEntity(this);
	}

	return true;
}

bool Spider::Update(float dt)
{
	PROFILER_FUNCTION();
	if (!App->scene->GamePaused) {
		if (coll == nullptr)
			coll = App->collision->AddCollider({ 0,0,72,53 }, COLLIDER_ENEMY_SPIDER, (j1Module*)App->entitymanager);
		coll->SetPos(position.x, position.y);
		if (!TouchingGround) {
			acceleration.y = gravity * dt;
		}
		else
			acceleration.y = 0;
		//if (death)
			//acceleration.y = 0;
		position.y -= acceleration.y;
		TouchingGround = false;
		AnimationLogic();

		float x = App->entitymanager->GetPlayerData()->position.x;
		float y = App->entitymanager->GetPlayerData()->position.y;
		if (!death) {
			if (position.x - x < range && x - position.x < range) {
				iPoint origin = App->map->WorldToMap(position.x, position.y);
				iPoint destination = App->map->WorldToMap(App->entitymanager->GetPlayerData()->position.x, App->entitymanager->GetPlayerData()->position.y - App->entitymanager->GetPlayerData()->coll->rect.h);
				if (position.DistanceTo(App->entitymanager->GetPlayerData()->position)) {
					App->pathfinding->CreatePath(origin, destination);
					const p2DynArray<iPoint>* entity_path = App->pathfinding->GetLastPath();
					for (int i = 0; i < entity_path->Count(); i++) {
						PATH.PushBack(*entity_path->At(i));
					}
				}
				if (PATH.Count() > 1) {
					velocity.x = -Speed;
				}
				if (App->entitymanager->GetPlayerData()->position.x < position.x) {
					position.x += velocity.x * dt;
				}
				else {
					position.x += -velocity.x * dt;
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
					const p2DynArray<iPoint>* entity_path = App->pathfinding->GetLastPath();
					for (int i = 0; i < entity_path->Count(); i++) {
						PATH.PushBack(*entity_path->At(i));
					}
				}
				if (PATH.Count() > 1) {
					velocity.x = -60;
				}
				if (originalpos.x < position.x) {
					position.x += velocity.x * dt;
				}
				else {
					position.x += -velocity.x * dt;
				}
			}
		}
		if (position.x + 100 < -App->render->camera.x)
			App->entitymanager->DeleteEntity(this);
		
	}
	return true;
}
bool Spider::Load(pugi::xml_node & spider)
{
	return true;
}

bool Spider::Save(pugi::xml_node & spider) const
{

	spider.append_child("position").append_attribute("x") = position.x;
	spider.child("position").append_attribute("y") = position.y;

	return true;
}

void Spider::Draw(float dt)
{
	if (!App->scene->GamePaused)
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));
	else App->render->Blit(texture, position.x, position.y, &(current_animation->frames[current_animation->SeeCurrentFrame()]));
	
}

bool Spider::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void Spider::OnCollision(Collider * c2)
{
	switch (c2->type) {
	case COLLIDER_PARTICLE:
		coll->CanBeDeleted = true;
		break;
	case COLLIDER_GROUND:
		if (position.y < c2->rect.y + c2->rect.h) 
			TouchingGround = true;
		break;
	case COLLIDER_PLATFORM:
		if (position.y + 72 < c2->rect.y)
			TouchingGround = true;
	}
}

Animation Spider::LoadPushbacks(pugi::xml_node &config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	XML_Name_Player_Anims = "AnimationsSpider";

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

void Spider::AnimationLogic() {

	if (App->entitymanager->GetPlayerData()->position.x + 5 <= position.x && !death) {
		current_animation = &GoLeft;
	}
	if (App->entitymanager->GetPlayerData()->position.x > position.x && !death) {
		current_animation = &GoRight;
	}
	if (death) {
		if (current_animation == &HitLeft || current_animation == &GoLeft)
			current_animation = &DieLeft;
		if (current_animation == &HitRight || current_animation == &GoRight)
			current_animation = &DieRight;
	}
}
