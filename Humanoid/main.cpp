// Author: Uljas Antero Lindell 2021
// Version 1.02

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Arkanoid.h"


int main()
{
    Arkanoid game;
    game.RunApplication();
    return 0;
}