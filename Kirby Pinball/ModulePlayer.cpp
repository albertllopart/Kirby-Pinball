#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ball = CreatePlayerBall();


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		App->physics->KickersForce(b2Vec2(0, 50), b2Vec2(0, 0), LEFT);
	}
	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) )
	{
		App->physics->KickersForce(b2Vec2(0, -50), b2Vec2(0, 0), RIGHT);
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePlayer::CreatePlayerBall()
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(40, 65);

	b2Body* b = App->physics->world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = 30;
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 0.3f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 2.5f;

	return pbody;
}



