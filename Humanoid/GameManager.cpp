#include "GameManager.h"
#include "stdlib.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

Color GameManager::GetColor(int combo)
{
	switch (combo)
	{
	case 3:
		return RED;
	case 4:
		return GREEN;
	case 5:
		return YELLOW;
	case 6:
		return BLUE;
	case 7:
		return MAROON;
	case 8:
		return DARKGREEN;
	case 9:
		return WHITE;
	default:
		return MAGENTA;
	}
}

bool GameManager::onClickEvent(Rectangle btn)
{
	if (IsMouseButtonReleased(0))
	{
		if (CheckCollisionPointRec(GetMousePosition(), btn))
			return true;
		else
			return false;
	}
}

int GameManager::ActivatePowerUp()
{
	if (RandomBool())
		return rand() % 5 + 1;
	else return 0;
}

void GameManager::PlayComboSfx(Sound* sfx, float pitch)
{
	pitch = 0.02f * pitch + 1.f;
	SetSoundPitch(*sfx, pitch);
	PlaySound(*sfx);
}

bool GameManager::RandomBool()
{
	int i = rand() % 100 + 1;

	if (i % 10 == 0)
		return true;
	else
		return false;
}