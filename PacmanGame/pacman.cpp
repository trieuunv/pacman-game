#include "globals.h"
#include "pacman.h"

Pacman::Pacman(int i, int j) {
    position[0] = i;
    position[1] = j;
    score = 0;
    lives = 3;
    coordinates[0] = j * 30;
    coordinates[1] = 900 - i * 30;
    orientation = 240;
}

void Pacman::increaseScore(int points) {
    score += points;
}

void Pacman::die() {
    if (lives > 0) {
        lives -= 1;
    }
    else {
        still_alive = false;
    }
}

int Pacman::getX() {
    return coordinates[0];
}

void Pacman::setX(int x) {
    coordinates[0] = x;
}

int Pacman::getY() {
    return coordinates[1];
}

void Pacman::setY(int y) {
    coordinates[1] = y;
}

int Pacman::getI() {
    return position[0];
}

int Pacman::getJ() {
    return position[1];
}

int Pacman::getScore() {
    return score;
}

int Pacman::getLives() {
    return lives;
}

void Pacman::resetLives() {
    lives = 3;
}

void Pacman::resetScore() {
    score = 0;
}

void Pacman::moveUp(int distance) {
    coordinates[1] += distance;
    if (coordinates[1] % 30 == 0) {
        position[0] = 30 - coordinates[1] / 30;
    }
}

void Pacman::moveDown(int distance) {
    coordinates[1] -= distance;
    if (coordinates[1] % 30 == 0) {
        position[0] = 30 - coordinates[1] / 30;
    }
}

void Pacman::moveRight(int distance) {
    coordinates[0] += distance;
    if (coordinates[0] % 30 == 0) {
        position[1] = coordinates[0] / 30;
    }
}

void Pacman::moveLeft(int distance) {
    coordinates[0] -= distance;
    if (coordinates[0] % 30 == 0) {
        position[1] = coordinates[0] / 30;
    }
}

void Pacman::setOrientation(int o) {
    orientation = o;
}

int Pacman::getOrientation() {
    return orientation;
}

// FIXED: Check wall properly - only walls (0) and ghost house (2) are blocked
bool Pacman::checkWall(int i, int j) {
    // Out of bounds check
    if (i < 0 || i >= 31 || j < 0 || j >= 28) {
        return true;
    }

    // Only wall (0) and ghost house (2) are blocked
    // All other values (1=pellet, -1=eaten, 3-6=items) are walkable
    if (board[i][j] == 0 || board[i][j] == 2)
        return true;
    return false;
}

void Pacman::moveTo(int i, int j) {
    position[0] = i;
    position[1] = j;
    coordinates[0] = j * 30;
    coordinates[1] = 900 - i * 30;
}

void Pacman::moveToStart() {
    coordinates[0] = 30;
    coordinates[1] = 870;
    position[0] = 1;
    position[1] = 1;
}