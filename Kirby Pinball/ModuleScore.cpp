#include "Globals.h"
#include "Application.h"
#include "ModuleScore.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

ModuleScore::ModuleScore(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleScore::~ModuleScore()
{

}

bool ModuleScore::Start()
{
	bool ret = true;
	LOG("Loading scores");

	current_score = 0;
	max_score = 9999;
	lives = 3;

	number_pos1 = 0;
	number_pos2 = 0;
	number_pos3 = 0;
	number_pos4 = 0;

	zero = App->textures->Load("pinball/0.png");
	one = App->textures->Load("pinball/1.png");
	two = App->textures->Load("pinball/2.png");
	three = App->textures->Load("pinball/3.png");
	four = App->textures->Load("pinball/4.png");
	five = App->textures->Load("pinball/5.png");
	six = App->textures->Load("pinball/6.png");
	seven = App->textures->Load("pinball/7.png");
	eight = App->textures->Load("pinball/8.png");
	nine = App->textures->Load("pinball/9.png");

	return ret;
}

update_status ModuleScore::Update()
{
	int fake_score = current_score;

	number_pos1 = fake_score / 1000;
	fake_score = fake_score % 1000;

	number_pos2 = fake_score / 100;
	fake_score = fake_score % 100;

	number_pos3 = fake_score / 10;
	number_pos4 = fake_score % 10;

	if (current_score >= max_score)
	{
		number_pos1 = 9;
		number_pos2 = 9;
		number_pos3 = 9;
		number_pos4 = 9;
	}

	return UPDATE_CONTINUE;
}

bool ModuleScore::CleanUp()
{
	return true;
}