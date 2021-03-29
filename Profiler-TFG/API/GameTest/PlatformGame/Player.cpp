#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "EntityManager.h"
#include "Label.h"
#include "j1Timer.h"
#include "j1Scene.h"
#include "j1Menu.h"
#include "Player.h"
#include "j1Particles.h"
#include "UI_Element.h"
#include "UI_Manager.h"
#include "Image.h"

#include "..\..\Profiler.h"

Player::Player() : Entity()
{

}

Player::~Player() {}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");

	pugi::xml_node player = config.child("player");

	sprites_name[0] = player.child("sprites").text().as_string();
	sprites_name[1] = player.child("sprites2").text().as_string();
	sprites_name[2] = player.child("sprites3").text().as_string();
	godmode = player.child("godmode").text().as_string();
	introlight = player.child("IntroLight").text().as_string();
	ringpositionx = player.child("ringpositionx").attribute("value").as_int();
	ringpositiony = player.child("ringpositiony").attribute("value").as_int();
	JumpFx = player.child("JumpFx").text().as_string();
	WaterFx = player.child("WaterFx").text().as_string();
	DeathFx = player.child("DeathFx").text().as_string();
	DeathFx2 = player.child("DeathFx2").text().as_string();
	LadderFx = player.child("LadderFx").text().as_string();
	LaserFx = player.child("LaserFx").text().as_string();
	DashFx = player.child("DashFx").text().as_string();
	OvniFx = player.child("OvniFx").text().as_string();
	BombJumpfx = player.child("BombJumpFx").text().as_string();
	SpiderDeathFx = player.child("SpiderDeathFx").text().as_string();
	WinningFx = player.child("WinningFx").text().as_string();
	LoseFx = player.child("LoseFx").text().as_string();
	finalmapplayer = player.child("finalmapplayer").attribute("value").as_int();
	finalmap = player.child("finalmap").attribute("value").as_int();
	startmap2 = player.child("startmap2").attribute("value").as_int();
	maxYcam = player.child("maxYcam").attribute("value").as_int();
	minYcam = player.child("minYcam").attribute("value").as_int();
	lowcam = player.child("lowcam").attribute("value").as_int();
	gravity = player.child("gravity").attribute("value").as_float();
	IntroFall = player.child("IntroFall").attribute("value").as_float();
	positionWinMap1 = player.child("positionWinMap1").attribute("value").as_int();
	startpointcameramap2 = player.child("startpointcameramap2").attribute("value").as_int();
	SpeedSwimLeftRight = player.child("SpeedSwimLeftRight").attribute("value").as_float();
	SpeedSwimUp = player.child("SpeedSwimUp").attribute("value").as_float();
	SpeedClimb = player.child("SpeedClimb").attribute("value").as_float();
	SpeedWalk = player.child("SpeedWalk").attribute("value").as_float();
	playerHeight = player.child("playerHeight").attribute("value").as_int();
	SpeedSwimDown = player.child("SpeedSwimDown").attribute("value").as_float();
	JumpTime = player.child("JumpTime").attribute("value").as_int();
	JumpSpeed = player.child("JumpSpeed").attribute("value").as_float();
	AuxJumpSpeed = player.child("AuxJumpSpeed").attribute("value").as_float();
	playerwidth = player.child("playerwidth").attribute("value").as_int();
	playerheight = player.child("playerheight").attribute("value").as_int();
	BottomLeft.speed = player.child("BottomSpeed").attribute("value").as_float();
	BottomRight.speed = player.child("BottomSpeed").attribute("value").as_float();
	TutorialX = player.child("TutorialX").attribute("value").as_int();
	TutorialY1 = player.child("TutorialY1").attribute("value").as_int();
	TutorialY2 = player.child("TutorialY2").attribute("value").as_int();
	TimeBetweenShoot = player.child("TimeBetweenShoot").attribute("value").as_float();
	lifes = player.child("lifes").attribute("value").as_int();

	for (int numplayer = 0; numplayer < 3; ++numplayer) {
		idle[numplayer] = LoadPushbacks(numplayer, player, "idle");
		idle2[numplayer] = LoadPushbacks(numplayer, player, "idle2");
		GoRight[numplayer] = LoadPushbacks(numplayer, player, "GoRight");
		GoLeft[numplayer] = LoadPushbacks(numplayer, player, "GoLeft");
		jumpR[numplayer] = LoadPushbacks(numplayer, player, "jumpR");
		jumpL[numplayer] = LoadPushbacks(numplayer, player, "jumpL");
		Climb[numplayer] = LoadPushbacks(numplayer, player, "Climb");
		ClimbIdle[numplayer] = LoadPushbacks(numplayer, player, "ClimbIdle");
		SwimRight[numplayer] = LoadPushbacks(numplayer, player, "SwimRight");
		SwimLeft[numplayer] = LoadPushbacks(numplayer, player, "SwimLeft");
		Death[numplayer] = LoadPushbacks(numplayer, player, "Death");
		if (numplayer == 0) {
			BottomLeft.anim = LoadPushbacks(numplayer, player, "BottomLeft");
			BottomRight.anim = LoadPushbacks(numplayer, player, "BottomRight");
			doubleJump = LoadPushbacks(numplayer, player, "doubleJump");
		}
		if (numplayer == 2) {
			dashR.StartDash = LoadPushbacks(numplayer, player, "StartDashRight");
			dashR.FinishDash = LoadPushbacks(numplayer, player, "FinishDashRight");
			dashR.Dashing = LoadPushbacks(numplayer, player, "DashingRight");
			dashL.StartDash = LoadPushbacks(numplayer, player, "StartDashLeft");
			dashL.FinishDash = LoadPushbacks(numplayer, player, "FinishDashLeft");
			dashL.Dashing = LoadPushbacks(numplayer, player, "DashingLeft");
		}
	}

	bool ret = true;
	return ret;
}

