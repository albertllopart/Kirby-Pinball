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
	ball = App->physics->CreateCircle(125, 170, 2.5f);

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
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ball->body->ApplyForce(b2Vec2(0, -150), b2Vec2(0, 0), true);
	}
	return UPDATE_CONTINUE;
}

PhysBody* ModulePlayer::CreatePlayerBall()
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(150, 200);

	b2Body* b = App->physics->world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = 30;
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 25.0f;
	fixture.restitution = 0.2f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 2.5f;

	return pbody;
}



