#pragma once

#include "raylib.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	static Color GetColor(int combo);
	static bool onClickEvent(Rectangle btn, Sound& sfx);
	static int ActivatePowerUp();
	static void PlayComboSfx(Sound& sfx, float pitch);
	static void SubmitScore(int score);
	static int ReadScore();
private:
	static bool RandomBool();
};