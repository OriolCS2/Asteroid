#include "MovingPlatform.h"
#include "Spider.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1PerfTimer.h"
#include "EntityManager.h"
#include "Player.h"
#include "..\..\Asteroid.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

MovingPlatform::MovingPlatform(int x, int y)
{
	position.x = x;
	position.y = y;
	original_pos.x = x;
	original_pos.y = y;


	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
	pugi::xml_node		MovingPlatform;

	config = App->LoadConfig(config_file);
	app_config = config.child("entity_manager").child("Platforms");
	MovingPlatform = app_config.child("MovingPlatform");
	sprites = MovingPlatform.child("Sprite").text().as_string();
	Idle = LoadPushbacks(MovingPlatform, "Animation");
	current_animation = &Idle;
	texture = App->tex->Load(sprites.GetString());

}

MovingPlatform::~MovingPlatform()
{
}

bool MovingPlatform::Update(float dt)
{
		if (Start) {
			Start = false;
			StartTime = SDL_GetTicks();
			MoveRight = true;
		}
		if (MoveRight) {
			position.x += Speed * dt;
			if (SDL_GetTicks() - StartTime > 2300) {
				App->entitymanager->GetPlayerData()->PlayerLeft = true;
				App->entitymanager->GetPlayerData()->PlayerRight = false;
				MoveRight = false;
				MoveLeft = true;
				StartTime = SDL_GetTicks();
			}
		}
		if (MoveLeft) {
			position.x -= Speed * dt;
			if (SDL_GetTicks() - StartTime > 2275) {
				App->entitymanager->GetPlayerData()->PlayerLeft = false;
				App->entitymanager->GetPlayerData()->PlayerRight = true;
				MoveRight = true;
				MoveLeft = false;
				StartTime = SDL_GetTicks();
			}
		}



	return true;
}


void MovingPlatform::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));
	if (coll == nullptr)
		coll = App->collision->AddCollider({ 0,0,70,40 }, COLLIDER_MOVINGPLATFORM, (j1Module*)App->entitymanager);
	coll->SetPos(position.x, position.y);

}

bool MovingPlatform::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

Animation MovingPlatform::LoadPushbacks(pugi::xml_node &config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	XML_Name_Player_Anims = "Animation";

	pugi::xml_node frames = config.child(NameAnim.GetString()).child("frame");
		rect.x = frames.attribute("x").as_int();
		rect.y = frames.attribute("y").as_int();
		rect.w = frames.attribute("w").as_int();
		rect.h = frames.attribute("h").as_int();
		anim.PushBack({ rect.x,rect.y,rect.w,rect.h });
	
	anim.speed = config.child(NameAnim.GetString()).attribute("speed").as_float();
	anim.loop = config.child(NameAnim.GetString()).attribute("loop").as_bool();

	return anim;
}

bool MovingPlatform::Load(pugi::xml_node& platform)
{
	App->entitymanager->CreateEntity(MOVING_PLATFORM, 7480, 900);
	App->entitymanager->CreateEntity(MOVING_PLATFORM, 8300, 900);
	return true;
}
bool MovingPlatform::Save(pugi::xml_node& platform) const
{
	platform.append_child("position").append_attribute("x") = position.x;
	platform.child("position").append_attribute("y") = position.y;

	return true;
}


