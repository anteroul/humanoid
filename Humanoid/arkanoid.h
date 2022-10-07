#pragma once

#include <cmath>
#include <raylib.h>
#include "GameManager.h"
#include "GUI.h"
#include "Levels.h"

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

    struct Player {
        Vector2 position;
        Vector2 size;
        int life;
    };

    struct Ball {
        Vector2 position;
        Vector2 speed;
        int radius;
        bool active;
    };

    struct Brick {
        Vector2 position;
        bool active;
        int brickType;
        Color color;
    };

    struct Projectile {
        Rectangle rect;
        bool active;
    };

    int bricks;
    int gameState;
    int level = 1;
    int score = 0;
    int comboMultiplier = 1;
    int powerup = 0;
    int sizeMultiplier = 1;
    int ammo = 0;
    int damage = 2;
    bool levelReady = false;
    bool gameOver = false;
    bool pause = false;
    bool stickyMode = false;
    bool superBallMode = false;

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
};