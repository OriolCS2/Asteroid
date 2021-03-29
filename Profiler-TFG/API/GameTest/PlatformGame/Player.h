#ifndef __Player_H__
#define __Player_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "Entity.h"
#include "j1PerfTimer.h"

class UI_Element;
class Button;
class Slider;
class Image;
class Label;
class CheckBox;

class Player : public Entity
{
	struct BottomHit {
		Animation anim;
		float speed = 0.0f;
		bool IsFalling = false;
	};

	struct Dash {
		Animation StartDash;
		Animation Dashing;
		Animation FinishDash;
		int DashTime = 8;
		int DashCont = 0;
		bool DashLeft = false;
		bool DashRight = false;
		void ResetDashAnims();
	};

public:

	Player();

	//Destructor
	virtual ~Player();

	//Called before player is aviable
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	//// Called each loop iteration
	bool PreUpdate();

	//// Called each loop iteration
	bool PostUpdate();


	//Called every loop iteration
	bool Update(float dt);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void Draw(float dt);
	bool CleanUp();
	void OnCollision(Collider* c2);

	void FirstInit();
	void Die();
	void Fall();
	void Spawn();
	void ChangeLiveSprite();
	Animation LoadPushbacks(int playernumber, pugi::xml_node&, p2SString NameAnim) const;
	void ChangePlayer(const int playernumber, bool InGame = false);
	void GoJump(float dt);
	void GoSwim(float dt);
	void GoClimb(float dt);
	void Move_Left_Right(float dt);
	void Camera(float dt);
	void DoDash(float dt);
	void ShootLaser(float dt);
	void DoubleJump(float dt);
	void BottomFall(float dt);
	void Gravity(float dt);
	void SetUI();
	void SetTutorials();
	void CheckWhatToDoWhenCollidingWithEnemy(Collider * c2);
	void Lifes();
	void TIME();
	void CountCoins();



private:
	float startDash;
	float DashTime;

public:
	bool cameraon = false;
	
	//float TIME;
	j1Timer time;
	
	SDL_Rect CamRect;

	
	Animation idle[3];
	Animation idle2[3];
	Animation GoLeft[3];
	Animation GoRight[3];
	Animation jumpR[3];
	Animation jumpL[3];
	Animation SwimRight[3];
	Animation SwimLeft[3];
	Animation Climb[3];
	Animation ClimbIdle[3];
	Animation Death[3];
	Animation doubleJump;

	pugi::xml_node node;
	
	SDL_Texture* ParticlesTex;

	p2SString sprites_name[3];
	p2SString godmode;
	p2SString introlight;
	p2SString JumpFx;
	p2SString WaterFx;
	p2SString DeathFx;
	p2SString DeathFx2;
	p2SString LadderFx;
	p2SString LaserFx;
	p2SString DashFx;
	p2SString BombJumpfx;
	p2SString SpiderDeathFx;
	p2SString WinningFx;
	p2SString OvniFx;
	p2SString LoseFx;


	int TimeSinceStarted;
	int CurrentTime;

	SDL_Texture* Godmode;
	SDL_Texture* IntroLight;

	Image* life = nullptr;
	Image* lifenumber = nullptr;
	Image* tutorial = nullptr;
	Label* TimeStart = nullptr;
	Label* NumCoins = nullptr;
	Image* CoinUI = nullptr;
	Image* FirstCCoin = nullptr;

	p2SString Coins;
	p2SString StringTime;
	int Min = 0;
	int Hours = 0;

	bool Lifehack = false;

	unsigned int jumpfx;
	unsigned int waterfx;
	unsigned int deathfx;
	unsigned int deathfx2;
	unsigned int ladderfx;
	unsigned int laserfx;
	unsigned int dashfx;
	unsigned int bombjumpfx;
	unsigned int spiderdeathfx;
	unsigned int winningfx;
	unsigned int ovnifx;
	unsigned int losefx;
	
	mutable int SaveDelay = 0;
	int LoadDelay = 0;
	mutable int TimeStarted = 0;

	bool AllCharged = false;
	bool PlayerLeft = false;
	bool PlayerRight = false;

	bool AnimDoubleJump = false;
	bool WalkLeft = false;
	bool WalkRight = false;
	bool GoUp = false;
	bool GoDown = false;
	bool Idle = false;
	bool Hability = false;
	bool Jump = false;
	bool IsJumping = false;
	bool CanJump = true;
	bool CanClimb = false;
	bool Falling = false;
	bool CanSwim = false;
	bool fall = false;
	bool God = false;
	bool NextMap = false;
	bool NoInput = false;
	bool dashing = false;
	bool dashright = false;
	bool StopDash = false;
	bool CanDash = false;
	bool CanJump2 = false;
	bool IsJumping2 = false;
	bool Jump2Complete = false;
	bool CanDoAnotherJump = true;
	bool FallingJump2 = false;
	bool OvniCanMove = false;
	bool GoingRight;
	bool Intro = false;
	
	float JumpSpeed = 0.0f;
	float SpeedWalk = 0.0f;
	float SpeedClimb = 0.0f;
	float SpeedSwimUp = 0.0f;
	float SpeedSwimDown = 0.0f;
	float SpeedSwimLeftRight = 0.0f;
	float DT = 0.0f;
	float AuxJumpSpeed = 0.0f;
	float particletime = 0.0f;
	float TimeBetweenShoot = 0.0f;
	float IntroFall;

	int lifes = 0;
	int startmap2 = 0;
	int maxYcam = 0;
	int minYcam = 0;
	int lowcam = 0;
	int positionWinMap1 = 0;
	int startpointcameramap2 = 0;
	int JumpTime = 0;
	int playerwidth = 0;
	int playerHeight = 0;
	int playerheight = 0;
	int finalmapplayer = 0;
	int finalmap = 0;
	int NumPlayer = 0;
	int ringpositionx = 0;
	int ringpositiony = 0;
	int TutorialX = 0;
	int TutorialY1 = 0;
	int TutorialY2 = 0;
	


	uint Time = 0;

	int cont = 0;

	BottomHit BottomLeft;
	BottomHit BottomRight;

	Dash dashR;
	Dash dashL;
};



#endif // 

