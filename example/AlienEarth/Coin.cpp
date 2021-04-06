#include "Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "Player.h"
#include "j1Scene.h"
#include "EntityManager.h"
#include "j1Menu.h"
#include "j1App.h"
#include "j1Audio.h"

Coin::Coin(int x, int y)
{
	position.x = x;
	position.y = y;
	this->x = x;
	this->y = y;
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		coin;


	config = App->LoadConfig(config_file);
	coin = config.child("entity_manager").child("Coin");
	sprites = coin.child("sprites").text().as_string();
	CoinFx = coin.child("CoinFx").text().as_string();
	coinfx = App->audio->LoadFx(CoinFx.GetString());
	texture = App->tex->Load(sprites.GetString());


	Jeff.PushBack({ 0,0,47,47 });
	Jane.PushBack({ 48,0,47,47 });
	Jerry.PushBack({ 96,0,47,47 });

}

Coin::~Coin()
{
}

bool Coin::Update(float dt)
{
	if (App->entitymanager->GetPlayerData()->NumPlayer == 0 && anim != &Jeff) {
		anim = &Jeff;
	}
	if (App->entitymanager->GetPlayerData()->NumPlayer == 1 && anim != &Jane) {
		anim = &Jane;
	}
	if (App->entitymanager->GetPlayerData()->NumPlayer == 2 && anim != &Jerry) {
		anim = &Jerry;
	}
	return true;
}

bool Coin::Load(pugi::xml_node &)
{
	return true;
}

bool Coin::Save(pugi::xml_node &coin) const
{
	coin.append_child("position").append_attribute("x") = position.x;
	coin.child("position").append_attribute("y") = position.y;

	return true;
}

void Coin::Draw(float dt)
{
	if (firsttime) {
		coll = App->collision->AddCollider({ x,y,47,47 }, COLLIDER_COIN, (j1Module*)App->entitymanager);
		firsttime = false;
	}
	if (anim != nullptr)
		App->render->Blit(texture, position.x, position.y, &(anim->GetCurrentFrame(dt)));

}

bool Coin::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void Coin::OnCollision(Collider * c2)
{

	App->scene->NumberCoins++;
	App->entitymanager->GetPlayerData()->CountCoins();
	App->audio->PlayFx(coinfx);
	coll->to_delete = true;
	App->entitymanager->DeleteEntity(this);
	if (App->menu->FirstCoin) {
		App->menu->FirstCoin = false;
		App->scene->GamePaused = true;
		App->scene->CoinPause = true;
		App->scene->SetTutorialCoin();
	}
}