bool Player::Start()
{
	PROFILER_FUNCTION();
	bool ret = true;
	auxGravity = gravity;


	if (!AllCharged) {
		jumpfx = App->audio->LoadFx(JumpFx.GetString());
		waterfx = App->audio->LoadFx(WaterFx.GetString());
		deathfx = App->audio->LoadFx(DeathFx.GetString());
		deathfx2 = App->audio->LoadFx(DeathFx2.GetString());
		ladderfx = App->audio->LoadFx(LadderFx.GetString());
		laserfx = App->audio->LoadFx(LaserFx.GetString());
		dashfx = App->audio->LoadFx(DashFx.GetString());
		bombjumpfx = App->audio->LoadFx(BombJumpfx.GetString());
		spiderdeathfx = App->audio->LoadFx(SpiderDeathFx.GetString());
		winningfx = App->audio->LoadFx(WinningFx.GetString());
		ovnifx = App->audio->LoadFx(OvniFx.GetString());
		losefx = App->audio->LoadFx(LoseFx.GetString());

		Godmode = App->tex->Load(godmode.GetString());
		IntroLight = App->tex->Load(introlight.GetString());

	}

	AllCharged = true;

	position.x = App->entitymanager->positionStartMap1.x;
	position.y = App->entitymanager->positionStartMap1.y;


	
	current_animation = &idle[NumPlayer];

	
	TimeSinceStarted = SDL_GetTicks();
	Min = 0;
	Hours = 0;
	App->scene->NumberCoins = 0;
	//CurrentTime = SDL_GetTicks();

	Death[NumPlayer].loops = 0;
	Death[NumPlayer].current_frame = 0.0f;
	
	return ret;
}
bool Player::PreUpdate() //Here we preload the input functions to determine the state of the player
{
	PROFILER_FUNCTION();
	if (!App->scene->GamePaused) {
		if (!God && !Intro) {
			if (!NoInput) {
				if (!dashing) {
					WalkLeft = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
					WalkRight = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
				}
				GoUp = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
				GoDown = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
				Hability = App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN;
				if (!God)
					Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
				else Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
				if (!WalkLeft && !WalkRight && !CanSwim && !CanClimb)
					Idle = true;
				else
					Idle = false;
			}
		}
		else if (God && !Intro)
		{
			if (!NoInput) {
				WalkLeft = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
				WalkRight = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
					position.y -= 500.0f*DT;
				if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
					position.y += 500.0f*DT;
				if (!WalkLeft && !WalkRight && !CanSwim && !CanClimb)
					Idle = true;
				else
					Idle = false;
			}
		}
		if (!Intro) {
			if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
				God = !God;
		}
	}

	return true;
}
bool Player::Update(float dt)
{
	PROFILER_FUNCTION();
	DT = dt;
	
	if (!App->scene->GamePaused || App->scene->StartTimer) {
		Camera(dt);
	}

	if (!App->scene->GamePaused) {
		if (!God && !Intro) {
			if (!TouchingGround && !dashing && !CanSwim) {
				acceleration.y = gravity * dt;
				LOG("Acceleration %f", acceleration.y);
			}
			else
				acceleration.y = 0;
			position.x += velocity.x;
			position.y -= velocity.y + acceleration.y;
		}
		else if (Intro) {
			position.y -= IntroFall * dt;
			App->audio->PlayFx(ovnifx);
		}

		if (!dashing) {
			if (NumPlayer == 0)
				DoubleJump(dt);
			GoJump(dt);
			GoSwim(dt);
			GoClimb(dt);
			Move_Left_Right(dt);
			if (NumPlayer == 1)
				ShootLaser(dt);
		}
		if (NumPlayer == 2)
			DoDash(dt);
		if (NumPlayer == 0)
			BottomFall(dt);
		TIME();
		if (death && !God) {
			death = false;
			Die();
		}
		if (fall && !God) {
			fall = false;
			if (lifes > 0) {
				if (NumPlayer == 0 || NumPlayer == 2)
					App->audio->PlayFx(deathfx2);
				else
					App->audio->PlayFx(deathfx);
			}
			
			Fall();
		}

		if (coll != nullptr)
			coll->SetPos(position.x, position.y);

		LOG("Position X: %f", position.x);
		LOG("Position Y: %f", position.y);

		TouchingGround = false;
	}


	return true;
}

