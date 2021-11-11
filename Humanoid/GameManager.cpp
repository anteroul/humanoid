#include "GameManager.h"
#include "arkanoid.h"

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
