#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2SString.h"
#include "p2DynArray.h"
#include "p2List.h"

class Button;
class UI_Element;
class Image;
class Label;
class Slider;
class CheckBox;
struct SDL_Texture;

class UI_Manager : public j1Module
{
public:

	UI_Manager();
	~UI_Manager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	void Draw(float dt, SDL_Texture* texture);
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	Button* CreateButton(int x, int y, UI_Element* parent = nullptr, p2SString name = "NONE", int size = 0);
	CheckBox* CreateCheckBox(int x, int y, UI_Element* parent = nullptr);
	Image* CreateImage(int x, int y, bool WantToBeMoved, UI_Element* parent = nullptr);
	Label* CreateLabel(int x, int y, p2SString name, int size, bool CanBeMoved, UI_Element* parent = nullptr);
	Slider* CreateSlider(int x, int y, int SliderPos, UI_Element* parent = nullptr);
	
	void DeleteAllUI();
	bool DeleteUI_Element(UI_Element* element);
	void DeleteAllUIExeptPlayer();

private:

	p2DynArray<UI_Element*> elements;
	SDL_Texture* atlas = nullptr;
	p2SString sprite;

public:
	bool debug_draw = false;
};

#endif
