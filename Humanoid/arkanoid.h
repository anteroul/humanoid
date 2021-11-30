#pragma once

#include "raylib.h"

#define DEFAULT_LIFE            3
#define LEVELS                  6
#define LINES_OF_BRICKS         5
#define BRICKS_PER_LINE        15
#define MAX_AMMO               30


typedef enum GameScreen { MENU, SETTINGS, GAMEPLAY, QUIT } GameScreen;
typedef enum PowerUps { NONE, EXTRA_LIFE, WIDE, SHOOT, MAGNETIC, SUPERBALL } PowerUps;

typedef struct Player {
    Vector2 position;
    Vector2 size;
    int life;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    bool active;
} Ball;

typedef struct Brick {
    Vector2 position;
    bool active;
    int brickType;
    Color color;
} Brick;

typedef struct Projectile {
    Rectangle rect;
    bool active;
} Projectile;

static const int screenWidth = 800;
static const int screenHeight = 600;

static int bricks;
static int gameState;
static int level = 1;
static int score = 0;
static int comboMultiplier = 1;
static int powerup = 0;
static int sizeMultiplier = 1;
static int ammo = 0;
static int damage = 2;

static bool levelReady = false;
static bool gameOver = false;
static bool pause = false;
static bool stickyMode = false;
static bool superBallMode = false;

static Player player = { 0 };
static Ball ball = { 0 };
static Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE] = { 0 };
static Vector2 brickSize = { 0 };
static Texture2D ship;
static Texture2D background;
static Projectile projectile[MAX_AMMO] = { 0 };

static Sound comboSfx;
static Sound hitSfx;
static Sound beginSfx;
static Sound gameOverSfx;
static Sound extraLife;
static Sound loseLife;
static Sound shoot;

static void StartUp(void);          // Initialize game (application)
static void InitGame(void);         // Initialize game (gameplay)
static void UpdateMenu(void);       // Update main menu
static void UpdateGame(void);       // Update gameplay
static void DrawGame(void);         // Draw graphics
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)