bool Player::PostUpdate()
{
	PROFILER_FUNCTION();
	if (!Intro && !App->scene->GamePaused) {
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
			ChangePlayer(0, true);
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
			ChangePlayer(1, true);
		}
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
			ChangePlayer(2, true);
		}
	}
	return true;
}
bool Player::Load(pugi::xml_node& player)
{
	

	position.x = player.child("position").attribute("x").as_float();
	position.y = player.child("position").attribute("y").as_float() - 50.0f;
	God = player.child("god").attribute("value").as_bool();
	lifes = player.child("lifes").attribute("value").as_int();
	Min = player.child("Min").attribute("value").as_int();
	Hours = player.child("Hours").attribute("Hours").as_int();
	App->scene->Delay = player.child("Delay").attribute("value").as_int();
	App->scene->NumberCoins = player.child("NumCoins").attribute("value").as_int();
	CountCoins();
	ChangePlayer(player.child("NumPlayer").attribute("value").as_int(), true);
	Lifes();
	App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
	LoadDelay = SDL_GetTicks() - SaveDelay;
	TimeSinceStarted = TimeStarted;
	return true;
}
bool Player::Save(pugi::xml_node& player) const
{
	player.append_child("position").append_attribute("x") = position.x;
	player.child("position").append_attribute("y") = position.y;
	player.append_child("god").append_attribute("value") = God;
	player.append_child("NumPlayer").append_attribute("value") = NumPlayer;
	player.append_child("lifes").append_attribute("value") = lifes;
	player.append_child("Min").append_attribute("value") = Min;
	player.append_child("Hours").append_attribute("Hours") = Hours;
	player.append_child("Delay").append_attribute("value") = App->scene->Delay;
	player.append_child("NumCoins").append_attribute("value") = App->scene->NumberCoins;
	SaveDelay = SDL_GetTicks();
	TimeStarted = TimeSinceStarted;

	return true;
}
void Player::Draw(float dt)
{
		if (Intro) {
			App->render->DrawQuad({ -App->render->camera.x,-App->render->camera.y,App->render->camera.w,App->render->camera.h }, 0, 0, 0, 150);
			if (!OvniCanMove)
				App->render->Blit(IntroLight, 620, 0);
			else {
				App->render->Blit(IntroLight, 620, 3);
			}
		}
		if (current_animation == &dashR.FinishDash) {
			if (!App->scene->GamePaused)
				App->render->Blit(texture, position.x - playerwidth, position.y, &(current_animation->GetCurrentFrame(dt)));
			else App->render->Blit(texture, position.x - playerwidth, position.y, &(current_animation->frames[current_animation->SeeCurrentFrame()]));
		}
		else {
			if (!App->scene->GamePaused)
				App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));
			else App->render->Blit(texture, position.x, position.y, &(current_animation->frames[current_animation->SeeCurrentFrame()]));
		}
		if (God) {
			if (current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer] || current_animation == &Climb[NumPlayer] || current_animation == &ClimbIdle[NumPlayer] || current_animation == &SwimRight[NumPlayer])
					App->render->Blit(Godmode, position.x - ringpositionx, position.y - ringpositiony);
			else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer] || current_animation == &SwimLeft[NumPlayer])
				App->render->Blit(Godmode, position.x + ringpositionx, position.y - ringpositiony, NULL, SDL_FLIP_HORIZONTAL);
		}
	
	
}
bool Player::CleanUp()
{
	App->tex->UnLoad(texture);
	App->tex->UnLoad(ParticlesTex);
	App->tex->UnLoad(Godmode);
	App->tex->UnLoad(IntroLight);
	NextMap = false;
	death = false;
	fall = false;
	God = false;
	if (coll)
		coll->to_delete = true;
	return true;
}

