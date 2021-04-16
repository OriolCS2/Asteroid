#ifndef __LABEL_H__
#define __LABEL_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Label : public UI_Element {
public:

	Label(int x, int y, p2SString name, int size, bool CanBeMoved, UI_Element* parent = nullptr);
	virtual ~Label();
	bool Update(float dt);
	void ChangeLabel(p2SString label, int size);
public:
	
	p2SString name;
	iPoint Distance;
	bool CanBeMoved = false;
};

#endif
