// Author: Uljas Antero Lindell 2021
// Version 1.1

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Arkanoid.h"

int main()
{
    Arkanoid game;
    game.RunApplication();
    return 0;
}