void Player::OnCollision(Collider * c2) //this determine what happens when the player touch a type of collider
{
	switch (c2->type) {
	case COLLIDER_GROUND:
		if (position.y < c2->rect.y + c2->rect.h) {
			if (Intro)
				Intro = false;
			velocity.y = 0;
			if (current_animation == &BottomLeft.anim || current_animation == &BottomRight.anim) {
				App->particles->AddParticle(App->particles->smokeBottom, position.x - 20, position.y, COLLIDER_NONE);
				App->audio->PlayFx(bombjumpfx);
				App->particles->smokeBottom.anim.current_frame = 0.0f;
				App->particles->smokeBottom.anim.loops = 0;
			}
			TouchingGround = true;
			CanJump = true;
			CanJump2 = false;
			GoDown = false;
			CanClimb = false;
			CanDash = true;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			Falling = false;
			FallingJump2 = false;
			CanSwim = false;
			//cameraon = true;
			CanDoAnotherJump = true;
			if (current_animation == &jumpR[NumPlayer]|| current_animation == &Climb[NumPlayer])
				current_animation = &idle[NumPlayer];
			if (current_animation == &jumpL[NumPlayer])
				current_animation = &idle2[NumPlayer];
			
		}
		break;
	case COLLIDER_WALL_UP:
		AnimDoubleJump = false;
		velocity.y = 0;
		IsJumping = false;
		if (Jump2Complete)
			IsJumping2 = false;
		Jump2Complete = false;
		FallingJump2 = true;
		GoUp = false;
		break;
	case COLLIDER_WALL_LEFT:

		if (!CanSwim && !CanClimb && WalkLeft)
			position.x += SpeedWalk * DT;
		if (CanSwim && WalkLeft)
			position.x += SpeedSwimLeftRight * DT;
		if (CanClimb && WalkLeft)
			position.x += SpeedWalk * DT;
		if (dashing) {
			dashing = false;
			current_animation = &GoLeft[NumPlayer];
		}
		break;
	case COLLIDER_WALL_RIGHT:
		if (!CanSwim && !CanClimb && WalkRight)
			position.x -= SpeedWalk * DT;
		if (CanSwim && WalkRight)
			position.x -= SpeedSwimLeftRight * DT;
		if (CanClimb && WalkRight)
			position.x -= SpeedWalk * DT;
		if (dashing) {
			dashing = false;
			current_animation = &GoRight[NumPlayer];
		}

		break;
	case COLLIDER_PLATFORM:
		if (position.y +playerHeight < c2->rect.y)
		{
			velocity.y = 0;
			if (current_animation == &BottomRight.anim || current_animation == &BottomRight.anim) {
				App->audio->PlayFx(bombjumpfx);
			}
			//IsJumping = false;
			//IsJumping2 = false;
			TouchingGround = true;
			CanDoAnotherJump = true;
			CanJump = true;
			CanClimb = false;
			FallingJump2 = false;
			CanJump2 = false;
			GoDown = false;
			CanSwim = false;
			CanDash = true;
			Falling = false;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			if (!dashing)
				if (current_animation == &GoRight[NumPlayer] || current_animation == &jumpR[NumPlayer])
					current_animation = &idle[NumPlayer];
				else if (current_animation == &GoLeft[NumPlayer] || current_animation == &jumpL[NumPlayer])
					current_animation = &idle2[NumPlayer];
		}

		break;
	case COLLIDER_MOVINGPLATFORM:
		if (position.y + playerHeight < c2->rect.y)
		{
			if (PlayerLeft)
				position.x -= 200 * DT;
			if (PlayerRight)
				position.x += 200 * DT;
			velocity.y = 0;
			if (current_animation == &BottomRight.anim || current_animation == &BottomRight.anim) {
				App->audio->PlayFx(bombjumpfx);
			}
			//IsJumping = false;
			//IsJumping2 = false;
			TouchingGround = true;
			CanDoAnotherJump = true;
			CanJump = true;
			CanClimb = false;
			FallingJump2 = false;
			CanJump2 = false;
			GoDown = false;
			CanSwim = false;
			CanDash = true;
			Falling = false;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			if (!dashing)
				if (current_animation == &GoRight[NumPlayer] || current_animation == &jumpR[NumPlayer])
					current_animation = &idle[NumPlayer];
				else if (current_animation == &GoLeft[NumPlayer] || current_animation == &jumpL[NumPlayer])
					current_animation = &idle2[NumPlayer];
		}

		break;
	case COLLIDER_CLIMB:
		if (!God) {
			if (current_animation == &Climb[NumPlayer])
				App->audio->PlayFx(ladderfx);
			IsJumping = false;
			IsJumping2 = false;
			CanDoAnotherJump = false;
			Falling = false;
			TouchingGround = true;
			AnimDoubleJump = false;
			CanClimb = true;
			CanJump = true;
			CanJump2 = false;
			velocity.y = 0;
			if (current_animation == &jumpR[NumPlayer] || current_animation == &jumpL[NumPlayer])
				current_animation = &ClimbIdle[NumPlayer];
		}
		break;
	case COLLIDER_WATER:
		App->audio->PlayFx(waterfx);
		if (!God) {
			CanSwim = true;
			TouchingGround = true;
			CanClimb = false;
			CanJump = false;
			CanJump2 = false;
			current_animation = &SwimRight[NumPlayer];
			IsJumping = false;
			IsJumping2 = false;
			CanDash = false;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			Falling = false;
			FallingJump2 = false;
			//cameraon = true;
			CanDoAnotherJump = false;
		}
		break;
	case COLLIDER_GROUND_WATER:
		if (!God) {
			if (position.y < c2->rect.y + c2->rect.h) {
				if (velocity.y < 0) {
					velocity.y = 0;
				}
				CanJump = false;
				CanJump2 = false;
				GoDown = false;
				CanClimb = false;
				CanDash = true;
				BottomLeft.IsFalling = false;
				BottomRight.IsFalling = false;
				Falling = false;
				FallingJump2 = false;
				CanSwim = true;
				//cameraon = true;
				CanDoAnotherJump = true;
				if (current_animation == &jumpR[NumPlayer])
					current_animation = &idle[NumPlayer];
				if (current_animation == &jumpL[NumPlayer])
					current_animation = &idle2[NumPlayer];
			}
		}
		break;
	case COLLIDER_NONE:
		CanClimb = false;
		CanSwim = false;
		break;
	case COLLIDER_SPIKES:
		if (!God) {
			velocity.y = 0;
			TouchingGround = true;
			WalkLeft = false;
			WalkRight = false;
			GoUp = false;
			GoDown = false;
			CanJump = false;
			CanJump2 = false;
			death = true;
			NoInput = true;
		}
		break;
	case COLLIDER_ENEMY_BAT:
		if (!God)
			CheckWhatToDoWhenCollidingWithEnemy(c2);
		break;
	case COLLIDER_ENEMY_SPIDER:
		if (!God)
			CheckWhatToDoWhenCollidingWithEnemy(c2);
		break;
	case COLLIDER_FALL:
		WalkLeft = false;
		WalkRight = false;
		GoUp = false;
		GoDown = false;
		fall = true;
		if (!God)
			NoInput = true;
		break;
	case COLLIDER_ROPE:
		if (!God) {
			TouchingGround = true;
			CanClimb = true;
			CanJump = true;
			CanJump2 = false;
			velocity.y = 0;
		}
		break;
	case COLLIDER_WIN:
		NoInput = true;
		App->scene->KnowMap = 0;
		App->fade->FadeToBlack(3.0f);
		App->audio->PlayFx(winningfx);
		current_animation = &idle[NumPlayer];
		TouchingGround = true;
		WalkLeft = false;
		WalkRight = false;
		App->scene->CanStart = true;
		break;
	}
}

void Player::FirstInit()
{

	position.x = App->entitymanager->positionStartMap1.x;
	position.y = App->entitymanager->positionStartMap1.y;

	current_animation = &idle[NumPlayer];


	TimeSinceStarted = SDL_GetTicks();
	Min = 0;
	Hours = 0;
	App->scene->NumberCoins = 0;
	//CurrentTime = SDL_GetTicks();

	Death[NumPlayer].loops = 0;
	Death[NumPlayer].current_frame = 0.0f;
}

void Player::Die()//What happens when the player die
{
	
	current_animation = &Death[NumPlayer];
	if (Death[NumPlayer].SeeCurrentFrame() == 1 && lifes > 0)
		if (NumPlayer == 0 || NumPlayer == 2)
			App->audio->PlayFx(deathfx2);
		else
			App->audio->PlayFx(deathfx);
	if (Death[NumPlayer].Finished()) {
		App->entitymanager->DeleteEnemies();
		if (App->scene->KnowMap == 0) {
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		}
		if (App->scene->KnowMap == 1) {
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		}
		if (!Lifehack)
			lifes -= 1;
		Spawn();
		App->scene->NumberCoins = 0;
		CountCoins();
	}
}

