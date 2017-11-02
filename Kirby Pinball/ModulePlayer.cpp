#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleScore.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	CreatePlayerBall();

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
	
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		App->score->lives = 3;
		App->score->current_score = 0;
		App->score->number_pos1 = 0;
		App->score->number_pos2 = 0;
		App->score->number_pos3 = 0;
		App->score->number_pos4 = 0;
		App->scene_intro->game_over_bool = false;
	}

	int x, y;
	ball->GetPosition(x, y);
	if (y>200)
	{
		App->score->lives--;
		CreatePlayerBall();
	}

	if (App->score->lives == 0)
	{
		App->scene_intro->game_over_bool = true;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::CreatePlayerBall()
{
	ball = App->physics->CreateCircle(125, 170, 2.5f);
}



