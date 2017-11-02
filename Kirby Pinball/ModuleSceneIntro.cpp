#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleScore.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	left_kicker = circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	left_kicker = App->textures->Load("pinball/Pala1.png");
	circle = App->textures->Load("pinball/Ball.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	map_texture = App->textures->Load("pinball/Map.png");
	game_over = App->textures->Load("pinball/GAME OVER.png");

	
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//obstacles
	circle_obstacles.add(App->physics->CreateCircleObstacle(34, 60, 6.5f));
	circle_obstacles.getLast()->data->listener = this;

	circle_obstacles.add(App->physics->CreateCircleObstacle(98, 60, 6.5f));
	circle_obstacles.getLast()->data->listener = this;

	circle_obstacles.add(App->physics->CreateCircleObstacle(58, 43, 6.5f));
	circle_obstacles.getLast()->data->listener = this;

	circle_obstacles.add(App->physics->CreateCircleObstacle(90, 83, 6.5f));
	circle_obstacles.getLast()->data->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(map_texture, 10, 10);
	if(App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 2.5f));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(40, 65, 2.5f));
		circles.getLast()->data->listener = this;
		App->player->ball = circles.getLast()->data;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if (App->player->ball != NULL)
		{
			p2List_item<PhysBody*>* c = circles.getFirst();
			while (c->data != App->player->ball)
			{
				c = c->next;
			}
			circles.del(c);
		}	
	}

	

	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	

	//
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	//player
	int x, y;
	App->player->ball->GetPosition(x, y);
	App->renderer->Blit(circle, x, y, NULL, 1.0f, App->player->ball->GetRotation());

	//kickers
	/*int kicker_x, kicker_y;
	p2List_item<PhysBody*>* item = App->physics->GetLeftKickers()->getFirst();
	while (item != nullptr)
	{
		item->data->GetPosition(kicker_x, kicker_y);
		App->renderer->Blit(left_kicker, kicker_x, kicker_y, NULL, 1.0F, item->data->GetRotation());
	}*/
	

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//DRAW SCORE

	switch (App->score->number_pos1)
	{
	case 0:
		App->renderer->Blit(App->score->zero, 140, 10);
		break;
	case 1:
		App->renderer->Blit(App->score->one, 140, 10);
		break;
	case 2:
		App->renderer->Blit(App->score->two, 140, 10);
		break;
	case 3:
		App->renderer->Blit(App->score->three, 140, 10);
		break;
	case 4:
		App->renderer->Blit(App->score->four, 140, 10);
		break;
	case 5:
		App->renderer->Blit(App->score->five, 140, 10);
		break;
	case 6:
		App->renderer->Blit(App->score->six, 140, 10);
		break;
	case 7:
		App->renderer->Blit(App->score->seven, 140, 10);
		break;
	case 8:
		App->renderer->Blit(App->score->eight, 140, 10);
		break;
	case 9:
		App->renderer->Blit(App->score->nine, 140, 10);
		break;
	}

	switch (App->score->number_pos2)
	{
	case 0:
		App->renderer->Blit(App->score->zero, 150, 10);
		break;
	case 1:
		App->renderer->Blit(App->score->one, 150, 10);
		break;
	case 2:
		App->renderer->Blit(App->score->two, 150, 10);
		break;
	case 3:
		App->renderer->Blit(App->score->three, 150, 10);
		break;
	case 4:
		App->renderer->Blit(App->score->four, 150, 10);
		break;
	case 5:
		App->renderer->Blit(App->score->five, 150, 10);
		break;
	case 6:
		App->renderer->Blit(App->score->six, 150, 10);
		break;
	case 7:
		App->renderer->Blit(App->score->seven, 150, 10);
		break;
	case 8:
		App->renderer->Blit(App->score->eight, 150, 10);
		break;
	case 9:
		App->renderer->Blit(App->score->nine, 150, 10);
		break;
	}

	switch (App->score->number_pos3)
	{
	case 0:
		App->renderer->Blit(App->score->zero, 160, 10);
		break;
	case 1:
		App->renderer->Blit(App->score->one, 160, 10);
		break;
	case 2:
		App->renderer->Blit(App->score->two, 160, 10);
		break;
	case 3:
		App->renderer->Blit(App->score->three, 160, 10);
		break;
	case 4:
		App->renderer->Blit(App->score->four, 160, 10);
		break;
	case 5:
		App->renderer->Blit(App->score->five, 160, 10);
		break;
	case 6:
		App->renderer->Blit(App->score->six, 160, 10);
		break;
	case 7:
		App->renderer->Blit(App->score->seven, 160, 10);
		break;
	case 8:
		App->renderer->Blit(App->score->eight, 160, 10);
		break;
	case 9:
		App->renderer->Blit(App->score->nine, 160, 10);
		break;
	}

	switch (App->score->number_pos4)
	{
	case 0:
		App->renderer->Blit(App->score->zero, 170, 10);
		break;
	case 1:
		App->renderer->Blit(App->score->one, 170, 10);
		break;
	case 2:
		App->renderer->Blit(App->score->two, 170, 10);
		break;
	case 3:
		App->renderer->Blit(App->score->three, 170, 10);
		break;
	case 4:
		App->renderer->Blit(App->score->four, 170, 10);
		break;
	case 5:
		App->renderer->Blit(App->score->five, 170, 10);
		break;
	case 6:
		App->renderer->Blit(App->score->six, 170, 10);
		break;
	case 7:
		App->renderer->Blit(App->score->seven, 170, 10);
		break;
	case 8:
		App->renderer->Blit(App->score->eight, 170, 10);
		break;
	case 9:
		App->renderer->Blit(App->score->nine, 170, 10);
		break;
	}

	switch (App->score->lives)
	{
	case 0:
		App->renderer->Blit(App->score->zero, 170, 30);
		break;
	case 1:
		App->renderer->Blit(App->score->one, 170, 30);
		break;
	case 2:
		App->renderer->Blit(App->score->two, 170, 30);
		break;
	case 3:
		App->renderer->Blit(App->score->three, 170, 30);
		break;
	case 4:
		App->renderer->Blit(App->score->four, 170, 30);
		break;
	case 5:
		App->renderer->Blit(App->score->five, 170, 30);
		break;
	case 6:
		App->renderer->Blit(App->score->six, 170, 30);
		break;
	case 7:
		App->renderer->Blit(App->score->seven, 170, 30);
		break;
	case 8:
		App->renderer->Blit(App->score->eight, 170, 30);
		break;
	case 9:
		App->renderer->Blit(App->score->nine, 170, 30);
		break;
	}

	App->renderer->Blit(App->score->score_title, 185, 10);
	App->renderer->Blit(App->score->lives_title, 185, 30);

	if (game_over_bool)
	{
		App->renderer->Blit(App->scene_intro->game_over, 10, 10);
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	p2List_item<PhysBody*>* c = circle_obstacles.getFirst();

	while (c != NULL)
	{
		if (bodyA == c->data)
		{
			App->audio->PlayFx(bonus_fx);
			App->score->current_score += 9;
		}
		c = c->next;
	}

	
	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