void Player::Fall()//What happens when the player falls
{
	
	App->entitymanager->DeleteEnemies();
	if (App->scene->KnowMap == 0) {
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
	}
	if (App->scene->KnowMap == 1) {
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
	}
	if (!Lifehack)
		lifes -= 1;
	Spawn();
	App->scene->NumberCoins = 0;
	CountCoins();

}

void Player::Spawn()
{
	if (!App->scene->CanStart) {
		if (lifes >= 0) {
			Lifes();
			NoInput = false;
			CanJump = true;
			CanClimb = false;
			CanSwim = false;
			Death[NumPlayer].current_frame = 0.0f;
			Death[NumPlayer].loops = 0;
			current_animation = &idle[NumPlayer];
			if (App->scene->KnowMap == 0) {
				position.x = App->entitymanager->positionSpawnMap1.x;
				position.y = App->entitymanager->positionSpawnMap1.y;
			}
			if (App->scene->KnowMap == 1) {
				position.x = App->entitymanager->positionStartMap2.x;
				position.y = App->entitymanager->positionStartMap2.y;
			}
			Death[NumPlayer].current_frame = 0.0f;
			Death[NumPlayer].loops = 0;
			App->entitymanager->DeleteEnemies();
			App->map->LoadEnemies();
			App->map->LoadCoins();
		}
		else {
			//current_animation = &idle[NumPlayer];
			Death[NumPlayer].current_frame = 0.0f;
			Death[NumPlayer].loops = 0;
			NoInput = true;
			App->scene->KnowMap = 0;
			App->fade->FadeToBlack(3.0f);
			App->audio->PlayFx(losefx);
			TouchingGround = true;
			WalkLeft = false;
			WalkRight = false;
			App->scene->CanStart = true;
			App->scene->KnowMap = 0;
		}
	}
	
}

void Player::ChangeLiveSprite()
{
	if (NumPlayer == 0) {
		life->SetSpritesData({ 1156,1103,63,63 });
		CoinUI->SetSpritesData({ 1156,1165,63,65 });
	}
	else if (NumPlayer == 1) {
		life->SetSpritesData({ 1220,1103,63,63 });
		CoinUI->SetSpritesData({ 1220,1165,63,65 });
	}
	else if (NumPlayer == 2) {
		life->SetSpritesData({ 1284,1103,63,63 });
		CoinUI->SetSpritesData({ 1284,1165,63,65 });
	}
}

Animation Player::LoadPushbacks(int playernumber, pugi::xml_node& config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	switch (playernumber) {
	case 0:
		XML_Name_Player_Anims = "AnimationsPlayerYellow";
		break;
	case 1:
		XML_Name_Player_Anims = "AnimationsPlayerPink";
		break;
	case 2:
		XML_Name_Player_Anims = "AnimationsPlayerBlue";
		break;
	}

	for (pugi::xml_node frames = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).child("frame"); frames; frames = frames.next_sibling("frame")) {
		rect.x = frames.attribute("x").as_int();
		rect.y = frames.attribute("y").as_int();
		rect.w = frames.attribute("w").as_int();
		rect.h = frames.attribute("h").as_int();
		anim.PushBack({ rect.x,rect.y,rect.w,rect.h });
	}
	anim.speed = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("speed").as_float();
	anim.loop = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("loop").as_bool();

	return anim;
}

void Player::ChangePlayer(const int playernumber, bool InGame)
{
	PROFILER_FUNCTION();
	
	App->tex->UnLoad(texture);
	texture = App->tex->Load(sprites_name[playernumber].GetString());
	App->collision->ColliderCleanUpPlayer();
	NumPlayer = playernumber;
	if (InGame)
		ChangeLiveSprite();
	switch (playernumber) {
	case 0:
		position.y -= 17;
		coll = App->collision->AddCollider({ 0, 0, playerwidth, playerheight }, COLLIDER_PLAYER, (j1Module*)App->entitymanager);
		break;
	case 1:
		position.y -= 17;
		coll = App->collision->AddCollider({ 0, 0, 67, 93 }, COLLIDER_PLAYER, (j1Module*)App->entitymanager);
		break;
	case 2:
		position.y -= 17;
		coll = App->collision->AddCollider({ 0, 0, 67, 93 }, COLLIDER_PLAYER, (j1Module*)App->entitymanager);
		break;
	}
	TouchingGround = false;
	current_animation = &idle[NumPlayer];
	
}

