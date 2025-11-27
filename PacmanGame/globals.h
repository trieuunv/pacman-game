#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>

#define M_PI 3.14159265358979323846

// Window settings
extern int width;
extern int height;
extern int interval;

// Game settings
extern float square_side;
extern int pebble_counter;
extern float pacman_speed;
extern float ghost_speed;
extern int open;
extern int direction;
extern bool move;
extern bool still_alive;
extern bool is_blue_mode;
extern int duration;

// Board with item support
// 0 = wall
// 1 = pellet
// -1 = eaten
// 2 = ghost house
// 3 = cherry
// 4 = strawberry
// 5 = orange
// 6 = star
extern int board[31][28];

#endif