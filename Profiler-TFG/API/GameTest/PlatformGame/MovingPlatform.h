#ifndef _MOVINGPLATFORM_H__
#define _MOVINGPLATFORM_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "Entity.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class MovingPlatform : public Entity {
public:
	MovingPlatform(int x, int y);
	virtual ~MovingPlatform();
	bool Update(float dt);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void Draw(float dt);
	bool CleanUp();
	Animation LoadPushbacks(pugi::xml_node&, p2SString NameAnim) const;


public:
	iPoint original_pos;
	bool firsttime = true;
	float StartTime = 0.0f;
	bool Start = true;
	bool MoveRight = false;
	bool MoveLeft = false;
	float Speed = 200;
	Animation Idle;
	p2SString sprites;
};

#endif