void Player::GoJump(float dt)
{
	if (Jump && CanJump && !CanSwim && !God && !IsJumping) { //If you clicked the jump button and you are able to jump(always except you just jumpt) you can jump
		IsJumping = true;
		TouchingGround = false;
		JumpSpeed = AuxJumpSpeed;
		starttime = SDL_GetTicks();
		//App->particles->AddParticle(App->particles->smokeBottom, position.x, position.y, COLLIDER_NONE, { 0,0 });
	}
	if (IsJumping) { //if you are able to jump, determine the animation and direction of the jump
					 //Time = Time * dt;

		float TIME = SDL_GetTicks();
		LOG("TIME %f", TIME);
		CanJump = false;
		if (TIME - starttime == 0)
			App->audio->PlayFx(jumpfx);
		if (TIME - starttime > 20 && TIME - starttime < 300) {
			JumpSpeed -= 2750.0f *dt;
		}
		if (TIME - starttime <= 300 && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300 && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		if (TIME - starttime >= 300) {
			IsJumping = false;
			CanJump2 = true;
			CanJump = false;
			Falling = true;
			JumpSpeed = AuxJumpSpeed;
			Time = 0;
			if (current_animation == &jumpR[NumPlayer]) {
				current_animation = &idle[NumPlayer];
			}
			else current_animation = &idle2[NumPlayer];
		}
	}

	if (God && Jump) { //if you are in god mode and jump, you can fly
					   //App->audio->PlayFx(jumpfx);
		position.y -= JumpSpeed * dt;
	}
}

void Player::GoSwim(float dt)
{
	if (CanSwim) {
		if (current_animation == &SwimLeft[NumPlayer]) {
			current_animation = &SwimLeft[NumPlayer];
		}
		else {
			current_animation = &SwimRight[NumPlayer];
		}
	}
	if (CanSwim && GoUp) { //Can Swim determine if you are in a water collider, if you are, it's true
		velocity.y = SpeedSwimUp * dt;
	}
	else if (CanSwim && GoDown) {
		velocity.y = -SpeedSwimDown * dt;
	}
	else
		velocity.y = 0;

}

void Player::GoClimb(float dt)
{
	if (CanClimb && GoUp) {
		position.y -= SpeedClimb * dt;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && GoDown) {
		position.y -= -SpeedClimb * dt;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && !GoUp && !GoDown)
		current_animation = &ClimbIdle[NumPlayer];

}

void Player::Move_Left_Right(float dt)
{
	if (!BottomLeft.IsFalling && !BottomRight.IsFalling) {
		if (WalkRight) { //This determine the movement to the right, depending on the state of the player
			if (!IsJumping && !CanSwim && !CanClimb) {
				dashR.ResetDashAnims();
				dashL.ResetDashAnims();
				position.x += SpeedWalk * dt;
				current_animation = &GoRight[NumPlayer];
			}
			if (IsJumping) {
				position.x += SpeedWalk * dt;
			}
			if (CanSwim && !CanClimb) { //Can Climb determine if you are in a climb collider, if you are, it's true
				position.x += SpeedSwimLeftRight * dt;
				current_animation = &SwimRight[NumPlayer];
			}
			if (CanClimb)
				position.x += SpeedWalk * dt;
			if (Falling)
				current_animation = &jumpR[NumPlayer];
		}
		if (WalkLeft) { //This determine the movement to the left, depending on the state of the player
			if (!IsJumping && !CanSwim && !CanClimb) {
				dashR.ResetDashAnims();
				dashL.ResetDashAnims();
				position.x -= SpeedWalk * dt;
				current_animation = &GoLeft[NumPlayer];
			}
			if (IsJumping) {
				position.x -= SpeedWalk * dt;
			}
			if (CanSwim && !CanClimb) {
				position.x -= SpeedSwimLeftRight * dt;
				current_animation = &SwimLeft[NumPlayer];
			}
			if (CanClimb)
				position.x -= SpeedWalk * dt;
			if (Falling)
				current_animation = &jumpL[NumPlayer];
		}
		if (WalkRight && WalkLeft) {
			if (!CanSwim)
				current_animation = &idle[NumPlayer];
			if (CanSwim)
				current_animation = &SwimRight[NumPlayer];
			if (CanClimb) {
				current_animation = &Climb[NumPlayer];
			}
			if (Falling)
				current_animation = &jumpR[NumPlayer];
		}
	}

	if (Idle) {
		if (!BottomLeft.IsFalling && NumPlayer == 0 && current_animation == &BottomLeft.anim)
			current_animation = &idle2[NumPlayer];
		if (!BottomRight.IsFalling && NumPlayer == 0 && current_animation == &BottomRight.anim)
			current_animation = &idle[NumPlayer];
		if (current_animation == &GoRight[NumPlayer])
			current_animation = &idle[NumPlayer];
		if (current_animation == &GoLeft[NumPlayer])
			current_animation = &idle2[NumPlayer];
		if (dashL.FinishDash.Finished()) {
			current_animation = &idle2[NumPlayer];
			dashL.ResetDashAnims();
			dashR.ResetDashAnims();
		}
		if (dashR.FinishDash.Finished()) {
			current_animation = &idle[NumPlayer];
			dashR.ResetDashAnims();
			dashL.ResetDashAnims();
		}
		if (Falling&&current_animation == &idle[NumPlayer])
			current_animation = &jumpR[NumPlayer];
		if (Falling&&current_animation == &idle2[NumPlayer])
			current_animation = &jumpL[NumPlayer];
	}
}

void Player::Camera(float dt)
{
	if (App->scene->KnowMap == 0 && position.x >= positionWinMap1) {//knowmap it's a varibable that let us know in which map we are. //Knowmap=0, level 1 //knowmap=2, level 2
		NextMap = true;
		App->entitymanager->DeleteEnemies();
	}
	if (position.x <= startmap2 && App->scene->KnowMap == 1) { //If player is in a position where the camera would print out of the map, camera stops
		App->render->camera.x = startpointcameramap2;
	}
	else if (position.x >= finalmapplayer) {
		App->render->camera.x = finalmap;
	}
	else {
			App->render->camera.x = -position.x + (App->render->camera.w / 2);
	}
	if (position.y <= minYcam) { //If player is in a position where the camera would print out of the map, camera stops
		App->render->camera.y = 0;
	}
	else if (position.y >= maxYcam) {
		App->render->camera.y = lowcam;//lowcam is the bottom part of the map, when the player is too low, the camera follows a constant height to don't get out of the map
	}
	else {
		OvniCanMove = true;
		App->render->camera.y = -position.y + (App->render->camera.h / 2);
	}
}

void Player::DoDash(float dt)
{
	if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && Hability && CanDash) {
		App->audio->PlayFx(dashfx);
		JumpSpeed = AuxJumpSpeed;
		dashing = true;
		dashR.DashRight = true;
		startDash = SDL_GetTicks();
	}
	if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && Hability && CanDash) {
		App->audio->PlayFx(dashfx);
		JumpSpeed = AuxJumpSpeed;
		dashing = true;
		dashL.DashLeft = true;
		startDash = SDL_GetTicks();
	}
	if (dashing && dashR.DashRight) {
		velocity.y = 0;
		DashTime = SDL_GetTicks();
		CanJump = false;
		if (DashTime - startDash == 0)
			current_animation = &dashR.StartDash;
		if (DashTime - startDash >= 50) {
			current_animation = &dashR.Dashing;
			position.x += 1800 * dt;
			if (DashTime - startDash >= 200) {
				position.x -= 1600 * dt;
				current_animation = &dashR.FinishDash;
			}
			if (dashR.FinishDash.Finished()) {
				dashing = false;
				IsJumping = false;
				CanJump = false;
				CanDash = false;
				DashTime = 0;
				startDash = 0;
			}
		}

	}
	if (dashing && dashL.DashLeft) {
		velocity.y = 0;
		DashTime = SDL_GetTicks();
		CanJump = false;
		if (DashTime - startDash == 0)
			current_animation = &dashL.StartDash;
		if (DashTime - startDash >= 50) {
			current_animation = &dashL.Dashing;
			position.x -= 1800 * dt;
			if (DashTime - startDash >= 200) {
				position.x += 1600 * dt;
				current_animation = &dashL.FinishDash;
			}
			if (dashL.FinishDash.Finished()) {
				dashing = false;
				IsJumping = false;
				CanJump = false;
				CanDash = false;
				DashTime = 0;
				startDash = 0;
			}
		}
		
	}
}


