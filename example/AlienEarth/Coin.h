#ifndef _COIN_H__
#define _COIN_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "Entity.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Coin : public Entity {
public:
	Coin(int x, int y);
	virtual ~Coin();
	bool Update(float dt);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void Draw(float dt);
	bool CleanUp();
	void OnCollision(Collider* c2);


public:
	int x;
	int y;
	bool firsttime = true;
	Animation Jeff;
	Animation Jane;
	Animation Jerry;
	Animation* anim = nullptr;
	p2SString sprites;
	p2SString CoinFx;
	unsigned int coinfx;
};

#endif