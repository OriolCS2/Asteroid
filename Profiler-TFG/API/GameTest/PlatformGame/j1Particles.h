#ifndef __J1PARTICLES_H__
#define __J1PARTICLES_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class j1Particles : public j1Module
{
public:
	j1Particles();
	~j1Particles();

	bool Start();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	void AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0);
	Animation LoadPushbacks(pugi::xml_node & config, p2SString NameAnim) const;
private:


	Particle* active[MAX_ACTIVE_PARTICLES];

public:

	Particle smokeBottom;
	Particle laserL;
	Particle laserR;

	p2SString sprites_name[2];
	
	Animation DoubleJump;
	Particle Doublejump;

};

#endif // __MODULEPARTICLES_H__