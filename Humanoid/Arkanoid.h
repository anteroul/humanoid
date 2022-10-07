#pragma once

#include <cmath>
#include <raylib.h>
#include "GameManager.h"
#include "GameVariables.h"
#include "GUI.h"
#include "Levels.h"
#include "Structs.h"

#define DEFAULT_LIFE            3
#define MAX_AMMO               30

class Arkanoid
{
public:
    Arkanoid();
    ~Arkanoid();
    void RunApplication();
private:
    enum GameScreen { MENU, SETTINGS, GAMEPLAY, QUIT };
    enum PowerUps { NONE, EXTRA_LIFE, WIDE, SHOOT, MAGNETIC, SUPERBALL };

    GameVariables g = {};
    Player player = {};
    Ball ball = {};
    Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE] = {};
    Vector2 brickSize = {};
    Texture2D ship;
    Texture2D background;
    Texture2D turret;
    Projectile projectile[MAX_AMMO] = {};
    Sound comboSfx;
    Sound hitSfx;
    Sound beginSfx;
    Sound gameOverSfx;
    Sound extraLife;
    Sound loseLife;
    Sound shoot;

    void InitGame();         // Initialize game
    void UpdateMenu();       // Update main menu
    void UpdateGame();       // Update gameplay
    void DrawGame();         // Draw graphics
    void UnloadGame();       // Unload game
    void UpdateDrawFrame();  // Update and Draw (one frame)
    void ReadInput();        // Get input

	void UpdateBall();
    void UpdateBricks();
    void UpdatePowerUps();
    void HitHorizontal(Brick* brick);
    void HitVertical(Brick* brick);
};