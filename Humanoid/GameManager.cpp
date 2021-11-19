#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

int GameManager::LevelUp(int level)
{
	return level++;
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