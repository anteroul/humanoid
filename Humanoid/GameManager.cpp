#include "GameManager.h"

Color GameManager::GetColor(int combo)
{
	switch (combo)
	{
	case 3: return RED;
	case 4: return GREEN;
	case 5: return YELLOW;
	case 6: return BLUE;
	case 7: return MAROON;
	case 8: return DARKGREEN;
	case 9: return WHITE;
	default: return MAGENTA;
	}
}

bool GameManager::onClickEvent(Rectangle btn, Sound& sfx)
{
	if (IsMouseButtonReleased(0))
	{
		if (CheckCollisionPointRec(GetMousePosition(), btn))
		{
			PlaySound(sfx);
			return true;
		}
	}
	return false;
}

int GameManager::ActivatePowerUp()
{
	if (RandomBool())
		return rand() % 5 + 1;

	return 0;
}

void GameManager::PlayComboSfx(const Sound& sfx, float pitch)
{
	pitch = 0.02f * pitch + 1.f;
	SetSoundPitch(sfx, pitch);
	PlaySound(sfx);
}

void GameManager::SubmitScore(int score)
{
	FILE* lboard;
	lboard = fopen("data/leaderboards.bin", "wb");
	fseek(lboard, 0, SEEK_END);
	fwrite(&score, sizeof(int), 1, lboard);
	fclose(lboard);
}

int GameManager::ReadScore()
{
	FILE* lboard;
	int score = 0;
	lboard = fopen("data/leaderboards.bin", "rb");
	fread((char*)&score, sizeof(int), 1, lboard);
	fclose(lboard);
	return score;
}

void GameManager::ReadVersion(char& v)
{
	FILE* file;
	size_t nread;
	file = fopen("data/version.txt", "r");
	if (file) {
		while ((nread = fread(&v, 5, sizeof v, file)) > 0)
			fwrite(&v, 1, nread, stdout);
		fclose(file);
	}
}

bool GameManager::RandomBool()
{
	int i = rand() % 100 + 1;

	if (i % 10 == 0)
		return true;

	return false;
}