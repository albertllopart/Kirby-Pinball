#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"

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

	//flippers
	int flipper_vertex[10] = {
		49, 180,
		49, 175,
		30, 167,
		27, 173,
		36, 177
	};

	b2Vec2 flipper_vertices[5];
	flipper_vertices[0].Set(49, 180);
	flipper_vertices[1].Set(49, 175);
	flipper_vertices[2].Set(30, 167);
	flipper_vertices[3].Set(27, 173);
	flipper_vertices[4].Set(36, 177);

	for (int i = 0; i < 5; i++) {
		flipper_vertices[i].x = PIXEL_TO_METERS(flipper_vertices[i].x);
		flipper_vertices[i].y = PIXEL_TO_METERS(flipper_vertices[i].y);
	}

	flippers[0] = App->physics->CreateFlipper(flipper_vertices, 5, 32); 

	flipper_anchor[0] = App->physics->CreateCircleObstacle(41, 184, 5);

	flipper_joints[0] = App->physics->CreateFlipperRevoluteJoint(flippers[0]->body, flipper_anchor[0]->body, { 0.13f,0.12f }, 20, -32);

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



