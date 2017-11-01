#pragma once
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleScore : public Module
{
public:

	ModuleScore(Application* app, bool start_enabled = true);
	~ModuleScore();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* lives_title = nullptr;
	SDL_Texture* score_title = nullptr;

	SDL_Texture* zero = nullptr;
	SDL_Texture* one = nullptr;
	SDL_Texture* two = nullptr;
	SDL_Texture* three = nullptr;
	SDL_Texture* four = nullptr;
	SDL_Texture* five = nullptr;
	SDL_Texture* six = nullptr;
	SDL_Texture* seven = nullptr;
	SDL_Texture* eight = nullptr;
	SDL_Texture* nine = nullptr;

	int number_pos1;
	int number_pos2;
	int number_pos3;
	int number_pos4;

	int current_score;
	int lives;
	int max_score;

};