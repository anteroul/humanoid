#pragma once

#include "raylib.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	static Color GetColor(int combo);
	static bool onClickEvent(Rectangle btn);
	static int ActivatePowerUp();
private:
	static bool RandomBool();
};