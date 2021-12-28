// (C) Uljas Antero Lindell 2021
// Version 0.9.8.1 Beta

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "arkanoid.h"
#include "GameManager.h"
#include "GUI.h"
#include "levels.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main()
{
    InitWindow(screenWidth, screenHeight, "Humanoid");

    appIcon = LoadImage("icons/icon.png");
    SetWindowIcon(appIcon);

    if (!frameLimiter.enabled) SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    else SetTargetFPS(60);

    StartUp();
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (gameState == QUIT)      // Break the game loop if gameState is set to 'QUIT'
            break;
        UpdateDrawFrame();
    }

    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseWindow();        // Close window and OpenGL context

    return 0;
}

// Set up everything. Load necessary game files.
void StartUp(void)
{
    InitAudioDevice();
    btnSfx = LoadSound("sounds/button.wav");
    comboSfx = LoadSound("sounds/coin.wav");
    hitSfx = LoadSound("sounds/hit.wav");
    extraLife = LoadSound("sounds/1up.wav");
    loseLife = LoadSound("sounds/fail.wav");
    beginSfx = LoadSound("sounds/ready.wav");
    gameOverSfx = LoadSound("sounds/game_over.wav");
    shoot = LoadSound("sounds/shoot.wav");
    ship = LoadTexture("graphics/ship.png");
    background = LoadTexture("graphics/background.png");
    turret = LoadTexture("graphics/turret.png");
    gameState = MENU;
}

void UpdateMenu(void)
{
    if (gameState == MENU)
    {
        if (GameManager::onClickEvent(play_btn.btn_rect, btnSfx))
            gameState = GAMEPLAY;
        if (GameManager::onClickEvent(conf_btn.btn_rect, btnSfx))
            gameState = SETTINGS;
        if (GameManager::onClickEvent(exit_btn.btn_rect, btnSfx))
            gameState = QUIT;
    }
    else if (gameState == SETTINGS)
    {
        if (GameManager::onClickEvent(fullScreen.btn_rect, btnSfx))
        {
            if (!fullScreen.enabled)
                fullScreen.enabled = true;
            else
                fullScreen.enabled = false;
            ToggleFullscreen();
        }
        if (GameManager::onClickEvent(frameLimiter.btn_rect, btnSfx))
        {
            if (!frameLimiter.enabled)
            {
                frameLimiter.enabled = true;
                SetTargetFPS(60);
            }
            else
            {
                frameLimiter.enabled = false;
                SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
            }
        }
        if (GameManager::onClickEvent(play_btn_copy.btn_rect, btnSfx))
            gameState = GAMEPLAY;
        if (GameManager::onClickEvent(exit_btn.btn_rect, btnSfx))
            gameState = QUIT;
    }
}

