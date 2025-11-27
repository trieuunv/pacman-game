/*
DEFINE FUNCTIONS AND OBJECTS THAT DEAL WITH THE ACTUAL GAMEPLAY LOGIC
*/

#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "globals.h"
#include "ghost.h"
#include "pacman.h"
#include "drawing.h"

void drawLives();

void restart();

void keyboard();

void movePacman();

void checkScore();

void moveGhost(Ghost& ghost);

void checkCollision(Ghost& ghost);

void checkColisions();

std::string int2str(int x);

void resetBoard();

void draw();

void update(int value);

void enable2D(int width, int height);

#endif // !GAME_LOGIC_H