void Player::ShootLaser(float dt)
{
	if (SDL_GetTicks() - particletime >= TimeBetweenShoot) {
		if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && Hability) {
			particletime = SDL_GetTicks();
			App->particles->AddParticle(App->particles->laserR, position.x - 10, position.y + 22, COLLIDER_PARTICLE);
			App->audio->PlayFx(laserfx);
		}
		if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && Hability) {
			particletime = SDL_GetTicks();
			App->particles->AddParticle(App->particles->laserL, position.x - 10, position.y + 22, COLLIDER_PARTICLE);
			App->audio->PlayFx(laserfx);
		}
	}
}

void Player::DoubleJump(float dt)
{
	if (CanJump2 && Jump && !IsJumping && CanDoAnotherJump) {
		AnimDoubleJump = true;
		IsJumping2 = true;
		Falling = false;
		Jump2Complete = true;
		starttime = SDL_GetTicks();
		JumpSpeed = AuxJumpSpeed;
	}
	if (Jump && IsJumping && !CanJump2 && CanDoAnotherJump) {
		AnimDoubleJump = true;
		IsJumping = false;
		CanJump2 = true;
		Falling = false;
		CanJump = false;
		Jump2Complete = true;
		Time = 0;
		starttime = SDL_GetTicks();
		JumpSpeed = AuxJumpSpeed;
		IsJumping2 = true;
	}
	if (AnimDoubleJump) {
		
		App->particles->AddParticle(App->particles->Doublejump, position.x - 9, position.y + playerHeight -25, COLLIDER_NONE);
	}
	if (IsJumping2) { //if you are able to jump, determine the animation and direction of the jump
		CanDoAnotherJump = false;
		Time += 100 * dt;
		float TIME = SDL_GetTicks();
		if (TIME - starttime == 0)
			App->audio->PlayFx(jumpfx);
		if (TIME - starttime > 20 && TIME - starttime < 300) {
			JumpSpeed -= 2750.0f *dt;
		}
		if (TIME - starttime <= 300 && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300 && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		if (TIME - starttime >= 300) {
			FallingJump2 = true;
			AnimDoubleJump = false;
			IsJumping2 = false;
			CanJump2 = false;
			Falling = true;
			JumpSpeed = AuxJumpSpeed;
			Jump2Complete = false;
		}
	}
}

void Player::BottomFall(float dt)
{
	if (Hability && IsJumping2) {
		if (current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) {
			AnimDoubleJump = false;
			BottomRight.anim.loops = 0;
			BottomRight.anim.current_frame = 0.0f;
			BottomRight.IsFalling = true;
		}
		else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) {
			AnimDoubleJump = false;
			BottomLeft.anim.loops = 0;
			BottomLeft.anim.current_frame = 0.0f;
			BottomLeft.IsFalling = true;
		}

		IsJumping2 = false;
		CanJump2 = false;
	}
	if (Hability && FallingJump2) {
		if (current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) {
			BottomRight.anim.loops = 0;
			BottomRight.anim.current_frame = 0.0f;
			BottomRight.IsFalling = true;
		}
		else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) {
			BottomLeft.anim.loops = 0;
			BottomLeft.anim.current_frame = 0.0f;
			BottomLeft.IsFalling = true;
		}
		IsJumping2 = false;
		CanJump2 = false;
	}
	if (BottomLeft.IsFalling) {
		current_animation = &BottomLeft.anim;
		position.y += BottomLeft.speed * dt;
	}
	if (BottomRight.IsFalling) {
		current_animation = &BottomRight.anim;
		position.y += BottomRight.speed * dt;
	}

}

void Player::Gravity(float dt)
{
	gravity = auxGravity;
	gravity = gravity * dt;
	position.y -= gravity;
}

