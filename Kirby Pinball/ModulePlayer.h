#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	PhysBody* CreatePlayerBall();

public:

	//ball
	PhysBody* ball = nullptr;

	//flippers
	PhysBody* flippers[2];
	PhysBody* flipper_anchor[2];
	PhysBody* player;
	b2RevoluteJoint* flipper_joints[2];
	SDL_Rect* flipper_sprites[2];

	//mini flippers
	PhysBody* mini_flippers[2];
	PhysBody* mini_flipper_anchor[2];
	b2RevoluteJoint* mini_flipper_joints[2];
	SDL_Rect* mini_flipper_sprites[2];

};