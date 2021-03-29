#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Entity.h"

struct SDL_Texture;
class UI_Element;
class Image;
class Label;
class CheckBox;
class Button;
class Slider;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



	void CreatePauseMenu();
	void PauseMenu(float dt);
	void TutorialCoin(float dt);
	void SetTutorialCoin();
	void CoinsLogic();


public:
	int KnowMap = 0; // KnowMap == 0 Map1 is working // KnowMap == 1 Map2 is working  
	p2List<p2SString*> map_name;
	p2SString Song;
	p2SString SongMenu;
	//Entity* player = nullptr;
	bool CanStart = false;
	Entity* bat1 = nullptr;
	bool WantToSpawnEnemies = true;
	bool TexCharged = false;
	bool GamePaused = false;
	bool InPause = false;
	bool CoinPause = false;
	int coins4life = 0;
	//MENU PAUSA

	Image* imagePAUSE = nullptr;
	Button* buttonRESUME = nullptr;
	Button* buttonGOMAINMENU = nullptr;
	Button* buttonSAVE = nullptr;
	Slider* sliderVOLUMEMUSIC = nullptr;
	Label* labelMUSICVOLUME = nullptr;
	Slider* sliderVOLUMEFX = nullptr;
	Label* labelPAUSE = nullptr;
	Label* labelVOLUMEFX = nullptr;
	Slider* sliderGENERALSOUND = nullptr;
	Label* labelGENERALSOUND = nullptr;
	CheckBox* checkboxSOUND = nullptr;
	Label* labelSOUND = nullptr;
	Image* image = nullptr;
	Image* imageNUMBER3 = nullptr;
	Image* imageNUMBER2 = nullptr;
	Image* imageNUMBER1 = nullptr;

	int Delay = 0;
	int Paused = 0;

	int NumberCoins = 0;

	int Number = 3;
	bool StartTimer = false;
	int StartTime = 0;
	bool Positioned = false;
	bool MenuDone = false;

};

#endif // __j1SCENE_H__