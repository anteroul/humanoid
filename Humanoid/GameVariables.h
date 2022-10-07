#pragma once

struct GameVariables
{
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
};