void Player::SetUI()
{
	//Coins
	Coins.create("%i", App->scene->NumberCoins);
	NumCoins = App->ui_manager->CreateLabel(90, 27, Coins.GetString(), 50, false);
	NumCoins->type = PLAYERUILABEL;
	CoinUI = App->ui_manager->CreateImage(20, 20, false);
	CoinUI->type = PLAYERUIIMAGE;
	if (NumPlayer == 0) {
		CoinUI->SetSpritesData({ 1156,1165,63,65 });
	}
	else if (NumPlayer == 1) {
		CoinUI->SetSpritesData({ 1220,1165,63,65 });
	}
	else if (NumPlayer == 2) {
		CoinUI->SetSpritesData({ 1284,1165,63,65 });
	}
	
	//Time
	CurrentTime = SDL_GetTicks();
	StringTime.create("%i:%i:%i", Hours, Min, (CurrentTime - TimeSinceStarted) / 1000);
	TimeStart = App->ui_manager->CreateLabel(App->win->Width / 2 - 73, 27, StringTime.GetString(), 50, false);
	TimeStart->type = PLAYERUILABEL;
	//TimeStart->Scree_pos.x -= 200;

	


	//LIVES
	life = App->ui_manager->CreateImage(882, 20, false);
	life->type = PLAYERUIIMAGE;
	if (NumPlayer == 0) {
		life->SetSpritesData({ 1156,1103,63,63 });
	}
	else if (NumPlayer == 1) {
		life->SetSpritesData({ 1220,1103,63,63 });
	}
		
	else if (NumPlayer == 2) {
		life->SetSpritesData({ 1284,1103,63,63 });
	}
		
	lifenumber = App->ui_manager->CreateImage(950, 25, false);
	lifenumber->type = PLAYERUIIMAGE;
	if (lifes == 5) {
		lifenumber->SetSpritesData({ 1252,1884,47,50 });
	}

	else if (lifes == 4) {
		lifenumber->SetSpritesData({ 1303,1884,48,50 });
	}

	else if (lifes == 3) {
		lifenumber->SetSpritesData({ 1252,1950,47,50 });
	}
		
	else if (lifes == 2) {
		lifenumber->SetSpritesData({ 1303,1950,47,50 });
	}
		
	else if (lifes == 1) {	
		lifenumber->SetSpritesData({ 1355,1950,36,50 });
	}
	
	else if (lifes == 0) {
		lifenumber->SetSpritesData({ 1399,1950,47,50 });
	
	}

}

void Player::SetTutorials()
{
	
	//Tutorials
	if (App->scene->KnowMap == 0) {
		if (NumPlayer == 0) {
			tutorial = App->ui_manager->CreateImage(1002, 250, true);
			tutorial->type = PLAYERUIIMAGE;
			tutorial->SetSpritesData({ 2700,525,602,363 });
		}
		else if (NumPlayer == 1) {
			tutorial = App->ui_manager->CreateImage(1006, 365, true);
			tutorial->type = PLAYERUIIMAGE;
			tutorial->SetSpritesData({ 2700,0,522,274 });
		}
		else if (NumPlayer == 2) {
			tutorial = App->ui_manager->CreateImage(1006, 340, true);
			tutorial->type = PLAYERUIIMAGE;
			tutorial->SetSpritesData({ 2700,274,539,251 });
		}
	}

}



void Player::CheckWhatToDoWhenCollidingWithEnemy(Collider * c2)
{
	if (BottomRight.IsFalling || BottomLeft.IsFalling || dashing)
	{
		App->audio->PlayFx(spiderdeathfx);
		c2->CanBeDeleted = true;
	}
	else {
		velocity.y = 0;
		TouchingGround = true;
		WalkLeft = false;
		WalkRight = false;
		GoUp = false;
		GoDown = false;
		CanJump = false;
		CanJump2 = false;
		death = true;
		if (!God)
			NoInput = true;
	}
	

}

void Player::Dash::ResetDashAnims()
{
	StartDash.current_frame = 0.0f;
	StartDash.loops = 0;

	FinishDash.current_frame = 0.0f;
	FinishDash.loops = 0;

	DashLeft = false;
	DashRight = false;
	DashCont = 0;

}

void Player::Lifes() {

	//Numbers
	if (lifes == 5) {
		lifenumber->SetSpritesData({ 1252,1884,47,50 });
	}

	else if (lifes == 4) {
		lifenumber->SetSpritesData({ 1303,1884,48,50 });
	}

	else if (lifes == 3)
		lifenumber->SetSpritesData({ 1252,1950,47,50 });
	else if (lifes == 2)
		lifenumber->SetSpritesData({ 1303,1950,47,50 });
	else if (lifes == 1)
		lifenumber->SetSpritesData({ 1355,1950,36,50 });
	else if (lifes == 0)
		lifenumber->SetSpritesData({ 1399,1950,47,50 });
	else
		lifenumber->SetSpritesData({ NULL });
}

void Player::TIME()
{
	
	CurrentTime = SDL_GetTicks() - App->scene->Delay -  LoadDelay;
	if ((CurrentTime - TimeSinceStarted) / 1000 >= 60) {
		Min++;
		TimeSinceStarted = SDL_GetTicks();
		LoadDelay = 0;
		App->scene->Delay = 0;
	}
	if (Min >= 60) {
		Hours++;
		Min = 0;
	}
	if (Min > 9&&((CurrentTime - TimeSinceStarted) / 1000)>9)
		StringTime.create("0%i : %i : %i", Hours, Min, (CurrentTime - TimeSinceStarted) / 1000);
	else if (Min > 9 && ((CurrentTime - TimeSinceStarted) / 1000) <= 9)
		StringTime.create("0%i : %i : 0%i", Hours, Min, (CurrentTime - TimeSinceStarted) / 1000);
	else if (Min <= 9 && ((CurrentTime - TimeSinceStarted) / 1000) <= 9)
		StringTime.create("0%i : 0%i : 0%i", Hours, Min, (CurrentTime - TimeSinceStarted) / 1000);
	else 
		StringTime.create("0%i : 0%i : %i", Hours, Min, (CurrentTime - TimeSinceStarted) / 1000);
	TimeStart->ChangeLabel(StringTime.GetString(), 50);
	
}

void Player::CountCoins()
{

	Coins.create("%i", App->scene->NumberCoins);
	NumCoins->ChangeLabel(Coins.GetString(), 50);
}

