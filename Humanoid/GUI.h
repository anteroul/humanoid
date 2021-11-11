#pragma once

#include "arkanoid.h"

static const Vector2 btn_size = { screenWidth / 4, screenHeight / 12 };

typedef struct Button {
	char text[12] = "";
	Vector2 btn_pos;
	Rectangle btn_rect = {btn_pos.x, btn_pos.y, btn_size.x, btn_size.y};
} Button;

typedef struct ToggleButton {
	bool enabled;
	char text[32] = "";
	Rectangle btn_rect;
} ToggleButton;

static Button play_btn = { "   Play  ", screenWidth / 2.5f, screenHeight / 3 };
static Button conf_btn = { " Settings", screenWidth / 2.5f, screenHeight / 3 * 1.5f };
static Button exit_btn = { "Quit Game", screenWidth / 2.5f, screenHeight / 3 * 2.0f };
static Button play_btn_copy = { "   Play  ", screenWidth / 2.5f, screenHeight / 3 * 1.5f };

static ToggleButton frameLimiter = { false, "Frame Limiter on/off", screenWidth / 3, screenHeight / 4, screenWidth / 2, screenHeight / 18 };
static ToggleButton fullScreen = { false, "Toggle Fullsceen on/off", screenWidth / 3, screenHeight / 3, screenWidth / 2, screenHeight / 18 };