#include "GUI.h"

GUI::GUI()
{
	btn_size = { screenWidth / 4, screenHeight / 12 };
	play_btn = {"   Play  ", screenWidth / 2.5f, screenHeight / 3};
	conf_btn = { " Settings", screenWidth / 2.5f, screenHeight / 3 * 1.5f };
	exit_btn = { "Quit Game", screenWidth / 2.5f, screenHeight / 3 * 2.0f };
	play_btn_copy = { "   Play  ", screenWidth / 2.5f, screenHeight / 3 * 1.5f };

	frameLimiter = { true, "Frame Limiter on/off", screenWidth / 3, screenHeight / 4, screenWidth / 3, screenHeight / 22 };
	fullScreen = { false, "Toggle Fullsceen on/off", screenWidth / 3, screenHeight / 3, screenWidth / 3, screenHeight / 22 };
}

GUI::~GUI() = default;