void InitGame(void)
{
    brickSize = Vector2 { screenWidth / BRICKS_PER_LINE, 40 };

    // Initialize player
    player.position = Vector2 { screenWidth / 2, screenHeight * 7 / 8 };
    player.size = Vector2 { (float) ship.width, (float) ship.height};

    if (level == 1)
        player.life = DEFAULT_LIFE;

    // Initialize ball
    ball.position = Vector2 { screenWidth / 2, screenHeight * 7 / 8 - 30 };
    ball.speed = Vector2 { 0, 0 };
    ball.radius = 7;
    ball.active = false;

    comboMultiplier = 1;
    powerup = 0;
    sizeMultiplier = 1;
    stickyMode = false;
    superBallMode = false;

    // Initialize bricks
    int initialDownPosition = 50;
    bricks = 0;

    for (int i = 0; i < LINES_OF_BRICKS; i++)
    {
        for (int j = 0; j < BRICKS_PER_LINE; j++)
        {
            brick[i][j].position = Vector2 { j * brickSize.x + brickSize.x / 2, i * brickSize.y + initialDownPosition };
            brick[i][j].active = true;
            brick[i][j].brickType = brickMap[level][i][j];
            if (brick[i][j].brickType != 5)
                bricks++;
        }
    }

    if (ammo <= 0)
    {
        for (int i = 0; i < MAX_AMMO; i++)
        {
            projectile[i].active = false;
        }
    }
    PlaySound(beginSfx);
    levelReady = true;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        player.size.x = ship.width * sizeMultiplier;

        if (level == LEVELS)
        {
            if (score > GameManager::ReadScore())
                GameManager::SubmitScore(score);
            gameOver = true;
        }

        if (!superBallMode) damage = 2;
        else damage = 4;

        if (bricks <= 0)
        {
            level++;
            levelReady = false;
        }

        if (IsKeyPressed('P'))
        {
            pause = !pause;
            PlaySound(btnSfx);
        }

        if (!pause)
        {
            // Player movement logic
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.position.x -= 5;
            if ((player.position.x - player.size.x / 2) <= 0) player.position.x = player.size.x / 2;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.position.x += 5;
            if ((player.position.x + player.size.x / 2) >= screenWidth) player.position.x = screenWidth - player.size.x / 2;

            // Ball launching logic
            if (!ball.active)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    ball.active = true;
                    ball.speed = Vector2{ 0, -5 };
                }
            }
            else
            {
                if (IsKeyPressed(KEY_SPACE) && ammo > 0)
                {
                    projectile[ammo].active = true;
                    projectile[ammo].rect = { player.position.x - 3, player.position.y - 30, 5, 10 };
                    ammo--;
                    PlaySound(shoot);
                }
            }

            // Projectile logic
            for (int i = 0; i < 30; i++)
            {
                if (projectile[i].active)
                {
                    if (projectile[i].rect.y < 0) projectile[i].active = false;
                    projectile[i].rect.y -= 15;
                }
            }

            // Ball movement logic
            if (ball.active)
            {
                if (stickyMode)
                {
                    if (ball.position.x > player.position.x) ball.position.x -= 0.75f;
                    if (ball.position.x < player.position.x) ball.position.x += 0.75f;
                }
                if (ball.position.x - ball.radius < 0) ball.position.x++;
                if (ball.position.x + ball.radius > GetScreenWidth()) ball.position.x--;
                ball.position.x += ball.speed.x;
                ball.position.y += ball.speed.y;
            }
            else
            {
                ball.position = Vector2{ player.position.x, screenHeight * 7 / 8 - 30 };
            }

            // Collision logic: ball vs walls
            if (((ball.position.x + ball.radius) >= screenWidth) || ((ball.position.x - ball.radius) <= 0))
            {
                ball.speed.x *= -1;
                PlaySound(hitSfx);
            }
            if ((ball.position.y - ball.radius) <= 0)
            {
                ball.speed.y *= -1;
                PlaySound(hitSfx);
            }
            if ((ball.position.y + ball.radius) >= screenHeight)
            {
                ball.speed = Vector2{ 0, 0 };
                ball.active = false;
                stickyMode = false;
                superBallMode = false;
                comboMultiplier = 1;
                player.life--;
                sizeMultiplier = 1;
                ammo = 0;
                PlaySound(loseLife);
            }

            // Collision logic: ball vs player
            if (CheckCollisionCircleRec(ball.position, ball.radius,
                Rectangle{
                player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y
                }))
            {
                if (ball.speed.y > 0)
                {
                    ball.speed.y *= -1;
                    ball.speed.x = (ball.position.x - player.position.x) / (player.size.x / 2) * 5;
                    comboMultiplier = 1;
                    PlaySound(hitSfx);
                }
            }

            // Collision logic: ball vs bricks
            for (int i = 0; i < LINES_OF_BRICKS; i++)
            {
                for (int j = 0; j < BRICKS_PER_LINE; j++)
                {
                    if (brick[i][j].active)
                    {
                        // Brick colors
                        switch (brick[i][j].brickType)
                        {
                        case 1:
                            brick[i][j].color = YELLOW;
                            break;
                        case 2:
                            brick[i][j].color = GREEN;
                            break;
                        case 3:
                            brick[i][j].color = BLUE;
                            break;
                        case 4:
                            brick[i][j].color = MAGENTA;
                            break;
                        case 5:
                            brick[i][j].color = DARKGRAY;
                            break;
                        default:
                            // Destroy brick if brick type is downgraded below type 1
                            bricks--;
                            brick[i][j].active = false;
                            powerup = GameManager::ActivatePowerUp();
                        }

                        // Projectile hit
                        for (int k = 0; k < 30; k++)
                        {
                            if (CheckCollisionRecs(projectile[k].rect, { brick[i][j].position.x, brick[i][j].position.y, brickSize.x, brickSize.y }) && projectile[k].active)
                            {
                                brick[i][j].brickType -= 2;
                                score += 100;
                                GameManager::PlayComboSfx(comboSfx, 1);
                                projectile[k].active = false;
                            }
                        }

                        // Hit below
                        if (((ball.position.y - ball.radius) <= (brick[i][j].position.y + brickSize.y / 2)) &&
                            ((ball.position.y - ball.radius) > (brick[i][j].position.y + brickSize.y / 2 + ball.speed.y)) &&
                            ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x / 2 + ball.radius * 2 / 3)) && (ball.speed.y < 0))
                        {
                            if (brick[i][j].brickType != 5)
                            {
                                brick[i][j].brickType -= damage;
                                score += 100 * comboMultiplier;
                                comboMultiplier++;
                                GameManager::PlayComboSfx(comboSfx, comboMultiplier);
                            }
                            else PlaySound(hitSfx);
                            ball.speed.y *= -1;
                        }
                        // Hit above
                        else if (((ball.position.y + ball.radius) >= (brick[i][j].position.y - brickSize.y / 2)) &&
                            ((ball.position.y + ball.radius) < (brick[i][j].position.y - brickSize.y / 2 + ball.speed.y)) &&
                            ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x / 2 + ball.radius * 2 / 3)) && (ball.speed.y > 0))
                        {
                            if (brick[i][j].brickType != 5)
                            {
                                brick[i][j].brickType -= damage;
                                score += 100 * comboMultiplier;
                                comboMultiplier++;
                                GameManager::PlayComboSfx(comboSfx, comboMultiplier);
                            }
                            else PlaySound(hitSfx);
                            ball.speed.y *= -1;
                        }
                        // Hit left
                        else if (((ball.position.x + ball.radius) >= (brick[i][j].position.x - brickSize.x / 2)) &&
                            ((ball.position.x + ball.radius) < (brick[i][j].position.x - brickSize.x / 2 + ball.speed.x)) &&
                            ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y / 2 + ball.radius * 2 / 3)) && (ball.speed.x > 0))
                        {
                            if (brick[i][j].brickType != 5)
                            {
                                brick[i][j].brickType -= damage;
                                score += 100 * comboMultiplier;
                                comboMultiplier++;
                                GameManager::PlayComboSfx(comboSfx, comboMultiplier);
                            }
                            else PlaySound(hitSfx);
                            ball.speed.x *= -1;
                        }
                        // Hit right
                        else if (((ball.position.x - ball.radius) <= (brick[i][j].position.x + brickSize.x / 2)) &&
                            ((ball.position.x - ball.radius) > (brick[i][j].position.x + brickSize.x / 2 + ball.speed.x)) &&
                            ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y / 2 + ball.radius * 2 / 3)) && (ball.speed.x < 0))
                        {
                            if (brick[i][j].brickType != 5)
                            {
                                brick[i][j].brickType -= damage;
                                score += 100 * comboMultiplier;
                                comboMultiplier++;
                                GameManager::PlayComboSfx(comboSfx, comboMultiplier);
                            }
                            else PlaySound(hitSfx);
                            ball.speed.x *= -1;
                        }
                    }
                }
            }

            // TODO: Improve power-ups.
            // 
            // Power-up logic
            switch (powerup)
            {
            case EXTRA_LIFE:
                player.life++;
                PlaySound(extraLife);
                powerup = NONE;
                break;
            case WIDE:
                sizeMultiplier = 2;
                stickyMode = false;
                superBallMode = false;
                PlaySound(extraLife);
                powerup = NONE;
                break;
            case SHOOT:
                ammo = MAX_AMMO - 1;
                PlaySound(extraLife);
                powerup = NONE;
                break;
            case MAGNETIC:
                sizeMultiplier = 1;
                superBallMode = false;
                stickyMode = true;
                PlaySound(extraLife);
                powerup = NONE;
                break;
            case SUPERBALL:
                sizeMultiplier = 1;
                stickyMode = false;
                superBallMode = true;
                PlaySound(extraLife);
                powerup = NONE;
                break;
            default:
                break;
            }

            // Game over logic
            if (player.life <= 0 && gameOver == false)
            {
                if (score > GameManager::ReadScore())
                    GameManager::SubmitScore(score);
                gameOver = true;
                PlaySound(gameOverSfx);
            }
            else
            {
                for (int i = 0; i < LINES_OF_BRICKS; i++)
                {
                    for (int j = 0; j < BRICKS_PER_LINE; j++)
                    {
                        if (brick[i][j].active) gameOver = false;
                    }
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            score = 0;
            level = 1;
            InitGame();
            gameOver = false;
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            score = 0;
            level = 1;
            levelReady = false;
            gameOver = false;
            gameState = MENU;
        }
    }
}

