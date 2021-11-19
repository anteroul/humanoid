#pragma once
#include "raylib.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	static int LevelUp(int level);
	static Color GetColor(int combo);
	static bool onClickEvent(Rectangle btn);
};