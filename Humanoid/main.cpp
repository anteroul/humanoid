#include "arkanoid.h"
#include "GameManager.h"
#include "GUI.h"
#include "levels.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(void)
{
    InitWindow(screenWidth, screenHeight, "Humanoid");
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

void StartUp(void)
{
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    gameState = MENU;
}

void UpdateMenu(void)
{
    if (gameState == MENU)
    {
        if (GameManager::onClickEvent(play_btn.btn_rect))
            gameState = GAMEPLAY;
        if (GameManager::onClickEvent(conf_btn.btn_rect))
            gameState = SETTINGS;
        if (GameManager::onClickEvent(exit_btn.btn_rect))
            gameState = QUIT;
    }
    else if (gameState == SETTINGS)
    {
        if (GameManager::onClickEvent(fullScreen.btn_rect))
        {
            if (!fullScreen.enabled)
                fullScreen.enabled = true;
            else
                fullScreen.enabled = false;
            ToggleFullscreen();
        }
        if (GameManager::onClickEvent(frameLimiter.btn_rect))
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
        if (GameManager::onClickEvent(play_btn_copy.btn_rect))
            gameState = GAMEPLAY;
        if (GameManager::onClickEvent(exit_btn.btn_rect))
            gameState = QUIT;
    }
}

void InitGame(void)
{
    brickSize = Vector2 { screenWidth / BRICKS_PER_LINE, 40 };

    // Initialize player
    player.position = Vector2 { screenWidth / 2, screenHeight * 7 / 8 };
    player.size = Vector2 { screenWidth / 10, 20 };

    if (level == 1)
        player.life = PLAYER_MAX_LIFE;

    // Initialize ball
    ball.position = Vector2 { screenWidth / 2, screenHeight * 7 / 8 - 30 };
    ball.speed = Vector2 { 0, 0 };
    ball.radius = 7;
    ball.active = false;

    comboMultiplier = 1;

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

    levelReady = true;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (bricks <= 0)
        {
            level++;
            levelReady = false;
        }

        if (level > 8) gameOver = true;

        if (!pause)
        {
            // Player movement logic
            if (IsKeyDown(KEY_LEFT)) player.position.x -= 5;
            if ((player.position.x - player.size.x / 2) <= 0) player.position.x = player.size.x / 2;
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 5;
            if ((player.position.x + player.size.x / 2) >= screenWidth) player.position.x = screenWidth - player.size.x / 2;

            // Ball launching logic
            if (!ball.active)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    ball.active = true;
                    ball.speed = Vector2 { 0, -5 };
                }
            }

            // Ball movement logic
            if (ball.active)
            {
                ball.position.x += ball.speed.x;
                ball.position.y += ball.speed.y;
            }
            else
            {
                ball.position = Vector2 { player.position.x, screenHeight * 7 / 8 - 30 };
            }

            // Collision logic: ball vs walls
            if (((ball.position.x + ball.radius) >= screenWidth) || ((ball.position.x - ball.radius) <= 0))
                ball.speed.x *= -1;
            if ((ball.position.y - ball.radius) <= 0)
                ball.speed.y *= -1;
            if ((ball.position.y + ball.radius) >= screenHeight)
            {
                ball.speed = Vector2 { 0, 0 };
                ball.active = false;
                comboMultiplier = 1;
                player.life--;
            }

            // Collision logic: ball vs player
            if (CheckCollisionCircleRec(ball.position, ball.radius,
                Rectangle {
                player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y
            }))
            {
                if (ball.speed.y > 0)
                {
                    ball.speed.y *= -1;
                    ball.speed.x = (ball.position.x - player.position.x) / (player.size.x / 2) * 5;
                    comboMultiplier = 1;
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
                            if (brick[i][j].brickType == 1) brick[i][j].color = YELLOW;
                            else if (brick[i][j].brickType == 2) brick[i][j].color = GREEN;
                            else if (brick[i][j].brickType == 3) brick[i][j].color = BLUE;
                            else if (brick[i][j].brickType == 4) brick[i][j].color = MAGENTA;
                            else if (brick[i][j].brickType == 5) brick[i][j].color = DARKGRAY;
                            // Destroy brick if brick type is downgraded below type 1
                            else
                            {
                                bricks--;
                                brick[i][j].active = false;
                            }

                            // Hit below
                            if (((ball.position.y - ball.radius) <= (brick[i][j].position.y + brickSize.y / 2)) &&
                                ((ball.position.y - ball.radius) > (brick[i][j].position.y + brickSize.y / 2 + ball.speed.y)) &&
                                ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x / 2 + ball.radius * 2 / 3)) && (ball.speed.y < 0))
                            {
                                if (brick[i][j].brickType != 5)
                                {
                                    brick[i][j].brickType -= 2;
                                    score += 100 * comboMultiplier;
                                    comboMultiplier++;
                                }
                                ball.speed.y *= -1;
                            }
                            // Hit above
                            else if (((ball.position.y + ball.radius) >= (brick[i][j].position.y - brickSize.y / 2)) &&
                                ((ball.position.y + ball.radius) < (brick[i][j].position.y - brickSize.y / 2 + ball.speed.y)) &&
                                ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x / 2 + ball.radius * 2 / 3)) && (ball.speed.y > 0))
                            {
                                if (brick[i][j].brickType != 5)
                                {
                                    brick[i][j].brickType -= 2;
                                    score += 100 * comboMultiplier;
                                    comboMultiplier++;
                                }
                                ball.speed.y *= -1;
                            }
                            // Hit left
                            else if (((ball.position.x + ball.radius) >= (brick[i][j].position.x - brickSize.x / 2)) &&
                                ((ball.position.x + ball.radius) < (brick[i][j].position.x - brickSize.x / 2 + ball.speed.x)) &&
                                ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y / 2 + ball.radius * 2 / 3)) && (ball.speed.x > 0))
                            {
                                if (brick[i][j].brickType != 5)
                                {
                                    brick[i][j].brickType -= 2;
                                    score += 100 * comboMultiplier;
                                    comboMultiplier++;
                                }
                                ball.speed.x *= -1;
                            }
                            // Hit right
                            else if (((ball.position.x - ball.radius) <= (brick[i][j].position.x + brickSize.x / 2)) &&
                                ((ball.position.x - ball.radius) > (brick[i][j].position.x + brickSize.x / 2 + ball.speed.x)) &&
                                ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y / 2 + ball.radius * 2 / 3)) && (ball.speed.x < 0))
                            {
                                if (brick[i][j].brickType != 5)
                                {
                                    brick[i][j].brickType -= 2;
                                    score += 100 * comboMultiplier;
                                    comboMultiplier++;
                                }
                                ball.speed.x *= -1;
                            }
                        }
                    }
                }

                // Game over logic
                if (player.life <= 0) gameOver = true;
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

    switch (gameState)
    {
    case MENU:              // Draw Main Menu

        DrawRectangle(play_btn.btn_rect.x, play_btn.btn_rect.y, play_btn.btn_rect.width, play_btn.btn_rect.height, RAYWHITE);
        DrawRectangle(conf_btn.btn_rect.x, conf_btn.btn_rect.y, conf_btn.btn_rect.width, conf_btn.btn_rect.height, RAYWHITE);
        DrawRectangle(exit_btn.btn_rect.x, exit_btn.btn_rect.y, exit_btn.btn_rect.width, exit_btn.btn_rect.height, RAYWHITE);

        DrawText(play_btn.text, play_btn.btn_pos.x, play_btn.btn_pos.y, 40, BLACK);
        DrawText(conf_btn.text, conf_btn.btn_pos.x, conf_btn.btn_pos.y, 40, BLACK);
        DrawText(exit_btn.text, exit_btn.btn_pos.x, exit_btn.btn_pos.y, 40, BLACK);

        break;
    case SETTINGS:          // Draw Settings Screen

        if (frameLimiter.enabled) DrawText(frameLimiter.text, frameLimiter.btn_rect.x, frameLimiter.btn_rect.y, 20, RED);
        else DrawText(frameLimiter.text, frameLimiter.btn_rect.x, frameLimiter.btn_rect.y, 20, BLACK);

        if (fullScreen.enabled) DrawText(fullScreen.text, fullScreen.btn_rect.x, fullScreen.btn_rect.y, 20, RED);
        else DrawText(fullScreen.text, fullScreen.btn_rect.x, fullScreen.btn_rect.y, 20, BLACK);

        DrawRectangle(play_btn_copy.btn_rect.x, play_btn_copy.btn_rect.y, play_btn_copy.btn_rect.width, play_btn_copy.btn_rect.height, RAYWHITE);
        DrawRectangle(exit_btn.btn_rect.x, exit_btn.btn_rect.y, exit_btn.btn_rect.width, exit_btn.btn_rect.height, RAYWHITE);

        DrawText(play_btn_copy.text, play_btn_copy.btn_pos.x, play_btn_copy.btn_pos.y, 40, BLACK);
        DrawText(exit_btn.text, exit_btn.btn_pos.x, exit_btn.btn_pos.y, 40, BLACK);

        break;
    case GAMEPLAY:          // Draw Gameplay
        if (!gameOver)
        {
            // Draw player bar
            DrawRectangle(player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y, BLACK);

            // Draw player lives
            for (int i = 0; i < player.life; i++) DrawRectangle(20 + 40 * i, screenHeight - 30, 35, 10, RED);

            // Draw ball
            DrawCircleV(ball.position, ball.radius, MAROON);

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
            DrawText(TextFormat("%04i", score), screenWidth - 150, screenHeight - 50, 40, BLACK);

            if (comboMultiplier > 2)
                DrawText(TextFormat("%01ix Combo!", comboMultiplier - 1), GetScreenWidth() / 2 - 50, GetScreenHeight() / 2, 20, GameManager::GetColor(comboMultiplier));

            if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
        }
        else
        {
            DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 80, 20, GRAY);
            DrawText("PRESS [BACKSPACE] TO RETURN TO MAIN MENU", GetScreenWidth() / 2 - MeasureText("PRESS [BACKSPACE] TO RETURN TO MAIN MENU", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
            DrawText(TextFormat("%04i", score), GetScreenWidth() / 2 - MeasureText(TextFormat("%04i", score), 40) / 2, GetScreenHeight() / 2, 40, BLACK);
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