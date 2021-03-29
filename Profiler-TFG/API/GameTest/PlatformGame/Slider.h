#ifndef __SLIDER_H__
#define __SLIDER_H__

#define MARGIN 7

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Slider : public UI_Element {
public:

	Slider(int x, int y, int SliderPos, UI_Element* parent = nullptr);
	virtual ~Slider();
	bool Update(float dt);
	void LookLimits();
	float Value = -1;
	Image * image = nullptr;
private:

	bool repeat2 = true;
	int distance;
	iPoint mouse_pos;
};

#endif