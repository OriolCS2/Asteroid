#ifndef __J1MENU_H__
#define __J1MENU_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"

class Image;
struct SDL_Texture;
class CheckBox;
struct SDL_Rect;
class Slider;
class Label;
class Button;
class UI_Element;

class j1Menu : public j1Module
{
public:

	j1Menu();

	// Destructor
	virtual ~j1Menu();

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

	Animation LoadGigantAliensAnimations(int playernumber, pugi::xml_node&, p2SString NameAnim) const;
	void CreateButtonsTypePlayer();
	void StartLevel();
	void MenuChoosePlayer(float dt);
	void MainMenu();
	void CreateMainMenu();
	void SettingsMenu(float dt);
	void CreateSettingsButtons();
	void CreateIntro();
	void WantToDisappearMainMenu(bool Disappear);
	void WantToDisappearButtonsTypePlayer(bool Disappear);
	void HacksMenu(float dt);
	void CreatehacksButtons();
	void CreateCredits();
	void Credits(float dt);


public:

	SDL_Texture * yellow = nullptr;
	Animation YellowStand;
	Animation YellowWalk;

	SDL_Texture * pink = nullptr;
	Animation PinkStand;
	Animation PinkWalk;

	SDL_Texture * blue = nullptr;
	Animation BlueStand;
	Animation BlueWalk;

	SDL_Texture* Choose = nullptr;
	p2SString ChooseFx;
	p2SString IntroFx;

	bool start = false;
	SDL_Texture * ScreenStart = nullptr;
	p2SString file_texture;
	iPoint mouse;
	int playernumber = -1;
	int MinY_ChooseRect = 0;
	int MaxY_ChooseRect = 0;
	int MinX_RectChoosePlayer1 = 0;
	int MaxX_RectChoosePlayer1 = 0;
	int MinX_RectChoosePlayer2 = 0;
	int MaxX_RectChoosePlayer2 = 0;
	int MinX_RectChoosePlayer3 = 0;
	int MaxX_RectChoosePlayer3 = 0;
	int PlayerNumber1 = 0;
	int PlayerNumber2 = 0;
	int PlayerNumber3 = 0;
	bool GameOn = false;
	bool repeat = false;
	bool repeat2 = false;
	bool repeat3 = false;
	bool FirstUpdate = true;
	bool GoStart = false;
	bool GoStartSaved = false;
	bool StartChoosing = false;
	bool InMainMenu = true;
	bool AlreadyChoosen = false;
	bool Exit = false;
	bool InSettings = false;
	bool CanLoad = true;
	bool Positioned = false;
	bool SettingMenuDone = false;
	bool InHacks = false;
	bool positioned = false;
	bool HacksMenuDone = false;
	bool InCredits = false;

	bool FirstCoin = true;

	unsigned int choosefx;
	unsigned int introfx;

	//START MENU
	Image* Background = nullptr;
	Image* Title = nullptr;
	Image* sentence = nullptr;

	//INITIAL MENU
	Image* MainTitle = nullptr;
	Button* buttonSTART = nullptr;
	Button* buttonCONTINUE = nullptr;
	Button* buttonSETTINGS = nullptr;
	Button* buttonHACKS = nullptr;
	Button* buttonCREDITS = nullptr;
	Button* buttonEXIT = nullptr;


	//CHOOSING PLAYER
	Button* buttonJEFF = nullptr;
	Button* buttonJANE = nullptr;
	Button* buttonJERRY = nullptr;
	Button* buttonGOBACK = nullptr;
	Image* CHOOSE = nullptr;
	Image* JEFFNAME = nullptr;
	Image* JANENAME = nullptr;
	Image* JERRYNAME = nullptr;

	//SETTINGS
	Button* buttonGOBACKSETTINGS = nullptr;
	Image* imageSETTINGS = nullptr;
	CheckBox* checkboxFPS = nullptr;
	Label* labelFPS = nullptr;
	SDL_Texture* Settings = nullptr;
	Slider* sliderVOLUMEMUSIC = nullptr;
	Label* labelMUSICVOLUME = nullptr;
	Slider* sliderVOLUMEFX = nullptr;
	Label* labelSETTINGS = nullptr;
	Label* labelVOLUMEFX = nullptr;
	Label* labelGODMODE = nullptr;
	CheckBox* checkboxGODMODE = nullptr;
	Slider* sliderGENERALSOUND = nullptr;
	Label* labelGENERALSOUND = nullptr;
	CheckBox* checkboxSOUND = nullptr;
	Label* labelSOUND = nullptr;
	int x = 170;
	int y = 950;
	

	//HACKS
	Label* labelHACKS = nullptr;
	Button* buttonGOBACKHACKS = nullptr;
	Image* imageHACKS = nullptr;
	CheckBox* checkboxSTARTLEVEL2 = nullptr;
	CheckBox* checkboxNOENEMIES = nullptr;
	Label* labelSTARTLEVEL2 = nullptr;
	Label* labelNOENEMIES = nullptr;
	CheckBox* checkboxCANTLOSELIFE = nullptr;
	Label* labelCANTLOSELIFE = nullptr;



	//CREDITS
	Image* CreditsTitle = nullptr;
	Image* imageCREDITS = nullptr;	
	Button* buttonGOBACKCREDITS = nullptr;
	Button* buttonGITHUB = nullptr;
	Button* buttonWEBSITE = nullptr;
	Button* buttonORIOLGIT = nullptr;
	Button* buttonVICTORGIT = nullptr;
	Image* AlienVictor = nullptr;
	Image* AlienOriol = nullptr;

	//PLAYER UI

	Image* image = nullptr;
	Label* label = nullptr;

	// 
	

};

#endif //
