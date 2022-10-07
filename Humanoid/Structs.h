#pragma once

#include <raylib.h>

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