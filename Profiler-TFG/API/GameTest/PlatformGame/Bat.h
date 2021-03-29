#ifndef _BAT_H__
#define _BAT_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "Entity.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Bat : public Entity {
public:
	Bat(int x, int y);
	virtual ~Bat();
	bool PreUpdate();
	bool PostUpdate();
	bool Update(float dt);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void Draw(float dt);
	bool CleanUp();
	void OnCollision(Collider* c2);
	void AnimationLogic();

	Animation LoadPushbacks(pugi::xml_node&, p2SString NameAnim) const;

public:
	iPoint original_pos;
	bool firsttime = true;
	Animation GoLeft;
	Animation GoRight;
	Animation Idle;
	int range = 0;
	float Speed = 0.0f;
	Animation HitLeft;
	Animation HitRight;
	Animation DieLeft;
	Animation DieRight;
	p2SString sprites;
};


#endif


