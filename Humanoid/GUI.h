#pragma once

#include <raylib.h>

constexpr int screenWidth = 800;
constexpr int screenHeight = 600;

static const Vector2 btn_size = { screenWidth / 4, screenHeight / 12 };

struct Button {
	char text[12] = "";
	Vector2 btn_pos;
	Rectangle btn_rect = { btn_pos.x, btn_pos.y, btn_size.x, btn_size.y };
};

struct ToggleButton {
	bool enabled;
	char text[32] = "";
	Rectangle btn_rect;
};

static Button play_btn = { "   Play  ", screenWidth / 2.5f, screenHeight / 3 };
static Button conf_btn = { " Settings", screenWidth / 2.5f, screenHeight / 3 * 1.5f };
static Button exit_btn = { "Quit Game", screenWidth / 2.5f, screenHeight / 3 * 2.0f };
static Button play_btn_copy = { "   Play  ", screenWidth / 2.5f, screenHeight / 3 * 1.5f };

static ToggleButton frameLimiter = { true, "Frame Limiter on/off", screenWidth / 3, screenHeight / 4, screenWidth / 3, screenHeight / 22 };
static ToggleButton fullScreen = { false, "Toggle Fullscreen on/off", screenWidth / 3, screenHeight / 3, screenWidth / 3, screenHeight / 22 };

static Sound btnSfx;
static Image appIcon;