// Draw game
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(SKYBLUE);
    DrawTexture(background, 0, 0, WHITE);

    switch (gameState)
    {
    case MENU:              // Draw Main Menu

        DrawRectangle(play_btn.btn_rect.x, play_btn.btn_rect.y, play_btn.btn_rect.width, play_btn.btn_rect.height, BLACK);
        DrawRectangle(conf_btn.btn_rect.x, conf_btn.btn_rect.y, conf_btn.btn_rect.width, conf_btn.btn_rect.height, BLACK);
        DrawRectangle(exit_btn.btn_rect.x, exit_btn.btn_rect.y, exit_btn.btn_rect.width, exit_btn.btn_rect.height, BLACK);

        DrawText(play_btn.text, play_btn.btn_pos.x, play_btn.btn_pos.y, 40, GREEN);
        DrawText(conf_btn.text, conf_btn.btn_pos.x, conf_btn.btn_pos.y, 40, GREEN);
        DrawText(exit_btn.text, exit_btn.btn_pos.x, exit_btn.btn_pos.y, 40, GREEN);
        
        DrawText("v0.9.8.1 beta", 5, GetScreenHeight() - 20, 10, GREEN);
        break;
    case SETTINGS:          // Draw Settings Screen

        if (frameLimiter.enabled) DrawText(frameLimiter.text, frameLimiter.btn_rect.x, frameLimiter.btn_rect.y, 20, RED);
        else DrawText(frameLimiter.text, frameLimiter.btn_rect.x, frameLimiter.btn_rect.y, 20, WHITE);

        if (fullScreen.enabled) DrawText(fullScreen.text, fullScreen.btn_rect.x, fullScreen.btn_rect.y, 20, RED);
        else DrawText(fullScreen.text, fullScreen.btn_rect.x, fullScreen.btn_rect.y, 20, WHITE);

        DrawRectangle(play_btn_copy.btn_rect.x, play_btn_copy.btn_rect.y, play_btn_copy.btn_rect.width, play_btn_copy.btn_rect.height, BLACK);
        DrawRectangle(exit_btn.btn_rect.x, exit_btn.btn_rect.y, exit_btn.btn_rect.width, exit_btn.btn_rect.height, BLACK);

        DrawText(play_btn_copy.text, play_btn_copy.btn_pos.x, play_btn_copy.btn_pos.y, 40, GREEN);
        DrawText(exit_btn.text, exit_btn.btn_pos.x, exit_btn.btn_pos.y, 40, GREEN);

        break;
    case GAMEPLAY:          // Draw Gameplay
        if (!gameOver)
        {
            // Draw player bar
            if (!stickyMode)
                DrawTextureRec(ship, {0, 0, player.size.x, player.size.y}, { player.position.x - player.size.x / 2, player.position.y - player.size.y / 2 }, WHITE);
            else
                DrawTextureRec(ship, { 0, 0, player.size.x, player.size.y }, { player.position.x - player.size.x / 2, player.position.y - player.size.y / 2 }, GREEN);

            if (ammo > 0)
                DrawTexture(turret, player.position.x - turret.width / 2, player.position.y - 30, WHITE);

            // Draw player lives
            for (int i = 0; i < player.life; i++) DrawRectangle(20 + 40 * i, screenHeight - 30, 35, 10, RED);

            // Draw projectiles
            for (int i = 0; i < 30; i++) if (projectile[i].active) DrawRectangle(projectile[i].rect.x, projectile[i].rect.y, projectile[i].rect.width, projectile[i].rect.height, WHITE);

            // Draw ball
            if (!superBallMode) DrawCircleV(ball.position, ball.radius, WHITE);
            else DrawCircleV(ball.position, ball.radius, MAROON);

            // Draw bricks
            for (int i = 0; i < LINES_OF_BRICKS; i++)
            {
                for (int j = 0; j < BRICKS_PER_LINE; j++)
                {
                    if (brick[i][j].active)
                        DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, brick[i][j].color);
                }
            }

            // Draw score
            DrawText(TextFormat("%04i", score), screenWidth - 150, screenHeight - 50, 40, GREEN);

            if (comboMultiplier > 2)
                DrawText(TextFormat("%01ix Combo!", comboMultiplier - 1), GetScreenWidth() / 2 - 50, GetScreenHeight() / 2, 20, GameManager::GetColor(comboMultiplier));

            if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, WHITE);
        }
        else
        {
            DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 80, 20, GREEN);
            DrawText("PRESS [BACKSPACE] TO RETURN TO MAIN MENU", GetScreenWidth() / 2 - MeasureText("PRESS [BACKSPACE] TO RETURN TO MAIN MENU", 20) / 2, GetScreenHeight() / 2 - 50, 20, GREEN);

            if (score >= GameManager::ReadScore())
                DrawText("NEW HIGH SCORE!", GetScreenWidth() / 2 - MeasureText("NEW HIGH SCORE!", 20) / 2, GetScreenHeight() / 2, 20, WHITE);
            else
                DrawText(TextFormat("your high score: %04i", GameManager::ReadScore()), GetScreenWidth() / 2 - 120, GetScreenHeight() / 2, 20, GREEN);

            DrawText(TextFormat("%04i", score), GetScreenWidth() / 2 - MeasureText(TextFormat("%04i", score), 40) / 2, GetScreenHeight() / 2 + 40, 40, GREEN);
        }

        break;

    default:
        break;
    }
    DrawFPS(0, 0);
    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    UnloadSound(btnSfx);
    UnloadSound(comboSfx);
    UnloadSound(hitSfx);
    UnloadSound(extraLife);
    UnloadSound(loseLife);
    UnloadSound(beginSfx);
    UnloadSound(gameOverSfx);
    UnloadSound(shoot);
    UnloadTexture(ship);
    UnloadTexture(background);
    UnloadTexture(turret);
    UnloadImage(appIcon);
    CloseAudioDevice();
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    switch (gameState)
    {
    case MENU:
        UpdateMenu();
        break;
    case SETTINGS:
        UpdateMenu();
        break;
    case GAMEPLAY:
        if (!levelReady)
            InitGame();
        UpdateGame();
        break;
    }
   DrawGame();
}