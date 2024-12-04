#pragma once

#include <raylib.h>
#include <cstdlib>
#include <cstdio>

class GameManager
{
public:
	static Color GetColor(int combo);
	static bool onClickEvent(Rectangle btn, Sound& sfx);
	static int ActivatePowerUp();
	static void PlayComboSfx(const Sound& sfx, float pitch);
	static void SubmitScore(int score);
	static int ReadScore();
	static void ReadVersion(char& v);
private:
	static bool RandomBool();
};