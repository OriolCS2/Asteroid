#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "Button.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "UI_Manager.h"
#include "Image.h"
#include "UI_Element.h"
#include "j1Scene.h"
#include "Player.h"
#include "CheckBox.h"
#include "Entity.h"
#include "Label.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Menu.h"
#include "Slider.h"
#include "j1FadeToBlack.h"

#include "..\..\Asteroid.h"

j1Scene::j1Scene() : j1Module()
{
	//App->scene->Disable();
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	
	for (pugi::xml_node map = config.child("mapname"); map; map = map.next_sibling("mapname")) {
		p2SString * data = new p2SString;
		data->create(map.attribute("name").as_string());
		map_name.add(data);
	}
	Song = config.child("song").text().as_string();
	SongMenu = config.child("songmenu").text().as_string();
	coins4life=config.child("coins4life").attribute("value").as_int();
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	PROFILER_FUNCTION();
	//App->map->Load(map_name.start->data->GetString());
	//active = false;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	PROFILER_FUNCTION();
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	PROFILER_FUNCTION();
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml", true);

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	
	if (App->entitymanager->GetPlayerData()->NextMap) {
		App->entitymanager->DeleteEnemies();
		App->entitymanager->GetPlayerData()->NextMap = false;
		KnowMap = 1;
		App->ui_manager->DeleteUI_Element(App->entitymanager->GetPlayerData()->tutorial);
		App->map->ChangeMap(map_name[KnowMap]);
		App->map->LoadEnemies();
		App->map->LoadCoins();
		App->entitymanager->GetPlayerData()->Spawn();
	}
	if (!App->entitymanager->GetPlayerData()->Intro) {
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
			App->render->camera.x = 0;
			App->render->camera.y = 0;
			App->entitymanager->DeleteEnemies();
			KnowMap = 0;
			App->map->ChangeMap(map_name[KnowMap]);
			App->map->LoadEnemies();
			App->map->LoadCoins();
			App->entitymanager->GetPlayerData()->Spawn();
			if (App->entitymanager->GetPlayerData()->tutorial == nullptr)
				App->entitymanager->GetPlayerData()->SetTutorials();
		}
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			if (KnowMap == 0) {
				App->render->camera.x = 0;
				App->render->camera.y = 0;
				if (App->entitymanager->GetPlayerData()->tutorial == nullptr)
					App->entitymanager->GetPlayerData()->SetTutorials();
				App->entitymanager->DeleteEnemies();
				App->map->ChangeMap(map_name[KnowMap]);
				App->map->LoadEnemies();
				App->map->LoadCoins();
				App->entitymanager->GetPlayerData()->Spawn();
			}
			else if (KnowMap == 1) {
				App->ui_manager->DeleteUI_Element(App->entitymanager->GetPlayerData()->tutorial);
				App->entitymanager->DeleteEnemies();
				App->map->ChangeMap(map_name[KnowMap]);
				App->map->LoadEnemies();
				App->map->LoadCoins();
				App->entitymanager->GetPlayerData()->Spawn();
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
			App->entitymanager->DeleteEnemies();
			KnowMap = 1;
			App->ui_manager->DeleteUI_Element(App->entitymanager->GetPlayerData()->tutorial);
			App->map->ChangeMap(map_name[KnowMap]);
			App->map->LoadEnemies();
			App->map->LoadCoins();
			App->entitymanager->GetPlayerData()->Spawn();
		}
		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			Delay = 0;
			App->fade->FadeToBlack(3.0f);
			CanStart = true;
		}
	}

	CoinsLogic();
	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !GamePaused) {
		Paused = SDL_GetTicks();
		CreatePauseMenu();
		GamePaused = true;
		InPause = true;
	}
	if (InPause) {
		PauseMenu(dt);
	}
	if (CoinPause) {
		TutorialCoin(dt);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	PROFILER_FUNCTION();
	bool ret = true;

	if (CanStart && App->fade->current_step == App->fade->fade_from_black) {
		App->entitymanager->GetPlayerData()->lifes = 3;
		CanStart = false;
		App->entitymanager->DeleteEnemies();
		App->entitymanager->ActiveGame = false;
		App->scene->active = false;
		App->collision->active = false;
		App->map->active = false;
		App->menu->start = false;
		App->menu->GameOn = false;
		App->entitymanager->GetPlayerData()->Intro = true;
		App->entitymanager->GetPlayerData()->NoInput = false;
		App->menu->Start();
		KnowMap = 0;
		GamePaused = false;
		InPause = false;
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	//App->collision->Disable();
	LOG("Freeing scene");
	return true;
}

bool j1Scene::Load(pugi::xml_node & scene)
{
	KnowMap = scene.child("KnowMap").attribute("value").as_int();
	//GamePaused = scene.child("GamePaused").attribute("value").as_bool();
	App->menu->FirstCoin = scene.child("FirstCoin").attribute("value").as_bool();
	return true;
}

bool j1Scene::Save(pugi::xml_node & scene) const
{
	scene.append_child("KnowMap").append_attribute("value") = KnowMap;
	//scene.append_child("GamePaused").append_attribute("value") = GamePaused;
	scene.append_child("FirstCoin").append_attribute("value") = App->menu->FirstCoin;

	return true;
}



void j1Scene::CreatePauseMenu()
{
	
	App->render->camera.x = 0;
	image = App->ui_manager->CreateImage(0, 0, false);
	image->SetSpritesData({ 0,1158,1024,768 });
	imagePAUSE = App->ui_manager->CreateImage(170, 60, true);
	imagePAUSE->SetSpritesData({ 758,0,705,671 });
	buttonRESUME = App->ui_manager->CreateButton(120, 120, imagePAUSE, "RESUME", 30);
	buttonGOMAINMENU = App->ui_manager->CreateButton(400, 120, imagePAUSE, "RETURN MAIN MENU", 30);
	sliderVOLUMEMUSIC = App->ui_manager->CreateSlider(380, 463, App->audio->volume, imagePAUSE);
	labelMUSICVOLUME = App->ui_manager->CreateLabel(100, 450, "MUSIC VOLUME", 50, true, imagePAUSE);
	sliderVOLUMEFX = App->ui_manager->CreateSlider(380, 563, App->audio->fxvolume, imagePAUSE);
	labelVOLUMEFX = App->ui_manager->CreateLabel(100, 550, "FX VOLUME", 50, true, imagePAUSE);
	labelPAUSE = App->ui_manager->CreateLabel(imagePAUSE->width / 2, 50, "PAUSE", 60, true, imagePAUSE);
	labelPAUSE->Local_pos.x -= labelPAUSE->width / 2;
	labelGENERALSOUND = App->ui_manager->CreateLabel(100, 350, "GENERAL SOUND", 50, true, imagePAUSE);
	sliderGENERALSOUND = App->ui_manager->CreateSlider(380, 363, App->audio->general, imagePAUSE);
	checkboxSOUND = App->ui_manager->CreateCheckBox(380, 257, imagePAUSE);
	buttonSAVE = App->ui_manager->CreateButton(260, 190, imagePAUSE,"SAVE GAME",30);
	App->render->camera.y = 0;
	if (!App->audio->NoAudio)
		checkboxSOUND->pressed = true;
	else checkboxSOUND->pressed = false;
	labelSOUND = App->ui_manager->CreateLabel(100, 250, "SOUND", 50, true, imagePAUSE);
}

void j1Scene::PauseMenu(float dt)
{
	
	if (buttonRESUME->pressed) {
		buttonRESUME->pressed = false;
		StartTime = SDL_GetTicks();
		StartTimer = true;
		App->ui_manager->DeleteAllUIExeptPlayer();
		imageNUMBER3 = App->ui_manager->CreateImage(App->win->Width / 2, App->win->Height / 2-100, false);
		imageNUMBER3->type = PLAYERUIIMAGE;
		imageNUMBER3->SetSpritesData({ 1584,1963,29,44 });
		imageNUMBER3->Local_pos.x -= imageNUMBER3->width / 2;
		imageNUMBER3->Local_pos.y -= imageNUMBER3->height / 2;
	}
	if (buttonSAVE->pressed) {
		App->SaveGame("save_game.xml");
	}
	if (buttonGOMAINMENU->pressed) {
		NumberCoins = 0;
		Delay = 0;
		StartTimer = true;
		App->fade->FadeToBlack(3.0f);
		App->ui_manager->DeleteAllUI();
		CanStart = true;
		
	}
	if (checkboxSOUND->pressed && App->audio->NoAudio) {
		App->audio->NoAudio = false;
		sliderGENERALSOUND->NoUse = false;
		sliderVOLUMEMUSIC->NoUse = false;
		sliderVOLUMEFX->NoUse = false;
		App->audio->general = sliderGENERALSOUND->Value;
		App->audio->volume = sliderVOLUMEMUSIC->Value;
		App->audio->fxvolume = sliderVOLUMEFX->Value;
	}
	else if (!checkboxSOUND->pressed && !App->audio->NoAudio){
		App->audio->NoAudio = true;
		sliderGENERALSOUND->NoUse = true;
		sliderVOLUMEMUSIC->NoUse = true;
		sliderVOLUMEFX->NoUse = true;
	}

	if (StartTimer && !CanStart) {
		if (SDL_GetTicks() - StartTime > 1000) {
			Number -= 1;
			StartTime = SDL_GetTicks();
			if (Number == 2) {
				App->ui_manager->DeleteUI_Element(imageNUMBER3);
				imageNUMBER2 = App->ui_manager->CreateImage(App->win->Width / 2, App->win->Height / 2-100, true);
				imageNUMBER2->type = PLAYERUIIMAGE;
				imageNUMBER2->SetSpritesData({ 1552,1963,31,42 });
				imageNUMBER2->Local_pos.x -= imageNUMBER2->width / 2;
				imageNUMBER2->Local_pos.y -= imageNUMBER2->height / 2;
			}
			if (Number == 1) {
				App->ui_manager->DeleteUI_Element(imageNUMBER2);
				imageNUMBER1 = App->ui_manager->CreateImage(App->win->Width / 2, App->win->Height / 2-100, true);
				imageNUMBER1->type = PLAYERUIIMAGE;
				imageNUMBER1->SetSpritesData({ 1524,1963,26,42 });
				imageNUMBER1->Local_pos.x -= imageNUMBER1->width / 2;
				imageNUMBER1->Local_pos.y -= imageNUMBER1->height / 2;
			}
			if (Number == 0) {
				Number = 3;
				App->ui_manager->DeleteUI_Element(imageNUMBER1);
				Delay += SDL_GetTicks() - Paused;
				StartTimer = false;
				GamePaused = false;
				InPause = false;
				
			}
				
			
		}
	}
}

void j1Scene::TutorialCoin(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		CoinPause = false;
		GamePaused = false;
		App->ui_manager->DeleteUI_Element(App->entitymanager->GetPlayerData()->FirstCCoin);
	}

}

void j1Scene::SetTutorialCoin()
{
	App->entitymanager->GetPlayerData()->FirstCCoin = App->ui_manager->CreateImage(App->win->width / 2 - 134, App->win->height / 2 - 52, false);
	App->entitymanager->GetPlayerData()->FirstCCoin->type = PLAYERUIIMAGE;
	if (App->entitymanager->GetPlayerData()->NumPlayer == 0) {
		App->entitymanager->GetPlayerData()->FirstCCoin->SetSpritesData({ 1122,1235,267,104 });
	}
	if (App->entitymanager->GetPlayerData()->NumPlayer == 1) {
		App->entitymanager->GetPlayerData()->FirstCCoin->SetSpritesData({ 1122,1342,267,104 });
	}
	if (App->entitymanager->GetPlayerData()->NumPlayer == 2) {
		App->entitymanager->GetPlayerData()->FirstCCoin->SetSpritesData({ 1122,1449,267,104 });
	}
}

void j1Scene::CoinsLogic()
{
	if (NumberCoins >= coins4life) {
		App->entitymanager->GetPlayerData()->lifes++;
		App->entitymanager->GetPlayerData()->Lifes();
		NumberCoins = 0;
		App->entitymanager->GetPlayerData()->CountCoins();
	}
}

