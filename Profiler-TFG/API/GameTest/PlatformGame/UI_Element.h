#ifndef __UI_Element_H__
#define __UI_Element_H__


#include "p2Point.h"
#include "p2Animation.h"
#include "p2DynArray.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"



class Image;
struct SDL_Texture;
enum UI_Elements
{
	NON_TYPE,

	BUTTON,
	CHECKBOX,
	IMAGE,
	LABEL,
	SLIDER,
	PLAYERUIIMAGE,
	PLAYERUILABEL

};


class UI_Element
{
public:

	UI_Element();
	UI_Element(int x, int y, UI_Element* parent = nullptr);
	virtual ~UI_Element();
	
	virtual bool Update(float dt);
	virtual bool CleanUp();
	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual void Draw(float dt, SDL_Texture* texture);
	bool IsMouseOn();
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };
	void SetPos(int x, int y);
	void SetSpritesData(SDL_Rect Idle, SDL_Rect Hover = { 0,0,0,0 }, SDL_Rect Pressed = { 0,0,0,0 });

public:
	
	iPoint Local_pos;
	iPoint Scree_pos;
	UI_Element* parent = nullptr;
	int height;
	int width;
	SDL_Rect png_pos = { 0,0,0,0 };
	UI_Elements type = UI_Elements::NON_TYPE;
	
	pugi::xml_node	UI_node;
	SDL_Texture * tex = nullptr;
	p2DynArray<UI_Element*> Son;

	bool CanBeMoved = true;
	bool pressed = false;
	bool mouseOn = false;
	iPoint mouse;
	unsigned int fxOn;
	unsigned int fxPressed;

	p2SString FXON;
	p2SString FXPRESSED;
	//
	SDL_Rect NoPressedNoMouseOn;
	SDL_Rect MouseOn;
	SDL_Rect Pressed;
	bool repeataudio = true;
	bool WantToRender = true;
	bool NoUse = false;
	

};

#endif

